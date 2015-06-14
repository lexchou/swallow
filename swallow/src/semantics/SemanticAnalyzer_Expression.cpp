/* SemanticAnalyzer_Expression.cpp --
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
#include "semantics/SemanticAnalyzer.h"
#include "semantics/GlobalScope.h"
#include "semantics/SymbolRegistry.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/TypeBuilder.h"
#include <cassert>
#include <algorithm>
#include "ast/NodeFactory.h"
#include "semantics/CollectionTypeAnalyzer.h"
#include <iostream>
#include "common/ScopedValue.h"
#include "semantics/InitializationTracer.h"

USE_SWALLOW_NS
using namespace std;



void SemanticAnalyzer::visitForcedValue(const ForcedValuePtr& node)
{
    node->getExpression()->accept(this);
    TypePtr type = node->getExpression()->getType();
    if(!symbolRegistry->getGlobalScope()->isOptional(type))
    {
        error(node, Errors::E_OPERAND_OF_POSTFIX_A_SHOULD_HAVE_OPTIONAL_TYPE_TYPE_IS_B_2, L"!", type->toString());
        return;
    }
    node->setType(type->getGenericArguments()->get(0));
}
void SemanticAnalyzer::visitOptionalChaining(const OptionalChainingPtr& node)
{

    node->getExpression()->accept(this);
    TypePtr type = node->getExpression()->getType();
    GlobalScope* global = symbolRegistry->getGlobalScope();
    if(!global->isOptional(type))
    {
        error(node, Errors::E_OPERAND_OF_POSTFIX_A_SHOULD_HAVE_OPTIONAL_TYPE_TYPE_IS_B_2, L"?", type->toString());
        return;
    }
    type = type->getGenericArguments()->get(0);
    node->setType(type);
}
bool SemanticAnalyzer::hasOptionalChaining(const NodePtr& node)
{
    if(!node)
        return false;
    NodeType::T nodeType = node->getNodeType();
    if(nodeType == NodeType::OptionalChaining)
        return true;
    if(nodeType == NodeType::MemberAccess)
        return hasOptionalChaining(static_pointer_cast<MemberAccess>(node)->getSelf());
    if(nodeType == NodeType::SubscriptAccess)
        return hasOptionalChaining(static_pointer_cast<SubscriptAccess>(node)->getSelf());
    return false;
}


bool SemanticAnalyzer::isParentInOptionalChain(const NodePtr& node)
{
    if(node == nullptr)
        return false;
    switch(node->getNodeType())
    {
        case NodeType::MemberAccess:
        case NodeType::Assignment:
        case NodeType::OptionalChaining:
        case NodeType::FunctionCall:
            return true;
        default:
            return false;
    }
    return false;
}

void SemanticAnalyzer::visitMemberAccess(const MemberAccessPtr& node)
{
    TypePtr selfType = ctx.contextualType;
    wstring fieldName = node->getField() ? node->getField()->getIdentifier() : toString(node->getIndex());
    bool staticAccess = false;
    if(node->getSelf())
    {
        validateInitializerDelegation(node);
        SCOPED_SET(ctx.flags, ctx.flags & ~SemanticContext::FLAG_WRITE_CONTEXT);
        node->getSelf()->accept(this);
        selfType = node->getSelf()->getType();
        assert(selfType != nullptr);

        if (selfType->getCategory() == Type::MetaType)
        {
            selfType = selfType->getInnerType();
            staticAccess = true;
        }
    }
    else //Type is implicitly retrieved from contextual type
    {
        selfType = ctx.contextualType;
        if(!ctx.contextualType)
        {
            //invalid contextual type
            error(node, Errors::E_NO_CONTEXTUAL_TYPE_TO_ACCESS_MEMBER_A_1, fieldName);
            return;
        }
        staticAccess = true;
    }

    if(node->getField())
    {
        SymbolPtr member = getMemberFromType(selfType, fieldName, (MemberFilter)((staticAccess ? FilterStaticMember : 0) | FilterLookupInExtension | FilterRecursive));

        if (member == nullptr)
        {
            //diagnose why it's not found
            if(staticAccess && selfType->getCategory() == Type::Enum && selfType->getEnumCase(fieldName))
            {
                const EnumCase* ec = selfType->getEnumCase(fieldName);
                if(ec && !ec->type->isNil())
                {
                    //it's a enum's case access, a case with associated data is not allowed to be accessed directly

                    error(node, Errors::E_PARTIAL_APPLICATION_OF_ENUM_CONSTRUCTOR_IS_NOT_ALLOWED);
                    abort();
                    return;
                }
                node->setType(selfType);
                return;
            }
            //if it's a int litertal, we can try to look it up in double type
            //this is not defined in official's document, but infered from official's example
            GlobalScope* global = symbolRegistry->getGlobalScope();
            if(node->getSelf() && node->getSelf()->getNodeType() == NodeType::IntegerLiteral && selfType == global->Int() && !staticAccess)
            {
                member = getMemberFromType(global->Double(), fieldName, (MemberFilter)((staticAccess ? FilterStaticMember : 0) | (FilterLookupInExtension | FilterRecursive)));
                if(member)
                {
                    //now we're sure it's Double, make it double
                    selfType = global->Double();
                    node->getSelf()->setType(selfType);
                }
            }
            if(!member)
            {
                error(node->getField(), Errors::E_DOES_NOT_HAVE_A_MEMBER_2, selfType->toString(), fieldName);
                return;
            }
        }
        node->setType(member->getType());
    }
    else
    {
        //so it must be a tuple type
        if(selfType->getCategory() != Type::Tuple)
        {
            error(node, Errors::E_TUPLE_ACCESS_ONLY_WORKS_FOR_TUPLE_TYPE);
            return;
        }
        int index = node->getIndex();
        if(index < 0 || index >= selfType->numElementTypes())
        {
            error(node, Errors::E_TUPLE_ACCESS_A_OUT_OF_RANGE_IN_B_2, toString(index), toString(node->getSelf()));
            return;
        }
        node->setType(selfType->getElementType(index));
    }

    //Optional Chaining, if parent node is not a member access and there's a optional chaining expression inside Self, mark the expression optional
    //if(!parentNode || (parentNode->getNodeType() != NodeType::MemberAccess && parentNode->getNodeType() != NodeType::Assignment && parentNode->getNodeType() != NodeType::OptionalChaining))
    if(!isParentInOptionalChain(node->getParentNode()))
    {
        if(hasOptionalChaining(node->getSelf()))
        {
            TypePtr type = symbolRegistry->getGlobalScope()->makeOptional(node->getType());
            node->setType(type);
        }
    }
}


TypePtr SemanticAnalyzer::evaluateType(const ExpressionPtr& expr)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return symbolRegistry->getGlobalScope()->Int();
        case NodeType::FloatLiteral:
            return symbolRegistry->getGlobalScope()->Double();
        case NodeType::StringLiteral:
            return symbolRegistry->getGlobalScope()->String();
        default:
            return nullptr;
    }
}
void SemanticAnalyzer::visitSubscriptAccess(const SubscriptAccessPtr& node)
{
    node->getSelf()->accept(this);
    TypePtr selfType = node->getSelf()->getType();
    assert(selfType != nullptr);
    vector<SymbolPtr> funcs;
    this->getMethodsFromType(selfType, L"subscript",  (MemberFilter)(FilterLookupInExtension | FilterRecursive), funcs);
    if(funcs.empty())
    {
        //undefined subscript access
        wstring s = this->toString(node->getSelf());
        error(node, Errors::E_UNDEFINED_SUBSCRIPT_ACCESS_FOR_1, s);
        return;
    }
    bool mutatingSelf = false;//TODO update this variable
    //Now inference the type returned by this subscript access
    SymbolPtr func = getOverloadedFunction(mutatingSelf, node, funcs, node->getIndex());
    assert(func && func->getType() && func->getType()->getCategory() == Type::Function);
    node->setType(func->getType()->getReturnType());
}


void SemanticAnalyzer::visitValueBindingPattern(const ValueBindingPatternPtr& node)
{
    assert(ctx.contextualType != nullptr);
    node->setType(ctx.contextualType);
}
