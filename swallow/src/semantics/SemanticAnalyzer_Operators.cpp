/* SemanticAnalyzer_Operators.cpp --
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

USE_SWALLOW_NS
using namespace std;




void SemanticAnalyzer::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

}
void SemanticAnalyzer::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    declareImmediately(node->getOperator());
    //look for binary function that matches
    OperatorInfo* op = symbolRegistry->getOperator(node->getOperator(), OperatorType::InfixBinary);
    std::vector<SymbolPtr> funcs = allFunctions(node->getOperator(), 0, true);
    SymbolPtr sym = symbolRegistry->lookupSymbol(node->getOperator());
    if(!op)
    {
        error(node, Errors::E_UNKNOWN_BINARY_OPERATOR_1, node->getOperator());
        return;
    }
    if((op->type & OperatorType::InfixBinary) == 0)
    {
        error(node, Errors::E_IS_NOT_BINARY_OPERATOR_1, node->getOperator());
        error(node, Errors::E_UNKNOWN_BINARY_OPERATOR_1, node->getOperator());
        return;
    }
    if(funcs.empty())
    {
        error(node, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, node->getOperator());
        return;
    }
    ExpressionPtr lhs = dynamic_pointer_cast<Expression>(node->getLHS());
    ExpressionPtr rhs = dynamic_pointer_cast<Expression>(node->getRHS());
    assert(lhs != nullptr);
    assert(rhs != nullptr);
    ParenthesizedExpressionPtr args(node->getNodeFactory()->createParenthesizedExpression(*node->getSourceInfo()));
    args->append(lhs);
    args->append(rhs);
    visitFunctionCall(false, funcs, args, node);
    //find for overload
}
void SemanticAnalyzer::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    declareImmediately(node->getOperator());
    int mask = 0;
    if(node->getOperatorType() == OperatorType::PostfixUnary)
        mask = SymbolFlagPostfix;
    else if(node->getOperatorType() == OperatorType::PrefixUnary)
        mask = SymbolFlagPrefix;
    else
        assert(0 && "Invalid operator type for unary operator");

    std::vector<SymbolPtr> funcs = allFunctions(node->getOperator(), mask, true);
    if(funcs.empty())
    {
        error(node, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, node->getOperator());
        return;
    }
    ParenthesizedExpressionPtr args(node->getNodeFactory()->createParenthesizedExpression(*node->getSourceInfo()));
    args->append(node->getOperand());
    visitFunctionCall(false, funcs, args, node);
}




/*!
 * Return true if one of the component node is read only
 */
bool SemanticAnalyzer::containsReadonlyNode(const ExpressionPtr& node)
{
    NodeType::T nodeType = node->getNodeType();
    if(nodeType == NodeType::Identifier)
    {
        IdentifierPtr id = static_pointer_cast<Identifier>(node);
        SymbolPtr sym = symbolRegistry->lookupSymbol(id->getIdentifier());
        //Check if the identifier is a structure, according to official document:
        //This behavior is due to structures being value types. When an instance of a value type is marked as a constant, so are all of its properties.
        //The same is not true for classes, which are reference types. If you assign an instance of a reference type to a constant, you can still change that instance’s variable properties.
        if(sym && !sym->hasFlags(SymbolFlagWritable) && sym->getType() && sym->getType()->getCategory() == Type::Struct)
            return true;
    }
    else if(nodeType == NodeType::MemberAccess)
    {
        MemberAccessPtr ma = static_pointer_cast<MemberAccess>(node);
        if(containsReadonlyNode(ma->getSelf()))
            return true;
        TypePtr selfType = ma->getSelf()->getType();
        if(ma->getField() && selfType)
        {
            SymbolPtr sym = selfType->getMember(ma->getField()->getIdentifier());
            //a constant member
            if(sym && !sym->hasFlags(SymbolFlagWritable))
                return true;
        }
    }
    //TODO: check non-mutating function call on struct/enum variable
    return false;
}


void SemanticAnalyzer::verifyTuplePatternForAssignment(const PatternPtr& pattern)
{

    if(pattern->getNodeType() == NodeType::Identifier)
    {
        IdentifierPtr id = std::static_pointer_cast<Identifier>(pattern);
        TypePtr type = symbolRegistry->lookupType(id->getIdentifier());
        if(type)
        {
            //cannot assign expression to type
            error(id, Errors::E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION, id->getIdentifier());
            return;
        }
        SymbolPtr sym = symbolRegistry->lookupSymbol(id->getIdentifier());
        if(!sym)
        {
            error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
        }
        if(!sym->hasFlags(SymbolFlagWritable))
        {
            //write on a constant variable
            if(id->getIdentifier() == L"self")
            {
                error(id, Errors::E_CANNOT_ASSIGN_TO_A_IN_A_METHOD_1, id->getIdentifier());
                return;
            }
            else
            {
                //do not emit this error if and only if the target is a member variable and inside an initializer
                bool init = ctx.currentFunction && ctx.currentFunction->hasFlags(SymbolFlagInit);
                bool storedProperty = sym->hasFlags(SymbolFlagStoredProperty);
                if(!(init && storedProperty))
                {
                    error(id, Errors::E_CANNOT_ASSIGN_TO_LET_VALUE_A_1, id->getIdentifier());
                    return;
                }
            }
        }
        if(sym)
        {
            sym->setFlags(SymbolFlagInitialized, true);
        }
    }
    else if(pattern->getNodeType() == NodeType::Tuple)
    {
        TuplePtr tuple = std::static_pointer_cast<Tuple>(pattern);
        for(const PatternPtr& element : *tuple)
        {
            verifyTuplePatternForAssignment(element);
        }
    }
    else
    {
        error(pattern, Errors::E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION);
        return;
    }

}

void SemanticAnalyzer::visitAssignment(const AssignmentPtr& node)
{
    declareImmediately(node->getOperator());
    node->setType(symbolRegistry->getGlobalScope()->Void());
    PatternPtr destination = node->getLHS();
    {
        SCOPED_SET(ctx.flags, ctx.flags | SemanticContext::FLAG_WRITE_CONTEXT);
        if (ExpressionPtr expr = std::dynamic_pointer_cast<Expression>(destination))
        {
            expr = transformExpression(nullptr, expr);
            node->setLHS(expr);
            destination = expr;
        }
        else
        {
            destination->accept(this);
        }
    }
    switch(destination->getNodeType())
    {
        case NodeType::Identifier:
        case NodeType::Tuple:
        {
            //check if the symbol is writable
            verifyTuplePatternForAssignment(destination);
            break;
        }
        case NodeType::SubscriptAccess:
        {
            //TODO: check if this subscript access is writable
            SubscriptAccessPtr subscriptAccess = static_pointer_cast<SubscriptAccess>(destination);
            TypePtr selfType = subscriptAccess->getSelf()->getType();
            SymbolPtr sym = selfType->getMember(L"subscript");
            if (!sym)
            {
                error(subscriptAccess, Errors::E_DOES_NOT_HAVE_A_MEMBER_2, selfType->toString(), L"subscript");
                return;
            }
            bool hasSetter = false;
            FunctionOverloadedSymbolPtr subscripts = dynamic_pointer_cast<FunctionOverloadedSymbol>(sym);
            assert(subscripts != nullptr);
            for(const FunctionSymbolPtr& func : *subscripts)
            {
                TypePtr type = func->getType();
                if(type->getParameters().size() == 2 && func->getReturnType() == symbolRegistry->getGlobalScope()->Void())
                {
                    hasSetter = true;
                    break;
                }
            }
            if (!hasSetter)
            {
                error(subscriptAccess, Errors::E_SUBSCRIPT_ACCESS_ON_A_IS_NOT_WRITABLE_1, toString(subscriptAccess->getSelf()));
                return;
            }
            break;
        }
        case NodeType::MemberAccess:
        {
            MemberAccessPtr ma = static_pointer_cast<MemberAccess>(node->getLHS());
            if(ma->getSelf()->getNodeType() == NodeType::Identifier)
            {
                //simple format to check, to give better compilation result
                IdentifierPtr self = static_pointer_cast<Identifier>(ma->getSelf());
                SymbolPtr selfSymbol = symbolRegistry->lookupSymbol(self->getIdentifier());
                assert(selfSymbol != nullptr);
                wstring index = ma->getField() ? ma->getField()->getIdentifier() : toString(ma->getIndex());
                //the members will be read only if the struct instance is marked by 'let'
                TypePtr selfType;
                bool staticAccess = false;
                if(TypePtr t = dynamic_pointer_cast<Type>(selfSymbol))
                {
                    //Type access
                    selfType = t;
                    staticAccess = true;
                }
                else
                {
                    //variable access
                    selfType = selfSymbol->getType();
                    assert(selfType != nullptr);
                }
                bool isSelf = self->getIdentifier() == L"self";
                SymbolPtr member = nullptr;
                TypePtr memberDeclaringType = nullptr;//which type declared this member
                if(ma->getField())
                {

                    member = getMemberFromType(selfType, ma->getField()->getIdentifier(), (MemberFilter) ((staticAccess ? FilterStaticMember : 0) | FilterLookupInExtension | FilterRecursive), &memberDeclaringType);
                }

                if(ma->getField() && !isSelf)
                {
                    assert(member != nullptr);
                    if(!member->hasFlags(SymbolFlagWritable))
                    {
                        error(ma->getField(), Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2, index, self->getIdentifier());
                        return;
                    }
                }
                bool supressError = false;
                if(isSelf && member && ctx.currentFunction && ctx.currentFunction->hasFlags(SymbolFlagInit) && member->hasFlags(SymbolFlagStoredProperty))
                {
                    //check if the member is defined in current type and not the super type
                    supressError = true;//it's allowed to modify 'let' variable inside an initializer
                }
                //Write on a field of a non-mutating symbol
                bool insideInit = ctx.currentFunction && ctx.currentFunction->hasFlags(SymbolFlagInit);
                if(selfSymbol->hasFlags(SymbolFlagNonmutating) && !staticAccess)
                {
                    //assigning on constant variable
                    if (selfType->getCategory() == Type::Struct)
                    {
                        if (!isSelf || !insideInit)
                        {
                            error(self, Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2, index, self->getIdentifier());
                            return;
                        }
                    }
                }
                if(isSelf && insideInit)
                {
                    //assigning on constant member inside an init function
                    bool constantMember = member && !member->hasFlags(SymbolFlagWritable);
                    if(constantMember)
                    {
                        if (!Type::equals(memberDeclaringType, ctx.currentType))
                        {
                            //changing constant member that belongs to a super class is not allowed
                            error(self, Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2, index, self->getIdentifier());
                            return;
                        }
                    }
                    //mark it initialized
                    if(member)
                    {
                        member->setFlags(SymbolFlagInitialized, true);
                    }
                }
            }
            else
            {
                if(containsReadonlyNode(ma))
                {
                    error(ma, Errors::E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION);
                    return;
                }
            }
            break;
        }
        default:
            assert(0);
            break;
    }

    //value(s) assignment
    TypePtr destinationType = destination->getType();
    assert(destinationType != nullptr);
    if(ExpressionPtr expr = dynamic_pointer_cast<Expression>(node->getRHS()))
    {
        node->setRHS(this->transformExpression(destinationType, expr));
    }
    else
    {

        SCOPED_SET(ctx.contextualType, destinationType);
        node->getRHS()->accept(this);
    }
    TypePtr sourceType = node->getRHS()->getType();
    assert(sourceType != nullptr);
    if(!sourceType->canAssignTo(destinationType))
    {
        error(node, Errors::E_A_IS_NOT_CONVERTIBLE_TO_B_2, sourceType->toString(), destinationType->toString());
        return;
    }

}
