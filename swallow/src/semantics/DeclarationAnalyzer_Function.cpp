/* DeclarationAnalyzer_Function.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/SemanticAnalyzer.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/TypeBuilder.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/SymbolScope.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/GlobalScope.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "common/ScopedValue.h"
#include "semantics/SemanticContext.h"
#include "semantics/ReturnStatementValidator.h"
#include "semantics/InitializerValidator.h"
#include "semantics/InitializationTracer.h"
#include <set>
#include <cassert>
#include <ast/NodeFactory.h>

USE_SWALLOW_NS
using namespace std;

void DeclarationAnalyzer::visitParameter(const ParameterNodePtr& node)
{
    TypePtr type = lookupType(node->getDeclaredType());
    node->setType(type);
    //check if local name is already defined
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->lookup(node->getLocalName());
    if(sym)
    {
        error(node, Errors::E_DEFINITION_CONFLICT, node->getLocalName());
        return;
    }
    if(node->getLocalName() == node->getExternalName())
    {
        warning(node, Errors::W_PARAM_CAN_BE_EXPRESSED_MORE_SUCCINCTLY_1, node->getExternalName());
    }
    //TODO: In-out parameters cannot have default values, and variadic parameters cannot be marked as inout. If you mark a parameter as inout, it cannot also be marked as var or let.

    //Protocols use the same syntax as normal methods, but are not allowed to specify default values for method parameters.
    if(node->getDefaultValue())
    {
        error(node, Errors::E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD);
    }


}
void DeclarationAnalyzer::visitParameters(const ParametersNodePtr& node)
{
    NodeVisitor::visitParameters(node);
    //check variadic parameter, the last parameter cannot be marked as inout parameter
    if(node->isVariadicParameters())
    {
        ParameterNodePtr param = node->getParameter(node->numParameters() - 1);
        if(param->isInout())
        {
            error(param, Errors::E_INOUT_ARGUMENTS_CANNOT_BE_VARIADIC);
            return;
        }
    }
    std::set<std::wstring> externalNames;
    //check extraneous shorthand external names
    for(const ParameterNodePtr& param : *node)
    {
        if(!param->getExternalName().empty())
            externalNames.insert(param->getExternalName());
    }
    for(const ParameterNodePtr& param : *node)
    {
        if(param->isShorthandExternalName())
        {
            if(externalNames.find(param->getLocalName()) != externalNames.end())
            {
                warning(param, Errors::W_EXTRANEOUS_SHARTP_IN_PARAMETER_1, param->getLocalName());
            }
        }
    }
}



TypePtr DeclarationAnalyzer::createFunctionType(const std::vector<ParametersNodePtr>::const_iterator& begin, const std::vector<ParametersNodePtr>::const_iterator& end, const TypePtr& retType, const GenericDefinitionPtr& generic)
{
    if(begin == end)
        return retType;

    TypePtr returnType = createFunctionType(begin + 1, end, retType, generic);

    std::vector<Parameter> parameterTypes;
    ParametersNodePtr params = *begin;
    {
        for (const ParameterNodePtr &param : *params)
        {
            TypePtr paramType = param->getType();
            if(!paramType)
            {
                param->accept(this);
                paramType = param->getType();
                assert(paramType != nullptr);
            }
            assert(paramType != nullptr);
            std::wstring externalName;
            if(param->isShorthandExternalName())
            {
                externalName = param->getLocalName();
            }
            else
            {
                if(param->getExternalName() != L"_")
                    externalName = param->getExternalName();
                else
                    externalName = L"";
            }
            parameterTypes.push_back(Parameter(externalName, param->isInout(), paramType));
        }
    }

    return Type::newFunction(parameterTypes, returnType, params->isVariadicParameters(), generic);
}

FunctionSymbolPtr DeclarationAnalyzer::createFunctionSymbol(const FunctionDefPtr& func, const GenericDefinitionPtr& generic)
{
    std::map<std::wstring, TypePtr> genericTypes;



    TypePtr retType = symbolRegistry->getGlobalScope()->Void();
    if(func->getReturnType())
        retType = lookupType(func->getReturnType());
    TypePtr funcType = createFunctionType(func->getParametersList().begin(), func->getParametersList().end(), retType, generic);
    //prepare flags
    if(ctx->currentType)
    {
        Type::Category category = ctx->currentType->getCategory();
        bool valueType = category == Type::Struct || category == Type::Enum;
        bool prototype = category == Type::Protocol;
        bool mutating = func->hasModifier(DeclarationModifiers::Mutating);
        if(!prototype && (!valueType || mutating))
            funcType->setFlags(SymbolFlagMutating, true);
        funcType->setFlags(SymbolFlagMember, true);
    }
    FunctionSymbolPtr ret(new FunctionSymbol(func->getName(), funcType, func->getBody()));
    return ret;
}
void DeclarationAnalyzer::visitClosure(const ClosurePtr& node)
{
    ScopedClosurePtr closure = std::static_pointer_cast<ScopedClosure>(node);

    //check contextual type
    if(ctx->contextualType && ctx->contextualType->getCategory() != Type::Function)
    {
        error(node, Errors::E_FUNCTION_PROCEDURES_EXPECTD_TYPE_A_DID_YOU_MEAN_TO_CALL_IT_WITH_1, ctx->contextualType->toString());
        return;
    }

    TypePtr returnedType = this->lookupType(node->getReturnType());
    std::vector<Parameter> params;
    if(node->getParameters())
    {
        node->getParameters()->accept(this);
        prepareParameters(closure->getScope(), node->getParameters());

        //create a function type for this
        for(const ParameterNodePtr& param : *node->getParameters())
        {
            TypePtr type = lookupType(param->getDeclaredType());
            assert(type != nullptr);
            const std::wstring& name = param->isShorthandExternalName() ? param->getLocalName() : param->getExternalName();
            params.push_back(Parameter(name, param->isInout(), type));
        }
    }
    if(ctx->contextualType)
    {
        node->setType(ctx->contextualType);
    }
    else
    {
        TypePtr type = Type::newFunction(params, returnedType, node->getParameters()->isVariadicParameters());
        node->setType(type);
    }

    if(node->getCapture())
    {
        node->getCapture()->accept(semanticAnalyzer);
    }

    SCOPED_SET(ctx->currentFunction, node->getType());

    for(const StatementPtr& st : *node)
    {
        st->accept(semanticAnalyzer);
    }

}

/*!
 * Register symbol self/super to given code block
 */
static void registerSelfSuper(const TypePtr& currentType, const TypePtr& currentFunction, const CodeBlockPtr& cb, int modifiers)
{
    SymbolScope* scope = static_pointer_cast<ScopedCodeBlock>(cb)->getScope();
    if (!currentType)
        return;
    {
        int flags = SymbolFlagReadable | SymbolFlagInitialized;
        bool valueType = currentType->getCategory() == Type::Enum || currentType->getCategory() == Type::Struct;
        if (valueType && (modifiers & DeclarationModifiers::Mutating))
        {
            //mutating function inside enum/struct
            flags |= SymbolFlagWritable;
        }
        else if (valueType && currentFunction && (currentFunction->hasFlags(SymbolFlagInit)))
        {
            //initializer inside enum/struct
            flags |= SymbolFlagWritable;
        }
        else if (valueType)
        {
            //fields are not-modifiable in value type without mutating keyword
            flags |= SymbolFlagNonmutating;
        }
        else
        {
            //ref-type self symbol is read only but mutating
        }
        SymbolPlaceHolderPtr self(new SymbolPlaceHolder(L"self", currentType, SymbolPlaceHolder::R_PARAMETER, flags));
        scope->addSymbol(self);
    }
    if(currentType->getParentType())
    {
        TypePtr type = currentType->getParentType();
        int flags = SymbolFlagReadable | SymbolFlagInitialized;
        SymbolPlaceHolderPtr super(new SymbolPlaceHolder(L"super", type, SymbolPlaceHolder::R_PARAMETER, flags));
        scope->addSymbol(super);
    }
}

void DeclarationAnalyzer::checkForPropertyOverriding(const std::wstring& name, const SymbolPlaceHolderPtr& decl, const ComputedPropertyPtr& node)
{
    TypePtr type = ctx->currentType;
    if(!type)
        return;
    MemberFilter filter = (MemberFilter)(FilterLookupInExtension | (decl->hasFlags(SymbolFlagStatic) ? FilterStaticMember : 0));
    SymbolPtr member = semanticAnalyzer->getMemberFromType(type, name, filter);
    if(member && member != decl)
    {
        error(node, Errors::E_INVALID_REDECLARATION_1, name);
        return;
    }
    bool hasOverride = node->hasModifier(DeclarationModifiers::Override);
    //look for ancestors
    type = type->getParentType();
    TypePtr matchedType = nullptr;
    SymbolPtr matchedMember = nullptr;
    for(; type; type = type->getParentType())
    {
        SymbolPtr member = semanticAnalyzer->getMemberFromType(type, name, filter);
        if(member && member != decl)
        {
            matchedType = type;
            matchedMember = member;
            break;
        }
    }
    if(!hasOverride && matchedType)
    {
        error(node, Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
        return;
    }
    if(hasOverride && !matchedType)
    {
        error(node, Errors::E_PROPERTY_DOES_NOT_OVERRIDE_ANY_PROPERTY_FROM_ITS_SUPERCLASS);
        return;
    }
    if(hasOverride && matchedType)
    {
        if(!Type::equals(decl->getType(), matchedMember->getType()))
        {
            error(node, Errors::E_PROPERTY_A_WITH_TYPE_B_CANNOT_OVERRIDE_A_PROPERTY_WITH_TYPE_C_3, name, decl->getType()->toString(), matchedMember->getType()->toString());
            return;
        }
        if(matchedMember->hasFlags(SymbolFlagWritable) && !decl->hasFlags(SymbolFlagWritable))
        {
            error(node, Errors::E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1, name);
            return;
        }
        if(matchedMember->hasFlags(SymbolFlagFinal))
        {
            error(node, Errors::E_VAR_OVERRIDES_A_FINAL_VAR);
            return;
        }
    }

}
/*!
 * Create property accessor
 */
static SymboledFunctionPtr createPropertyAccessor(const ComputedPropertyPtr& node, bool mutating, const CodeBlockPtr& body, const std::wstring& suffix, const std::wstring& argName)
{
    NodeFactory* nodeFactory = node->getNodeFactory();
    const SourceInfo* si = body ? body->getSourceInfo() : node->getSourceInfo();
    SymboledFunctionPtr func =  static_pointer_cast<SymboledFunction>(nodeFactory->createFunction(*si));
    func->setBody(body);
    func->setKind(FunctionKind::FunctionKindNormal);
    int modifiers = node->getModifiers() | DeclarationModifiers::_Generated;
    if(mutating)
        modifiers |= DeclarationModifiers::Mutating;
    func->setModifiers(modifiers);
    func->setName(node->getName() + suffix);
    if(!argName.empty())
    {
        //it's a setter, no return value
        TypeIdentifierPtr Void = nodeFactory->createTypeIdentifier(*body->getSourceInfo());
        Void->setName(L"Void");
        func->setReturnType(Void);
    }
    else
    {
        func->setReturnType(node->getDeclaredType());
    }
    ParametersNodePtr params = nodeFactory->createParameters(*si);
    if(!argName.empty())
    {
        ParameterNodePtr param = nodeFactory->createParameter(*si);
        param->setLocalName(argName);
        param->setDeclaredType(node->getDeclaredType());
        params->addParameter(param);
    }
    func->addParameters(params);
    return func;
}
void DeclarationAnalyzer::visitComputedProperty(const ComputedPropertyPtr& node)
{
    CodeBlockPtr didSet = node->getDidSet();
    CodeBlockPtr willSet = node->getWillSet();
    CodeBlockPtr getter = node->getGetter();
    CodeBlockPtr setter = node->getSetter();
    TypePtr type = lookupType(node->getDeclaredType());
    assert(type != nullptr);

    shared_ptr<ComposedComputedProperty> property = static_pointer_cast<ComposedComputedProperty>(node);


    if(ctx->flags & SemanticContext::FLAG_PROCESS_DECLARATION)
    {
        node->setType(type);

        //register symbol
        int flags = SymbolFlagInitialized;
        if (didSet || willSet)
            flags |= SymbolFlagReadable | SymbolFlagWritable;
        if (setter)
            flags |= SymbolFlagWritable;
        if (getter)
            flags |= SymbolFlagReadable;
        if (node->hasModifier(DeclarationModifiers::Static) || node->hasModifier(DeclarationModifiers::Class))
            flags |= SymbolFlagStatic;
        flags |= SymbolFlagComputedProperty;
        if(ctx->currentType)
            flags |= SymbolFlagMember;
        if(node->hasModifier(DeclarationModifiers::Final))
            flags |= SymbolFlagFinal;

        SymbolPlaceHolderPtr symbol(new SymbolPlaceHolder(node->getName(), type, SymbolPlaceHolder::R_PROPERTY, flags));
        SymbolScope* scope = symbolRegistry->getCurrentScope();
        scope->addSymbol(symbol);
        declarationFinished(symbol->getName(), symbol, node);
        checkForPropertyOverriding(node->getName(), symbol, node);
        validateDeclarationModifiers(node);

        SCOPED_SET(ctx->flags, (ctx->flags & (~SemanticContext::FLAG_PROCESS_IMPLEMENTATION)) | SemanticContext::FLAG_PROCESS_DECLARATION);
        Type::Category currentCategory = ctx->currentType->getCategory();
        bool isProtocol = currentCategory == Type::Protocol;
        //create function for setter/getter/willSet/didSet
        if(!isProtocol)
        {
            bool valueType = currentCategory == Type::Enum || currentCategory == Type::Struct;
            if (getter)
            {
                property->functions.getter = createPropertyAccessor(node, !valueType, getter, L".getter", L"");
                property->functions.getter->accept(semanticAnalyzer);
            }


            if (setter)// || didSet || willSet)
            {
                std::wstring arg = L"newValue";
                if (!node->getSetterName().empty())
                    arg = node->getSetterName();
                property->functions.setter = createPropertyAccessor(node, true, setter, L".setter", arg);
                property->functions.setter->accept(semanticAnalyzer);
            }

            if (didSet)
            {
                std::wstring arg = L"oldValue";
                if (!node->getDidSetSetter().empty())
                    arg = node->getDidSetSetter();
                property->functions.didSet = createPropertyAccessor(node, true, didSet, L".didSet", arg);
                property->functions.didSet->accept(semanticAnalyzer);
            }

            if (willSet)
            {
                std::wstring arg = L"newValue";
                if (!node->getWillSetSetter().empty())
                    arg = node->getWillSetSetter();
                property->functions.willSet = createPropertyAccessor(node, true, willSet, L".willSet", arg);
                property->functions.willSet->accept(semanticAnalyzer);
            }
        }

    }
    //prepare and visit each accessors
    if(ctx->flags & SemanticContext::FLAG_PROCESS_IMPLEMENTATION)
    {
        //prepare type for getter/setter
        std::vector<Parameter> params;
        TypePtr getterType = Type::newFunction(params, type, nullptr);
        params.push_back(Parameter(type));
        TypePtr setterType = Type::newFunction(params, symbolRegistry->getGlobalScope()->Void(), false);

        SCOPED_SET(ctx->flags, (ctx->flags & (~SemanticContext::FLAG_PROCESS_DECLARATION)) | SemanticContext::FLAG_PROCESS_IMPLEMENTATION);
        if(property->functions.getter)
            property->functions.getter->accept(semanticAnalyzer);
        if(property->functions.setter)
            property->functions.setter->accept(semanticAnalyzer);
        if(property->functions.willSet)
            property->functions.willSet->accept(semanticAnalyzer);
        if(property->functions.didSet)
            property->functions.didSet->accept(semanticAnalyzer);
        /*
        if (getter)
        {
            getter->setType(getterType);
            SCOPED_SET(ctx->currentFunction, getterType);
            registerSelfToAccessor(ctx->currentType, getter);
            getter->accept(this);
        }
        if (setter)
        {
            registerSelfToAccessor(ctx->currentType, setter);
            std::wstring name = node->getSetterName().empty() ? L"newValue" : node->getSetterName();
            //TODO: replace the symbol to internal value
            ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(setter);
            cb->setType(setterType);
            cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(name, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized)));

            SCOPED_SET(ctx->currentFunction, setterType);
            cb->accept(this);
        }
        if (willSet)
        {
            std::wstring setter = node->getWillSetSetter().empty() ? L"newValue" : node->getWillSetSetter();
            //TODO: replace the symbol to internal value
            ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(willSet);
            cb->setType(setterType);
            cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized)));

            SCOPED_SET(ctx->currentFunction, setterType);
            registerSelfToAccessor(ctx->currentType, willSet);
            cb->accept(this);
        }
        if (didSet)
        {
            std::wstring setter = node->getDidSetSetter().empty() ? L"oldValue" : node->getDidSetSetter();
            //TODO: replace the symbol to internal value
            ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(didSet);
            cb->setType(setterType);
            cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized)));

            SCOPED_SET(ctx->currentFunction, setterType);
            registerSelfToAccessor(ctx->currentType, didSet);
            cb->accept(this);
        }
        */
    }
}
void DeclarationAnalyzer::visitAccessor(const CodeBlockPtr& accessor, const ParametersNodePtr& params, const SymbolPtr& setter, int modifiers)
{
    if(!accessor)
        return;
    ScopedCodeBlockPtr codeBlock = std::static_pointer_cast<ScopedCodeBlock>(accessor);
    SymbolScope *scope = codeBlock->getScope();
    ScopeGuard scopeGuard(codeBlock.get(), this);
    (void) scopeGuard;
    registerSelfSuper(ctx->currentType, nullptr, accessor, modifiers);
    if(setter)
        scope->addSymbol(setter);

    params->accept(this);
    prepareParameters(scope, params);

    SCOPED_SET(ctx->currentFunction, accessor->getType());

    accessor->accept(semanticAnalyzer);
}


/*!
 * Find a subscript definition from given type with the same signature to given reference
 */
static const Subscript* getSubscriptFromType(const TypePtr& type, const Subscript& reference)
{
    for(const Subscript& s : type->getSubscripts())
    {
        if(s.parameters.size() != reference.parameters.size())
            continue;
        if(!Type::equals(s.returnType, reference.returnType))
            continue;
        auto iter = s.parameters.begin();
        bool matched = true;
        for(auto param : reference.parameters)
        {
            const Parameter& param2 = *iter++;
            if(param.name != param2.name || !Type::equals(param.type, param2.type))
            {
                matched = false;
                continue;
            }
        }
        if(matched)
            return &s;
    }
    return nullptr;
}
void DeclarationAnalyzer::checkForSubscriptOverride(const Subscript& subscript, const SubscriptDefPtr& node)
{
    //check for duplication
    TypePtr type = ctx->currentType;
    if(getSubscriptFromType(type, subscript))
    {
        error(node, Errors::E_INVALID_REDECLARATION_1, L"subscript");
        return;
    }
    type = type->getParentType();
    TypePtr matchedType = nullptr;
    const Subscript* matchedSubscript = nullptr;
    for(; type; type = type->getParentType())
    {
        matchedSubscript = getSubscriptFromType(type, subscript);
        if(matchedSubscript)
        {
            matchedType = type;
            break;
        }
    }
    bool hasOverride = node->hasModifier(DeclarationModifiers::Override);
    if(matchedSubscript && !hasOverride)
    {
        error(node, Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
        return;
    }
    if(!matchedSubscript && hasOverride)
    {
        error(node, Errors::E_SUBSCRIPT_DOES_NOT_OVERRIDE_ANY_SUBSCRIPT_FROM_ITS_SUPERCLASS);
        return;
    }
    if(matchedSubscript)
    {
        if(matchedSubscript->setter && !subscript.setter)
        {
            error(node, Errors::E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1, L"subscript");
            return;
        }
        if(matchedSubscript->flags & SymbolFlagFinal)
        {
            error(node, Errors::E_SUBSCRIPT_OVERRIDES_A_FINAL_SUBSCRIPT);
            return;
        }
    }

}
void DeclarationAnalyzer::visitSubscript(const SubscriptDefPtr &node)
{
    assert(ctx->currentType != nullptr);

    ParametersNodePtr parameters = node->getParameters();

    // Register subscript as functions to type
    std::vector<ParametersNodePtr> paramsList = {parameters};
    TypePtr retType = lookupType(node->getReturnType());
    if (!node->getGetter() && !node->getSetter())
        return;

    if(ctx->flags & SemanticContext::FLAG_PROCESS_DECLARATION)
    {
        FunctionSymbolPtr getter, setter;
        TypePtr getterType;
        if(!node->getGetter())
        {
            error(node, Errors::E_SUBSCRIPT_DECLARATIONS_MUST_HAVE_A_GETTER);
            return;
        }
        //check for inout in parameter list
        for(auto param : *parameters)
        {
            if(param->isInout())
            {
                error(param, Errors::E_INOUT_IS_ONLY_VALID_IN_PARAMTER_LISTS);
                return;
            }

        }

        //if (node->getGetter())
        {
            ScopeGuard scope(static_cast<ScopedCodeBlock*>(node->getGetter().get()), semanticAnalyzer);
            getterType = this->createFunctionType(paramsList.begin(), paramsList.end(), retType, nullptr);
            node->getGetter()->setType(getterType);
            getter = FunctionSymbolPtr(new FunctionSymbol(L"subscript", getterType, node->getGetter()));
            declarationFinished(getter->getName(), getter, node->getGetter());
        }
        if (node->getSetter())
        {
            //TODO: replace nullptr to void
            ScopeGuard scope(static_cast<ScopedCodeBlock*>(node->getSetter().get()), semanticAnalyzer);
            TypePtr funcType = this->createFunctionType(paramsList.begin(), paramsList.end(), symbolRegistry->getGlobalScope()->Void(), nullptr);
            node->getSetter()->setType(funcType);
            static_pointer_cast<TypeBuilder>(funcType)->addParameter(Parameter(retType));
            setter = FunctionSymbolPtr(new FunctionSymbol(L"subscript", funcType, node->getSetter()));
            declarationFinished(setter->getName(), setter, node->getSetter());
        }
        assert(getter != nullptr);
        TypeBuilderPtr t = static_pointer_cast<TypeBuilder>(ctx->currentType);
        int flags = 0;
        if(node->hasModifier(DeclarationModifiers::Final))
            flags |= SymbolFlagFinal;
        Subscript subscript(getterType->getParameters(), getterType->getReturnType(), getter, setter, flags);
        checkForSubscriptOverride(subscript, node);
        t->addSubscript(subscript);
    }
    if(ctx->flags & SemanticContext::FLAG_PROCESS_IMPLEMENTATION)
    {
        //TypeInference

        TypePtr type = lookupType(node->getReturnType());
        assert(type != nullptr);
        std::vector<Parameter> params;
        for (const ParameterNodePtr &param : *node->getParameters())
        {
            wstring name = param->isShorthandExternalName() ? param->getLocalName() : param->getExternalName();
            TypePtr paramType = lookupType(param->getDeclaredType());
            params.push_back(Parameter(name, param->isInout(), paramType));
        }


        //process getter
        visitAccessor(node->getGetter(), parameters, nullptr, node->getModifiers());
        //process setter
        wstring setterName = L"newValue";
        if (!node->getSetterName().empty())
            setterName = node->getSetterName();
        SymbolPlaceHolderPtr setter(new SymbolPlaceHolder(setterName, retType, SymbolPlaceHolder::R_PARAMETER, SymbolFlagReadable | SymbolFlagInitialized));
        visitAccessor(node->getSetter(), parameters, setter, node->getModifiers());
    }
}
void DeclarationAnalyzer::visitFunctionDeclaration(const FunctionDefPtr& node)
{
    GenericDefinitionPtr generic;
    if(node->getGenericParametersDef())
        generic = prepareGenericTypes(node->getGenericParametersDef());

    //validate function as operator implemention
    if(node->getKind() == FunctionKindOperator)
    {
        int params = node->getParameters(0)->numParameters();
        if(params != 1 && params != 2)
        {
            error(node, Errors::E_OPERATORS_MUST_HAVE_ONE_OR_TWO_ARGUMENTS);
            return;
        }
        int modifiers = node->getModifiers();
        bool isPrefix = modifiers & DeclarationModifiers::Prefix;
        bool isPostfix = modifiers & DeclarationModifiers::Postfix;
        if(isPrefix && params != 1)
        {
            error(node, Errors::E_A_REQUIRES_A_FUNCTION_WITH_ONE_ARGUMENT_1, L"prefix");
            return;
        }
        if(isPostfix && params != 1)
        {
            error(node, Errors::E_A_REQUIRES_A_FUNCTION_WITH_ONE_ARGUMENT_1, L"postfix");
            return;
        }
        if(params == 1 && (!isPrefix && !isPostfix))
        {
            error(node, Errors::E_UNARY_OPERATOR_IMPLEMENTATION_MUST_HAVE_A_PREFIX_OR_POSTFIX_MODIFIER);
            return;
        }
        if(isPostfix && !symbolRegistry->getOperator(node->getName(), OperatorType::PostfixUnary))
        {
            error(node, Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION);
            return;
        }
        if(isPrefix && !symbolRegistry->getOperator(node->getName(), OperatorType::PrefixUnary))
        {
            error(node, Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION);
            return;
        }
        if(params == 2 && !symbolRegistry->getOperator(node->getName(), OperatorType::InfixBinary))
        {
            error(node, Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION);
            return;
        }

    }



    FunctionSymbolPtr func;
    //enter code block's scope
    {
        ScopedCodeBlockPtr codeBlock = std::static_pointer_cast<ScopedCodeBlock>(node->getBody());
        SymbolScope *scope = codeBlock->getScope();

        ScopeGuard scopeGuard(codeBlock.get(), this);
        (void) scopeGuard;

        if(generic)
            generic->registerTo(scope);


        for (const ParametersNodePtr &params : node->getParametersList())
        {
            params->accept(this);
            prepareParameters(scope, params);
        }

        //prepare function's type
        func = createFunctionSymbol(node, generic);
        TypePtr funcType = func->getType();
        node->setType(funcType);
        node->getBody()->setType(funcType);

        lookupType(node->getReturnType());

        if(node->getModifiers() & DeclarationModifiers::Prefix)
            func->setFlags(SymbolFlagPrefix, true);
        else if(node->getModifiers() & DeclarationModifiers::Postfix)
            func->setFlags(SymbolFlagPostfix, true);

        if(node->getModifiers() & (DeclarationModifiers::Class | DeclarationModifiers::Static))
        {
            func->setFlags(SymbolFlagStatic, true);
            funcType->setFlags(SymbolFlagStatic, true);
        }
        if(node->hasModifier(DeclarationModifiers::Final))
        {
            func->setFlags(SymbolFlagFinal, true);
            funcType->setFlags(SymbolFlagFinal, true);
        }
        if(ctx->currentType && (node->getModifiers() & (DeclarationModifiers::Class | DeclarationModifiers::Static)) == 0)
        {
            registerSelfSuper(ctx->currentType, nullptr, codeBlock, node->getModifiers());
        }
    }


    //check if the same symbol has been defined in this scope
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr oldSym = scope->lookup(func->getName());
    SymbolPtr sym;
    if(oldSym)
    {
        FunctionOverloadedSymbolPtr overload = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(oldSym);
        if(!overload)
        {
            //wrap it as overload
            if(FunctionSymbolPtr oldFunc = std::dynamic_pointer_cast<FunctionSymbol>(oldSym))
            {
                overload = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(func->getName()));
                overload->add(oldFunc);
                scope->removeSymbol(oldSym);
                scope->addSymbol(overload);
            }
            else
            {
                //error, symbol with same name exists
                error(node, Errors::E_INVALID_REDECLARATION_1, node->getName());
                abort();
            }
        }
        overload->add(func);
        sym = overload;
    }
    else
    {
        scope->addSymbol(func);
        sym = func;
    }
    static_pointer_cast<SymboledFunction>(node)->symbol = func;
    //validate it and mark it declared
    validateDeclarationModifiers(node);
    checkForFunctionOverriding(sym->getName(), func, node);
    declarationFinished(sym->getName(), func, node);

}
void DeclarationAnalyzer::checkForFunctionOverriding(const std::wstring& name, const FunctionSymbolPtr& decl, const DeclarationPtr& node)
{
    if(!ctx->currentType)
        return;//not for global function
    if(node->hasModifier(DeclarationModifiers::_Generated))
        return;//do not check for generated functions
    bool staticMember = decl->hasFlags(SymbolFlagStatic);
    //check if there's the same signature exists
    {
        std::vector<SymbolPtr> funcs;
        //std::wstring typeName = ctx.currentType->getName();
        semanticAnalyzer->getMethodsFromType(ctx->currentType, name, (MemberFilter)((staticMember ? FilterStaticMember : 0) | FilterLookupInExtension ), funcs);
        for(const SymbolPtr& f : funcs)
        {
            if(f == decl)
                continue;
            if(Type::equals(f->getType(), decl->getType()))
            {
                error(node, Errors::E_INVALID_REDECLARATION_1, name);
                return;
            }
        }
    }

    //check if it override
    if(ctx->currentType->getParentType())
    {
        std::vector<SymbolPtr> funcs;
        semanticAnalyzer->getMethodsFromType(ctx->currentType->getParentType(), name, (MemberFilter)((staticMember ? FilterStaticMember : 0) | (FilterLookupInExtension | FilterRecursive)), funcs);
        SymbolPtr matched = nullptr;
        for(const SymbolPtr& func : funcs)
        {
            if(Type::equals(func->getType(), decl->getType()))
            {
                matched = func;
                break;
            }
        }
        assert(node != nullptr);
        {
            if(ctx->currentExtension && matched)
            {
                error(node, Errors::E_DECLARATIONS_IN_EXTENSIONS_CANNOT_OVERRIDE_YET);
                return;
            }
            if (node->hasModifier(DeclarationModifiers::Override))
            {
                if (!matched)
                {
                    error(node, Errors::E_METHOD_DOES_NOT_OVERRIDE_ANY_METHOD_FROM_ITS_SUPERCLASS);
                    return;
                }
                if(matched->hasFlags(SymbolFlagFinal))
                {
                    error(node, Errors::E_INSTANCE_METHOD_OVERRIDES_A_FINAL_INSTANCE_METHOD);
                    return;
                }
            }
            else
            {
                bool supress = false;
                //it's ok to use keyword 'required' without 'override' on an initializer
                if(decl->getType()->hasFlags(SymbolFlagRequired) && matched && matched->getType()->hasFlags(SymbolFlagRequired))
                    supress = true;
                if (matched && !supress)
                {
                    if(matched->getType()->hasFlags(SymbolFlagRequired))
                        error(node, Errors::E_REQUIRED_MODIFIER_MUST_BE_PRESENT_ON_ALL_OVERRIDES_OF_A_REQUIRED_INITIALIZER);
                    else
                        error(node, Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
                    return;
                }
            }
        }
    }
}

void DeclarationAnalyzer::visitFunction(const FunctionDefPtr& node)
{
    if(ctx->flags & SemanticContext::FLAG_PROCESS_DECLARATION)
    {
        visitFunctionDeclaration(node);
    }
    if(ctx->flags & SemanticContext::FLAG_PROCESS_IMPLEMENTATION)
    {
        //visit implementation
        FunctionSymbolPtr func = static_pointer_cast<SymboledFunction>(node)->symbol;
        assert(func != nullptr);
        SCOPED_SET(ctx->currentFunction, func->getType());
        node->getBody()->accept(semanticAnalyzer);

        if(!Type::equals(func->getType()->getReturnType(), symbolRegistry->getGlobalScope()->Void()))
        {
            //check return in all branches
            ReturnStatementValidator validator(ctx);
            node->getBody()->accept(&validator);
            NodePtr refNode = validator.getRefNode() ? validator.getRefNode() : node;
            ReturnCoverResult  result = validator.getResult();
            if (result & ReturnCoverDeadcode)
            {
                this->warning(refNode, Errors::W_CODE_AFTER_A_WILL_NEVER_BE_EXECUTED_1, L"return");
                return;
            }
            switch (result)
            {
                case ReturnCoverNoResult:
                case ReturnCoverUnmatched:
                case ReturnCoverPartial:
                    error(refNode, Errors::E_MISSING_RETURN_IN_A_FUNCTION_EXPECTED_TO_RETURN_A_1, func->getType()->getReturnType()->toString());
                    return;
                default:
                    break;
            }
        }
    }
}

void DeclarationAnalyzer::visitDeinit(const DeinitializerDefPtr& node)
{
    if(ctx->flags & SemanticContext::FLAG_PROCESS_IMPLEMENTATION)
    {
        node->accept(semanticAnalyzer);
    }
}

void DeclarationAnalyzer::visitInit(const InitializerDefPtr& node)
{
    //TypeDeclaration* declaration = dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner());
    assert(ctx->currentType != nullptr);
    if(ctx->flags & SemanticContext::FLAG_PROCESS_DECLARATION)
    {
        TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(ctx->currentType);

        ScopedCodeBlockPtr body = std::static_pointer_cast<ScopedCodeBlock>(node->getBody());

        ScopeGuard scopeGuard(body.get(), this);
        (void) scopeGuard;
        node->getParameters()->accept(this);

        prepareParameters(body->getScope(), node->getParameters());

        std::vector<Parameter> params;
        for (const ParameterNodePtr &param : *node->getParameters())
        {
            //init's parameter always has an external name unless it's marked by _
            //local name will be used if external name is undeclared
            std::wstring externalName;
            if(param->getExternalName() != L"_")
            {
                if(param->getExternalName().empty())
                    externalName = param->getLocalName();
                else
                    externalName = param->getExternalName();
            }
            params.push_back(Parameter(externalName, param->isInout(), param->getType()));
        }

        TypePtr funcType = Type::newFunction(params, symbolRegistry->getGlobalScope()->Void(), node->getParameters()->isVariadicParameters());
        if(node->hasModifier(DeclarationModifiers::Convenience))
            funcType->setFlags(SymbolFlagConvenienceInit, true);
        funcType->setFlags(SymbolFlagInit, true);
        if(node->isFailable())
            funcType->setFlags(SymbolFlagFailableInitializer, true);
        if(node->isImplicitFailable())
            funcType->setFlags(SymbolFlagImplicitFailableInitializer, true);
        if(node->hasModifier(DeclarationModifiers::Required))
            funcType->setFlags(SymbolFlagRequired, true);

        //prepare implicit self
        registerSelfSuper(ctx->currentType,funcType, body, node->getModifiers());

        validateDeclarationModifiers(node);
        std::wstring name(L"init");
        FunctionSymbolPtr init(new FunctionSymbol(name, funcType, nullptr));
        checkForFunctionOverriding(name, init, node);
        declarationFinished(name, init, node);
        node->getBody()->setType(funcType);
        static_pointer_cast<SymboledInit>(node)->symbol = init;
    }

    if(ctx->flags & SemanticContext::FLAG_PROCESS_IMPLEMENTATION)
    {
        FunctionSymbolPtr init = static_pointer_cast<SymboledInit>(node)->symbol;
        TypePtr funcType = init->getType();
        SCOPED_SET(ctx->currentFunction, funcType);
        {
            InitializationTracer tracer(nullptr, InitializationTracer::Sequence);
            SCOPED_SET(ctx->currentInitializationTracer, &tracer);
            node->getBody()->accept(semanticAnalyzer);
            //check if some stored property is not initialized
            for(const SymbolPtr& storedProp : ctx->currentType->getDeclaredStoredProperties())
            {
                if(!storedProp->hasFlags(SymbolFlagInitialized))
                {
                    error(node, Errors::E_PROPERTY_A_NOT_INITIALIZED, L"self." + storedProp->getName());
                    return;
                }
            }
        }

        if(node->hasModifier(DeclarationModifiers::Convenience))
        {
            //convenience initializer must call designated initializer in all paths
            InitializerValidator validator(ctx, this);
            node->getBody()->accept(&validator);
            NodePtr refNode = validator.getRefNode() ? validator.getRefNode() : node;
            if(validator.getResult() & InitializerCoverMultiple)
            {
                error(refNode, Errors::E_SELF_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER);
                return;
            }
            switch(validator.getResult())
            {
                case InitializerCoverNoResult:
                case InitializerCoverUnmatched:
                case InitializerCoverPartial:
                    error(refNode, Errors::E_SELF_INIT_ISNT_CALLED_ON_ALL_PATHS_IN_DELEGATING_INITIALIZER);
                    return;
                default:
                    break;
            }

        }
        else
        {
            //designated initializer must call designated initializer if parent type has customized initializer
            bool hasCustomizedInitializer = false;
            if(ctx->currentType->getParentType())
            {
                FunctionOverloadedSymbolPtr inits = ctx->currentType->getDeclaredInitializer();
                if(inits)
                {
                    for(const FunctionSymbolPtr& init : *inits)
                    {
                        if(!init->getType()->getParameters().empty())
                        {
                            hasCustomizedInitializer = true;
                            break;
                        }
                    }
                }
            }
            if(hasCustomizedInitializer)
            {
                InitializerValidator validator(ctx, this);
                node->getBody()->accept(&validator);
                NodePtr refNode = validator.getRefNode() ? validator.getRefNode() : node;
                if(validator.getResult() & InitializerCoverMultiple)
                {
                    error(refNode, Errors::E_SUPER_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER);
                    return;
                }
                switch(validator.getResult())
                {
                    case InitializerCoverNoResult:
                    case InitializerCoverUnmatched:
                    case InitializerCoverPartial:
                        error(refNode, Errors::E_SUPER_INIT_ISNT_CALLED_BEFORE_RETURNING_FROM_INITIALIZER);
                        return;
                    default:
                        break;
                }
            }

        }
    }
}

void DeclarationAnalyzer::visitCodeBlock(const CodeBlockPtr &node)
{
    SCOPED_SET(ctx->flags, ctx->flags | SemanticContext::FLAG_PROCESS_IMPLEMENTATION | SemanticContext::FLAG_PROCESS_DECLARATION);
    auto iter = node->begin();
    for(; iter != node->end(); iter++)
    {
        StatementPtr st = *iter;
        if(BinaryOperatorPtr op = dynamic_pointer_cast<BinaryOperator>(st))
            *iter = semanticAnalyzer->transformExpression(nullptr, op);
        st->accept(semanticAnalyzer);
    }
}
