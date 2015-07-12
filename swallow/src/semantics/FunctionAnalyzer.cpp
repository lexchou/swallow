/* FunctionAnalyzer.cpp --
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
#include "semantics/FunctionAnalyzer.h"
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
#include <set>
#include <cassert>
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/SemanticContext.h"
#include "semantics/ReturnStatementValidator.h"
#include "semantics/InitializerValidator.h"
#include "semantics/InitializationTracer.h"
#include "semantics/SemanticAnalyzer.h"

USE_SWALLOW_NS
using namespace std;

FunctionAnalyzer::FunctionAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults, SemanticContext* context, SemanticAnalyzer* semanticAnalyzer, DeclarationAnalyzer* declarationAnalyzer)
    :SemanticPass(symbolRegistry, compilerResults)
{
    this->ctx = context;
    this->semanticAnalyzer = semanticAnalyzer;
    this->declarationAnalyzer = declarationAnalyzer;
}


//Enter type scope
void FunctionAnalyzer::visitEnum(const EnumDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);

    semanticAnalyzer->visitImplementation(node, this, true);
}
void FunctionAnalyzer::visitClass(const ClassDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    semanticAnalyzer->visitImplementation(node, this, true);
}
void FunctionAnalyzer::visitStruct(const StructDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    semanticAnalyzer->visitImplementation(node, this, true);
}
void FunctionAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    semanticAnalyzer->visitImplementation(node, this, true);
}

void FunctionAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    SemanticExtensionDefPtr extensionNode = static_pointer_cast<SemanticExtensionDef>(node);
    TypePtr extension = extensionNode->extension;
    assert(extension != nullptr);
    TypePtr type = extension->getInnerType();
    assert(type != nullptr);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    SCOPED_SET(ctx->currentExtension, extension);
    semanticAnalyzer->visitImplementation(node, this, true);
}



void FunctionAnalyzer::visitClosure(const ClosurePtr& node)
{
    ScopedClosurePtr closure = std::static_pointer_cast<ScopedClosure>(node);
    TypePtr type = node->getType();
    assert(type != nullptr);
    SCOPED_SET(ctx->currentFunction, node->getType());
    for(const StatementPtr& st : *node)
    {
        st->accept(semanticAnalyzer);
    }
}

void FunctionAnalyzer::visitSubscript(const SubscriptDefPtr &node)
{
    //TypeInference
    ParametersNodePtr parameters = node->getParameters();
    TypePtr retType = declarationAnalyzer->resolveType(node->getReturnType(), true);

    TypePtr type = declarationAnalyzer->resolveType(node->getReturnType(), true);
    assert(type != nullptr);
    std::vector<Parameter> params;
    for (const ParameterNodePtr &param : *node->getParameters())
    {
        wstring name = param->isShorthandExternalName() ? param->getLocalName() : param->getExternalName();
        TypePtr paramType = declarationAnalyzer->resolveType(param->getDeclaredType(), true);
        params.push_back(Parameter(name, param->isInout(), paramType));
        //verify index's access level
        declarationAnalyzer->verifyAccessLevel(node, paramType, DeclarationTypeSubscript, ComponentTypeIndex);
    }
    //verify element type's access level
    declarationAnalyzer->verifyAccessLevel(node, type, DeclarationTypeSubscript, ComponentTypeType);


    //process getter
    visitAccessor(node->getGetter(), parameters, nullptr, node->getModifiers());
    //process setter
    wstring setterName = L"newValue";
    if (!node->getSetterName().empty())
        setterName = node->getSetterName();
    SymbolPlaceHolderPtr setter(new SymbolPlaceHolder(setterName, retType, SymbolPlaceHolder::R_PARAMETER, SymbolFlagReadable | SymbolFlagInitialized));
    visitAccessor(node->getSetter(), parameters, setter, node->getModifiers());
}


void FunctionAnalyzer::visitAccessor(const CodeBlockPtr& accessor, const ParametersNodePtr& params, const SymbolPtr& setter, int modifiers)
{
    if(!accessor)
        return;
    ScopedCodeBlockPtr codeBlock = std::static_pointer_cast<ScopedCodeBlock>(accessor);
    SymbolScope *scope = codeBlock->getScope();
    ScopeGuard scopeGuard(codeBlock.get(), this);
    (void) scopeGuard;
    if(setter)
        scope->addSymbol(setter);
    params->accept(this);
    declarationAnalyzer->prepareParameters(scope, params);

    SCOPED_SET(ctx->currentFunction, accessor->getType());

    accessor->accept(semanticAnalyzer);
}

void FunctionAnalyzer::visitFunction(const FunctionDefPtr& node)
{
    if(node->hasModifier(DeclarationModifiers::_Generated))
        return;
    //visit implementation
    FunctionSymbolPtr func = static_pointer_cast<SymboledFunction>(node)->symbol;
    if(func->getDeclaringType() && func->getDeclaringType()->getCategory() == Type::Protocol)
        return;//do not check implementation if it's defined as protocol function
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

void FunctionAnalyzer::visitDeinit(const DeinitializerDefPtr& node)
{
    TypePtr funcType = ctx->currentType->getDeinit()->getType();
    SCOPED_SET(ctx->currentFunction, funcType);
    node->getBody()->accept(semanticAnalyzer);
}

void FunctionAnalyzer::visitInit(const InitializerDefPtr& node)
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

void FunctionAnalyzer::visitCodeBlock(const CodeBlockPtr &node)
{
}


void FunctionAnalyzer::visitComputedProperty(const ComputedPropertyPtr& node)
{
    CodeBlockPtr didSet = node->getDidSet();
    CodeBlockPtr willSet = node->getWillSet();
    CodeBlockPtr getter = node->getGetter();
    CodeBlockPtr setter = node->getSetter();
    TypePtr type = declarationAnalyzer->resolveType(node->getDeclaredType(), true);
    assert(type != nullptr);

    shared_ptr<ComposedComputedProperty> property = static_pointer_cast<ComposedComputedProperty>(node);
    //prepare type for getter/setter
    /*
    std::vector<Parameter> params;
    TypePtr getterType = Type::newFunction(params, type, nullptr);
    params.push_back(Parameter(type));
    TypePtr setterType = Type::newFunction(params, symbolRegistry->getGlobalScope()->Void(), false);

    */
    SCOPED_SET(ctx->flags, (ctx->flags & (~SemanticContext::FLAG_PROCESS_DECLARATION)) | SemanticContext::FLAG_PROCESS_IMPLEMENTATION);
    if(!property->hasModifier(DeclarationModifiers::_Generated))
    {
        if(property->functions.getter)
            property->functions.getter->accept(semanticAnalyzer);
        if(property->functions.setter)
            property->functions.setter->accept(semanticAnalyzer);
        if(property->functions.willSet)
            property->functions.willSet->accept(semanticAnalyzer);
        if(property->functions.didSet)
            property->functions.didSet->accept(semanticAnalyzer);
    }
}
