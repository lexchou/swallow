/* SemanticAnalyzer_Variables.cpp --
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
#include "SemanticAnalyzer.h"
#include "ast/ast.h"
#include "Symbol.h"
#include "SymbolScope.h"
#include "SymbolRegistry.h"
#include "common/Errors.h"
#include "ScopedNodes.h"
#include "TypeBuilder.h"
#include "common/CompilerResults.h"
#include <cassert>
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "GlobalScope.h"
#include "ast/NodeFactory.h"

USE_SWALLOW_NS
using namespace std;

void SemanticAnalyzer::visitAssignment(const AssignmentPtr& node)
{
    //check node's LHS, it must be a tuple or identifier
    PatternPtr pattern = node->getLHS();
    pattern->accept(this);
    TypePtr destinationType = pattern->getType();
    assert(destinationType != nullptr);
    StackedValueGuard<TypePtr> contextualType(this->t_hint);
    contextualType.set(destinationType);
    verifyTuplePattern(pattern);
    node->getRHS()->accept(this);
    TypePtr sourceType = node->getRHS()->getType();
    assert(sourceType != nullptr);
    if(!sourceType->canAssignTo(destinationType))
    {
        error(node, Errors::E_A_IS_NOT_CONVERTIBLE_TO_B_2, sourceType->toString(), destinationType->toString());
        return;
    }
}
void SemanticAnalyzer::verifyTuplePattern(const PatternPtr& pattern)
{

    if(pattern->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        TypePtr type = symbolRegistry->lookupType(id->getIdentifier());
        if(type)
        {
            //cannot assign expression to type
            error(id, Errors::E_CANNOT_ASSIGN, id->getIdentifier());
            return;
        }
        SymbolPtr sym = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(!sym)
        {
            error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const PatternPtr& element : *tuple)
        {
            verifyTuplePattern(element);
        }
    }
    else
    {
        error(pattern, Errors::E_CANNOT_ASSIGN);
        return;
    }

}

void SemanticAnalyzer::visitComputedProperty(const ComputedPropertyPtr& node)
{

    CodeBlockPtr didSet = node->getDidSet();
    CodeBlockPtr willSet = node->getWillSet();
    CodeBlockPtr getter = node->getGetter();
    CodeBlockPtr setter = node->getSetter();
    TypePtr type = lookupType(node->getDeclaredType());
    assert(type != nullptr);
    node->setType(type);
    //prepare type for getter/setter
    std::vector<Type::Parameter> params;
    TypePtr getterType = Type::newFunction(params, type, nullptr);
    params.push_back(Type::Parameter(type));
    TypePtr setterType = Type::newFunction(params, symbolRegistry->getGlobalScope()->Void, false);

    if(getter)
    {
        getter->setType(getterType);
        getter->accept(this);
    }
    if(setter)
    {
        std::wstring name = node->getSetterName().empty() ? L"newValue" : node->getSetterName();
        //TODO: replace the symbol to internal value
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(setter);
        cb->setType(setterType);
        cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(name, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized)));
        cb->accept(this);
    }
    if(willSet)
    {
        std::wstring setter = node->getWillSetSetter().empty() ? L"newValue" : node->getWillSetSetter();
        //TODO: replace the symbol to internal value
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(willSet);
        cb->setType(setterType);
        cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized)));
        cb->accept(this);
    }
    if(didSet)
    {
        std::wstring setter = node->getDidSetSetter().empty() ? L"oldValue" : node->getDidSetSetter();
        //TODO: replace the symbol to internal value
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(didSet);
        cb->setType(setterType);
        cb->getScope()->addSymbol(SymbolPtr(new SymbolPlaceHolder(setter, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized)));
        cb->accept(this);
    }
    //register symbol
    int flags = SymbolFlagInitialized;
    if(didSet || willSet)
        flags |= SymbolFlagReadable | SymbolFlagWritable;
    if(setter)
        flags |= SymbolFlagWritable;
    if(getter)
        flags |= SymbolFlagReadable;
    SymbolPlaceHolderPtr symbol(new SymbolPlaceHolder(node->getName(), type, SymbolPlaceHolder::R_PROPERTY, flags));
    registerSymbol(symbol);

}
void SemanticAnalyzer::registerSymbol(const SymbolPlaceHolderPtr& symbol)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    NodeType::T nodeType = scope->getOwner()->getNodeType();
    if(nodeType != NodeType::Program)
        symbol->setFlags(SymbolFlagMember, true);
    scope->addSymbol(symbol);
    switch(nodeType)
    {
        case NodeType::Class:
        case NodeType::Struct:
        case NodeType::Protocol:
        case NodeType::Extension:
        case NodeType::Enum:
            assert(currentType != nullptr);
            static_pointer_cast<TypeBuilder>(currentType)->addMember(symbol->getName(), symbol);
            break;
        default:
            break;
    }
}

void SemanticAnalyzer::visitValueBinding(const ValueBindingPtr& node)
{
    if(node->getOwner()->isReadOnly() && !node->getInitializer())
    {
        error(node, Errors::E_LET_REQUIRES_INITIALIZER);
        return;
    }


    //TypePtr type = evaluateType(node->initializer);
    if(IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(node->getName()))
    {
        TypePtr declaredType = node->getType() ? node->getType() : lookupType(node->getDeclaredType());//node->getDeclaredType() == nullptr ? id->getDeclaredType() : node->getDeclaredType());
        StackedValueGuard<TypePtr> guard(t_hint);
        guard.set(declaredType);
        if(!declaredType && !node->getInitializer())
        {
            error(node, Errors::E_TYPE_ANNOTATION_MISSING_IN_PATTERN);
            return;
        }
        SymbolPtr sym = symbolRegistry->getCurrentScope()->lookup(id->getIdentifier());
        assert(sym != nullptr);
        SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
        assert(placeholder != nullptr);
        if(declaredType)
        {
            placeholder->setType(declaredType);
        }
        if(node->getInitializer())
        {
            node->getInitializer()->accept(this);
            TypePtr actualType = node->getInitializer()->getType();
            assert(actualType != nullptr);
            if(declaredType)
            {
                if(!Type::equals(actualType, declaredType) && !canConvertTo(node->getInitializer(), declaredType))
                {
                    error(node->getInitializer(), Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, actualType->toString(), declaredType->toString());
                    return;
                }
            }

            if(!declaredType)
                placeholder->setType(actualType);
        }
        assert(placeholder->getType() != nullptr);
    }
    else if(TuplePtr id = std::dynamic_pointer_cast<Tuple>(node->getName()))
    {
        TypeNodePtr declaredType = id->getDeclaredType();
        if(declaredType)
        {
            checkTupleDefinition(id, node->getInitializer());
        }
    }
    if(currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1);
    }
}


/**
* Need to explode a tuple variable definition into a sequence of single variable definitions
*/
void SemanticAnalyzer::explodeValueBindings(const ValueBindingsPtr& node)
{
    auto iter = node->begin();
    for(; iter != node->end(); iter++)
    {
        ValueBindingPtr var = *iter;
        TuplePtr tuple = dynamic_pointer_cast<Tuple>(var->getName());
        if(!tuple)
            continue;
        explodeValueBinding(node, iter);
    }
}
void SemanticAnalyzer::explodeValueBinding(const ValueBindingsPtr& valueBindings, const ValueBindings::Iterator& iter)
{
    ValueBindingPtr var = *iter;
    TuplePtr name = dynamic_pointer_cast<Tuple>(var->getName());
    TypePtr declaredType = var->getDeclaredType() ? lookupType(var->getDeclaredType()) : nullptr;
    TypePtr initializerType;
    if(var->getInitializer())
    {
        StackedValueGuard<TypePtr> guard(t_hint);
        guard.set(declaredType);
        var->getInitializer()->accept(this);
        initializerType = var->getInitializer()->getType();
        assert(initializerType != nullptr);
    }

    if(declaredType && initializerType)
    {
        //it has both type definition and initializer, then we need to check if the initializer expression matches the type annotation
        if(!initializerType->canAssignTo(declaredType))
        {
            error(var, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, initializerType->toString(), declaredType->toString());
            return;
        }
    }
    //expand it into tuple assignment
    NodeFactory* nodeFactory = valueBindings->getNodeFactory();
    //need a temporay variable to hold the initializer
    std::wstring tempName = L"#0";
    ValueBindingPtr tempVar = nodeFactory->createValueBinding(*var->getSourceInfo());
    IdentifierPtr tempVarId = nodeFactory->createIdentifier(*var->getSourceInfo());
    tempVarId->setIdentifier(tempName);
    tempVar->setName(tempVarId);
    tempVar->setInitializer(var->getInitializer());
    tempVar->setType(declaredType ? declaredType : initializerType);
    tempVar->setTemporary(true);
    valueBindings->insertAfter(tempVar, iter);
    //now expand tuples
    vector<TupleExtractionResult> result;
    vector<int> indices;
    expandTuple(result, indices, name, tempName, tempVar->getType(), valueBindings->isReadOnly() ? AccessibilityConstant : AccessibilityVariable);
    for(auto v : result)
    {
        if(v.name->getIdentifier() == L"_")
            continue;//ignore the placeholder
        ValueBindingPtr var = nodeFactory->createValueBinding(*v.name->getSourceInfo());
        var->setName(v.name);
        var->setType(v.type);
        var->setInitializer(v.initializer);
        valueBindings->add(var);
    }
}

MemberAccessPtr SemanticAnalyzer::makeAccess(SourceInfo* info, NodeFactory* nodeFactory, const std::wstring& tempName, const std::vector<int>& indices)
{
    assert(!indices.empty());
    IdentifierPtr self = nodeFactory->createIdentifier(*info);
    self->setIdentifier(tempName);
    ExpressionPtr ret = self;
    for(int i : indices)
    {
        MemberAccessPtr next = nodeFactory->createMemberAccess(*info);
        next->setSelf(ret);
        next->setIndex(i);
        ret = next;
    }
    return static_pointer_cast<MemberAccess>(ret);
}
void SemanticAnalyzer::expandTuple(vector<TupleExtractionResult>& results, vector<int>& indices, const PatternPtr& name, const std::wstring& tempName, const TypePtr& type, PatternAccessibility accessibility)
{
    TypeNodePtr declaredType;
    switch (name->getNodeType())
    {
        case NodeType::Identifier:
        {
            IdentifierPtr id = static_pointer_cast<Identifier>(name);
            if(accessibility == AccessibilityUndefined)
            {
                //undefined variable
                error(name, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
                abort();
                return;
            }
            name->setType(type);
            bool readonly = accessibility == AccessibilityConstant;
            results.push_back(TupleExtractionResult(id, type, makeAccess(id->getSourceInfo(), id->getNodeFactory(), tempName, indices), readonly));
            //check if the identifier already has a type definition
            break;
        }
        case NodeType::TypedPattern:
        {
            TypedPatternPtr pat = static_pointer_cast<TypedPattern>(name);
            assert(pat->getDeclaredType());
            TypePtr declaredType = lookupType(pat->getDeclaredType());
            pat->setType(declaredType);
            if(!Type::equals(declaredType, type))
            {
                error(name, Errors::E_TYPE_ANNOTATION_DOES_NOT_MATCH_CONTEXTUAL_TYPE_A_1, type->toString());
                abort();
                return;
            }
            expandTuple(results, indices, pat->getPattern(), tempName, declaredType, accessibility);
            break;
        }
        case NodeType::Tuple:
        {
            if(type->getCategory() != Type::Tuple)
            {
                error(name, Errors::E_TUPLE_PATTERN_CANNOT_MATCH_VALUES_OF_THE_NON_TUPLE_TYPE_A_1, type->toString());
                return;
            }
            TuplePtr tuple = static_pointer_cast<Tuple>(name);
            declaredType = tuple->getDeclaredType();
            if((type->getCategory() != Type::Tuple) || (tuple->numElements() != type->numElementTypes()))
            {
                error(name, Errors::E_TYPE_ANNOTATION_DOES_NOT_MATCH_CONTEXTUAL_TYPE_A_1, type->toString());
                abort();
                return;
            }
            //check each elements
            int elements = tuple->numElements();
            for(int i = 0; i < elements; i++)
            {
                PatternPtr element = tuple->getElement(i);
                TypePtr elementType = type->getElementType(i);
                indices.push_back(i);
                //validateTupleType(isReadonly, element, elementType);
                expandTuple(results, indices, element, tempName, elementType, accessibility);
                indices.pop_back();
            }
            break;
        }
        case NodeType::ValueBindingPattern:
        {
            if(accessibility != AccessibilityUndefined)
            {
                error(name, Errors::E_VARLET_CANNOT_APPEAR_INSIDE_ANOTHER_VAR_OR_LET_PATTERN_1, accessibility == AccessibilityConstant ? L"let" : L"var");
                abort();
            }
            else
            {
                ValueBindingPatternPtr p = static_pointer_cast<ValueBindingPattern>(name);
                expandTuple(results, indices, p->getBinding(), tempName, type, p->isReadOnly() ? AccessibilityConstant : AccessibilityVariable);
            }
            break;
        }
        case NodeType::EnumCasePattern:
        {
            EnumCasePatternPtr p(static_pointer_cast<EnumCasePattern>(name));
            if(p->getAssociatedBinding())
            {
                expandTuple(results, indices, p->getAssociatedBinding(), tempName, type, accessibility);
            }
            break;
        }
        case NodeType::TypeCase:
        case NodeType::TypeCheck:
        default:
            error(name, Errors::E_EXPECT_TUPLE_OR_IDENTIFIER);
            break;
    }
}


void SemanticAnalyzer::visitValueBindings(const ValueBindingsPtr& node)
{
    explodeValueBindings(node);


    if(node->isReadOnly() && currentType && currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1);
        return;
    }

    for(const ValueBindingPtr& var : *node)
    {
        PatternPtr name = var->getName();
        if(name->getNodeType() != NodeType::Identifier)
            continue;//The tuple has been exploded into a sequence of single variable bindings, no need to handle tuple again
        IdentifierPtr id = std::static_pointer_cast<Identifier>(name);
        SymbolPtr s = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(s)
        {
            //already defined
            error(id, Errors::E_DEFINITION_CONFLICT, id->getIdentifier());
        }
        else
        {
            SymbolPlaceHolderPtr pattern(new SymbolPlaceHolder(id->getIdentifier(), id->getType(), SymbolPlaceHolder::R_LOCAL_VARIABLE, 0));
            registerSymbol(pattern);
        }
    }
    int flags = SymbolFlagInitializing | SymbolFlagReadable;
    if(dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner()))
        flags |= SymbolFlagMember;
    if(!node->isReadOnly())
        flags |= SymbolFlagWritable;
    for(const ValueBindingPtr& v : *node)
    {
        PatternPtr name = v->getName();
        if(name->getNodeType() != NodeType::Identifier)
            continue;
        //skip placeholder
        IdentifierPtr id = std::static_pointer_cast<Identifier>(name);
        SymbolPtr s = symbolRegistry->lookupSymbol(id->getIdentifier());
        assert(s != nullptr);
        ExpressionPtr initializer = v->getInitializer();
        SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(s);
        assert(placeholder != nullptr);
        placeholder->setFlags((SymbolFlags)flags, true);
        if(initializer)
            placeholder->setFlags(SymbolFlagHasInitializer, true);
        if(v->isTemporary())
            placeholder->setFlags(SymbolFlagTemporary, true);
        v->accept(this);

        placeholder->setFlags(SymbolFlagInitialized, true);
        placeholder->setFlags(SymbolFlagInitializing, false);
    }
}

void SemanticAnalyzer::visitIdentifier(const IdentifierPtr& id)
{
    SymbolPtr sym = NULL;
    SymbolScope* scope = NULL;
    symbolRegistry->lookupSymbol(id->getIdentifier(), &scope, &sym);
    if(!sym)
    {
        error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
        return;
    }
    if(SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
    {
        if(placeholder->hasFlags(SymbolFlagInitializing))
        {
            error(id, Errors::E_USE_OF_INITIALIZING_VARIABLE, placeholder->getName());
        }
        else if(!placeholder->hasFlags(SymbolFlagInitialized))
        {
            error(id, Errors::E_USE_OF_UNINITIALIZED_VARIABLE_1, placeholder->getName());
        }
        //check if this identifier is accessed inside a class/protocol/extension/struct/enum but defined not in program
        if(dynamic_cast<TypeDeclaration*>(symbolRegistry->getCurrentScope()->getOwner()) && symbolRegistry->getCurrentScope() != scope)
        {
            if(scope->getOwner()->getNodeType() != NodeType::Program)
            {
                error(id, Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, placeholder->getName());
            }
        }
        id->setType(sym->getType());
    }
    else if(TypePtr type = dynamic_pointer_cast<Type>(sym))
    {
        TypePtr ref = Type::newTypeReference(type);
        id->setType(ref);
    }
    else if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        id->setType(func->getType());
    }
    else if(FunctionOverloadedSymbolPtr func = dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
    {
        //TODO: check contextual type hint
        //error(id, Errors::E_AMBIGUOUS_USE_1, id->getIdentifier());
    }
}