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
#include "SemanticAnalyzer.h"
#include "GlobalScope.h"
#include "SymbolRegistry.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "ast/NodeSerializer.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "TypeBuilder.h"
#include "FunctionIterator.h"
#include <cassert>
#include <algorithm>
#include "ast/NodeFactory.h"
#include "CollectionTypeAnalyzer.h"
#include <iostream>
#include "common/ScopedValue.h"

USE_SWALLOW_NS
using namespace std;
static bool hasOptionalChaining(const NodePtr& node);
static bool isParentInOptionalChain(const NodePtr& node);


bool SemanticAnalyzer::isInteger(const TypePtr& type)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    return type->conformTo(scope->_IntegerType());
}
bool SemanticAnalyzer::isNumber(const TypePtr& type)
{
    if(isFloat(type))
        return true;
    if(isInteger(type))
        return true;
    return false;
}
bool SemanticAnalyzer::isFloat(const TypePtr& type)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    return type->conformTo(scope->FloatingPointType());
}

void SemanticAnalyzer::checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer)
{
    //this is a tuple definition, the corresponding declared type must be a tuple type
    TypeNodePtr declaredType = tuple->getDeclaredType();
    TypePtr type = lookupType(declaredType);
    if(!type)
    {
        std::wstringstream out;
        NodeSerializerW serializer(out);
        declaredType->accept(&serializer);
        error(tuple, Errors::E_USE_OF_UNDECLARED_TYPE_1, out.str());
        return;
    }
    if(!(type->getCategory() == Type::Tuple))
    {
        //tuple definition must have a tuple type definition
        std::wstringstream out;
        NodeSerializerW serializer(out);
        declaredType->accept(&serializer);
        error(tuple, Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1, out.str());
        return;
    }
    if(tuple->numElements() != type->numElementTypes())
    {
        //tuple pattern has the wrong length for tuple type '%'
        std::wstringstream out;
        NodeSerializerW serializer(out);
        declaredType->accept(&serializer);
        error(tuple, Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1, out.str());
        return;
    }
    //check if initializer has the same type with the declared type
    if(initializer)
    {
        TypePtr valueType = evaluateType(initializer);
        if(valueType && !Type::equals(valueType, type))
        {
            //tuple pattern has the wrong length for tuple type '%'
            std::wstringstream out;
            NodeSerializerW serializer(out);
            declaredType->accept(&serializer);
            //tuple types '%0' and '%1' have a different number of elements (%2 vs. %3)
            wstring expectedType = type->toString();
            wstring got = toString(valueType->numElementTypes());
            wstring expected = toString(type->numElementTypes());
            error(initializer, Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT_4, out.str(), expectedType, got, expected);
            return;
        }
    }


    for(const PatternPtr& p : *tuple)
    {
        NodeType::T nodeType = p->getNodeType();
        if(nodeType != NodeType::Identifier)
        {

        }

    }
}


TypePtr SemanticAnalyzer::getExpressionType(const ExpressionPtr& expr, const TypePtr& hint, float& score)
{
    if(expr->getType() == nullptr)
        expr->accept(this);
    score = 0.9;
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(expr->getNodeType() == NodeType::IntegerLiteral && hint != nullptr)
    {
        IntegerLiteralPtr literal = std::static_pointer_cast<IntegerLiteral>(expr);
        if(hint->conformTo(scope->FloatingPointType()))
        {
            score  = 0.5;
            return hint;
        }
        if(hint->conformTo(scope->_IntegerType()))
        {
            if(hint == scope->Int())
                score = 1;
            return hint;
        }

    }
    if(expr->getNodeType() == NodeType::FloatLiteral && hint != nullptr)
    {
        FloatLiteralPtr literal = std::static_pointer_cast<FloatLiteral>(expr);
        if(hint == scope->Double())
        {
            score = 1;
            return hint;
        }
        if(isFloat(hint))
            return hint;
    }
    TypePtr ret = expr->getType();
    assert(ret != nullptr);
    return ret;
}
/*!
 *
 * \param parameter
 * \param argument
 * \param variadic  Variadic parameter must have no name
 * \param score
 * \param supressErrors
*/
bool SemanticAnalyzer::checkArgument(const TypePtr& funcType, const Type::Parameter& parameter, const std::pair<std::wstring, ExpressionPtr>& argument, bool variadic, float& score, bool supressErrors, map<wstring, TypePtr>& genericTypes)
{

    const std::wstring& name = argument.first;
    float s = 1;
    TypePtr argType = getExpressionType(argument.second, parameter.type, s);

    if(variadic)
    {
        //variadic parameter must have no name
        if(!name.empty())
        {
            error(argument.second, Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1, name);
            abort();
        }
    }
    else
    {
        if (parameter.name != name)
        {
            if (!supressErrors)
            {
                if (name.empty() && !parameter.name.empty())
                    error(argument.second, Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL_1, parameter.name);
                else if(!name.empty() && parameter.name.empty())
                    error(argument.second, Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1, name);
                else
                    error(argument.second, Errors::E_INCORRECT_ARGUMENT_LABEL_IN_CALL_HAVE_A_EXPECTED_B_2, name, parameter.name);
                abort();
            }
            return false;
        }
    }
    if(!argType->canAssignTo(parameter.type))
    {
        //cannot assign to, check if it's a generic type and can be specialized to
        bool canSpecialized = false;
        if(funcType->getGenericDefinition())
        {
            if(parameter.type->canSpecializeTo(argType, genericTypes))
            {
                canSpecialized = true;
            }
        }
        if (!canSpecialized)
        {
            if (!supressErrors)
            {
                error(argument.second, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, argType->toString(), parameter.type->toString());
                abort();
            }
            return false;//parameter is not matched
        }
    }
    score += s;
    return true;
}

float SemanticAnalyzer::calculateFitScore(TypePtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors)
{
    float score = 0;
    const std::vector<Type::Parameter>& parameters = func->getParameters();
    bool variadic = func->hasVariadicParameters();

    //TODO: check trailing closure


    std::vector<Type::Parameter>::const_iterator paramIter = parameters.begin();
    std::vector<ParenthesizedExpression::Term>::iterator argumentIter = arguments->begin();
    std::vector<Type::Parameter>::const_iterator paramEnd = variadic ? parameters.end() - 1 : parameters.end();
    map<wstring, TypePtr> genericTypes;
    for(;argumentIter != arguments->end() && paramIter != paramEnd; argumentIter++, paramIter++)
    {
        const Type::Parameter& parameter = *paramIter;
        ParenthesizedExpression::Term& argument = *argumentIter;
        SCOPED_SET(t_hint, parameter.type);
        argument.transformedExpression = this->transformExpression(parameter.type, argument.expression);
        bool ret = checkArgument(func, parameter, make_pair(argumentIter->name, argumentIter->transformedExpression), false, score, supressErrors, genericTypes);
        if(!ret)
            return -1;
    }
    //if there's the rest parameters, they must have initializer
    if(paramIter != paramEnd && argumentIter == arguments->end())
    {
        //if there's only one parameter and it's a variadic parameter, ignore this error
        if(!(paramIter + 1 == parameters.end() && variadic))
        {
            //TODO: check initializer
            if (!supressErrors)
            {
                error(arguments, Errors::E_UNMATCHED_PARAMETERS);
                abort();
            }
            return -1;
        }
    }
    //if there's the rest arguments, check for variadic
    if(paramIter == paramEnd && argumentIter != arguments->end())
    {
        if(!variadic)
        {
            if(!supressErrors)
            {
                error(argumentIter->expression, Errors::E_EXTRANEOUS_ARGUMENT);
                abort();
            }
            return -1;
        }
        const Type::Parameter& parameter = *paramIter;
        //the first variadic argument must have a label if the parameter got a label
        if(!parameter.name.empty())
        {
            bool ret = checkArgument(func, parameter, make_pair(argumentIter->name, argumentIter->transformedExpression), false, score, supressErrors, genericTypes);
            argumentIter++;
            if(!ret)
                return -1;
        }

        //check rest argument
        for(;argumentIter != arguments->end(); argumentIter++)
        {
            bool ret = checkArgument(func, parameter, make_pair(argumentIter->name, argumentIter->transformedExpression), true, score, supressErrors, genericTypes);
            if(!ret)
                return -1;
        }
    }

    if(func->getGenericDefinition())// && !genericTypes.empty())
    {
        GenericDefinitionPtr generic = func->getGenericDefinition();
        //the generic types are infered from previous stage while calculating the fitness score, now need to verify if they
        //satisify the generic constraints.
        //NOTE: only name and types are available, so it's impossible here to know which argument used the generic type.
        for(auto iter : genericTypes)
        {
            TypePtr argType = iter.second;
            assert(argType != nullptr);
            //check for type constraint
            TypePtr expectedType;
            bool constraintSatisfied = generic->validate(iter.first, argType, expectedType);
            if(!constraintSatisfied)
            {
                if(supressErrors)
                    return false;
                error(arguments, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, argType->getName(), expectedType->getName());
                abort();
            }
        }
        assert(generic->numParameters() == genericTypes.size());
        func = func->newSpecializedType(func, genericTypes);
    }

    if(!arguments->numExpressions())
        return 1;
    return score / arguments->numExpressions();
}

SymbolPtr SemanticAnalyzer::getOverloadedFunction(const NodePtr& node, const std::vector<SymbolPtr>& funcs, const ParenthesizedExpressionPtr& arguments)
{
    typedef std::tuple<float, SymbolPtr, TypePtr> ScoredFunction;
    std::vector<ScoredFunction> candidates;
    for(const SymbolPtr& func : funcs)
    {
        TypePtr type = func->getType();
        assert(type->getCategory() == Type::Function);
        float score = calculateFitScore(type, arguments, true);
        if(score > 0)
            candidates.push_back(std::make_tuple(score, func, type));
    }
    if(candidates.empty())
    {
        error(node, Errors::E_NO_MATCHED_OVERLOAD_FOR_A_1, funcs[0]->getName());
        abort();
        return nullptr;
    }
    //sort by fit score
    if(candidates.size() > 1)
    {
        sort(candidates.begin(), candidates.end(), [](const ScoredFunction& lhs, const ScoredFunction& rhs ){
            return get<0>(rhs) < get<0>(lhs);
        });
        if(get<0>(candidates[0]) == get<0>(candidates[1]))
        {
            error(node, Errors::E_AMBIGUOUS_USE_1, funcs[0]->getName());
            abort();
            return nullptr;
        }
    }
    SymbolPtr matched = get<1>(candidates.front());
    return matched;
}

SymbolPtr SemanticAnalyzer::visitFunctionCall(const std::vector<SymbolPtr>& funcs, const ParenthesizedExpressionPtr& args, const PatternPtr& node)
{
    if(funcs.size() == 1)
    {
        SymbolPtr sym = funcs.front();
        if(FunctionSymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
        {
            //verify argument
            std::wstring name = func->getName();
            TypePtr type = func->getType();
            calculateFitScore(type, args, false);
            node->setType(type->getReturnType());
            return func;
        }
        else if(SymbolPlaceHolderPtr symbol = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
        {
            //it must be a function type to call
            TypePtr type = symbol->getType();
            assert(type != nullptr);
            if(type->getCategory() != Type::Function)
            {
                error(node, Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2, toString(node), type->toString());
                abort();
                return nullptr;
            }
            calculateFitScore(type, args, false);
            node->setType(type->getReturnType());
            return symbol;
        }
        else
        {
            assert(0 && "Unsupported function to call");
            return nullptr;
        }
    }
    else
    {
        SymbolPtr matched = getOverloadedFunction(node, funcs, args);
        assert(matched != nullptr && matched->getType()->getCategory() == Type::Function);
        node->setType(matched->getType()->getReturnType());
        return matched;
    }

}

void SemanticAnalyzer::visitReturn(const ReturnStatementPtr& node)
{
    if(!currentFunction)
    {
        error(node, Errors::E_RETURN_INVALID_OUTSIDE_OF_A_FUNC);
        return;
    }
    TypePtr funcType = currentFunction;

    SCOPED_SET(t_hint, funcType->getReturnType());

    float score = 0;
    TypePtr retType = this->getExpressionType(node->getExpression(), funcType->getReturnType(), score);
    TypePtr expectedType = funcType->getReturnType();
    if(!retType->canAssignTo(expectedType))
    {
        error(node->getExpression(), Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, retType->toString(), expectedType->toString());
    }
}
void SemanticAnalyzer::visitFunctionCall(const FunctionCallPtr& node)
{
    NodeType::T nodeType = node->getFunction()->getNodeType();
    ExpressionPtr func = node->getFunction();

    switch(nodeType)
    {
        case NodeType::Identifier:
        {
            func->accept(this);
            IdentifierPtr id = std::static_pointer_cast<Identifier>(func);
            const std::wstring &symbolName = id->getIdentifier();
            //TODO: add a parameter on allFunctions to allow it to select types
            vector<SymbolPtr> funcs = allFunctions(symbolName, 0, true);
            if (funcs.empty())
            {
                error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, symbolName);
                return;
            }
            //if symbol points to a type, then redirect it to a initializer
            if(funcs.size() == 1)
            {
                SymbolPtr sym = funcs[0];
                if (TypePtr type = std::dynamic_pointer_cast<Type>(sym))
                {
                    if (type->getCategory() == Type::Class || type->getCategory() == Type::Struct)
                    {
                        funcs.erase(funcs.begin());
                        getMethodsFromType(type, L"init", false, funcs);
                    }
                }
            }
            visitFunctionCall(funcs, node->getArguments(), node);
            break;
        }
        case NodeType::MemberAccess:
        {
            MemberAccessPtr ma = std::static_pointer_cast<MemberAccess>(func);
            ma->getSelf()->accept(this);
            TypePtr selfType = ma->getSelf()->getType();
            assert(selfType != nullptr);
            const wstring& identifier = ma->getField()->getIdentifier();
            SymbolPtr sym;
            const EnumCase*c = nullptr;

            if(identifier == L"init")
            {
                if(ma->getSelf()->getNodeType() == NodeType::Identifier && static_pointer_cast<Identifier>(ma->getSelf())->getIdentifier() == L"self")
                {
                    //it must be inside a init
                    if(!currentFunction || !currentFunction->hasFlags(SymbolFlagInit))
                    {
                        error(ma, Errors::E_INITIALIZER_DELEGATION_CAN_ONLY_OCCUR_WITHIN_AN_INITIALIZER);
                        return;
                    }
                }
                else
                {
                    error(ma, Errors::E_INIT_CAN_ONLY_REFER_TO_THE_INITIALIZERS_OF_SELF);
                    return;
                }
            }
            vector<SymbolPtr> funcs;
            if(selfType->getCategory() == Type::MetaType)
            {
                selfType = selfType->getInnerType();
                if(selfType->getCategory() == Type::Enum && (c = selfType->getEnumCase(identifier)) && c->constructor)
                    //constructing an enum
                    funcs.push_back(c->constructor);
                else
                    getMethodsFromType(selfType, identifier, true, funcs);
            }
            else
                getMethodsFromType(selfType, identifier, false, funcs);
            if(funcs.empty())
            {
                error(ma, Errors::E_DOES_NOT_HAVE_A_MEMBER_2, selfType->toString(), identifier);
                abort();
                return;
            }
            SymbolPtr func = visitFunctionCall(funcs, node->getArguments(), node);
            assert(func != nullptr);
            ma->setType(func->getType());
            if(hasOptionalChaining(ma->getSelf()) && !isParentInOptionalChain(parentNode))
            {
                TypePtr type = symbolRegistry->getGlobalScope()->makeOptional(node->getType());
                node->setType(type);
            }
            break;
        }
        case NodeType::Closure:
        {
            func->accept(this);
            ClosurePtr closure = std::static_pointer_cast<Closure>(func);
            TypePtr type = closure->getType();
            assert(type != nullptr && type->getCategory() == Type::Function);
            calculateFitScore(type, node->getArguments(), false);
            node->setType(type->getReturnType());
            break;
        }
        case NodeType::ArrayLiteral:
        {
            func->accept(this);
            //check if it's an array expression
            ArrayLiteralPtr array = std::static_pointer_cast<ArrayLiteral>(func);
            if(array->numElements() != 1 || (*array->begin())->getNodeType() != NodeType::Identifier)
            {
                wstring call = toString(func);
                wstring type = func->getType()->toString();
                error(func, Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2, call, type);
                break;
            }
            IdentifierPtr typeName = std::static_pointer_cast<Identifier>(*array->begin());
            SymbolPtr sym = symbolRegistry->lookupSymbol(typeName->getIdentifier());
            TypePtr type = std::dynamic_pointer_cast<Type>(sym);
            if(!type)
            {
                error(func, Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2, toString(func), func->getType()->toString());
                break;
            }
            TypePtr arrayType = symbolRegistry->getGlobalScope()->makeArray(type);
            node->setType(arrayType);
            break;
        }
        case NodeType::DictionaryLiteral:
        {
            func->accept(this);
            TypePtr initType = func->getType();
            assert(initType != nullptr);
            //check if it's an array expression
            if(initType->getCategory() != Type::MetaType)
            {
                wstring call = toString(func);
                wstring type = initType->toString();
                error(func, Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2, call, type);
                break;
            }
            TypePtr dictType = initType->getInnerType();
            node->setType(dictType);
            break;
        }
        default:
        {
            error(func, Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2, toString(func), func->getType()->toString());
            break;
        }
    }

}
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
static bool hasOptionalChaining(const NodePtr& node)
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


static bool isParentInOptionalChain(const NodePtr& node)
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
    TypePtr selfType = t_hint;
    wstring fieldName = node->getField() ? node->getField()->getIdentifier() : toString(node->getIndex());
    bool staticAccess = false;
    if(node->getSelf())
    {
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
        selfType = t_hint;
        if(!t_hint)
        {
            //invalid contextual type
            error(node, Errors::E_NO_CONTEXTUAL_TYPE_TO_ACCESS_MEMBER_A_1, fieldName);
            return;
        }
        staticAccess = true;
    }

    if(node->getField())
    {
        SymbolPtr member = getMemberFromType(selfType, fieldName, staticAccess);

        if (member == nullptr)
        {
            //diagnose why it's not found
            if(staticAccess && selfType->getCategory() == Type::Enum && selfType->getEnumCase(fieldName))
            {
                //it's a enum's case access, it's not allowed to be accessed directly
                error(node, Errors::E_PARTIAL_APPLICATION_OF_ENUM_CONSTRUCTOR_IS_NOT_ALLOWED);
                abort();
            }
            //if it's a int litertal, we can try to look it up in double type
            //this is not defined in official's document, but infered from official's example
            GlobalScope* global = symbolRegistry->getGlobalScope();
            if(node->getSelf()->getNodeType() == NodeType::IntegerLiteral && selfType == global->Int() && !staticAccess)
            {
                member = getMemberFromType(global->Double(), fieldName, staticAccess);
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
    if(!isParentInOptionalChain(parentNode))
    {
        if(hasOptionalChaining(node->getSelf()))
        {
            TypePtr type = symbolRegistry->getGlobalScope()->makeOptional(node->getType());
            node->setType(type);
        }
    }
}
void SemanticAnalyzer::visitNilLiteral(const NilLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(!t_hint || !t_hint->canAssignTo(scope->NilLiteralConvertible()))
    {
        error(node, Errors::E_EXPRESSION_DOES_NOT_CONFORM_TO_PROTOCOL_1, L"NilLiteralConvertible");
        return;
    }
    node->setType(t_hint);
}
void SemanticAnalyzer::visitBooleanLiteral(const BooleanLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->BooleanLiteralConvertible()))
        node->setType(t_hint);
    else
        node->setType(scope->Bool());
}
void SemanticAnalyzer::visitString(const StringLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->StringLiteralConvertible()))
        node->setType(t_hint);
    else if(t_hint && node->value.length() == 1 && t_hint->canAssignTo(scope->UnicodeScalarLiteralConvertible()))
        node->setType(t_hint);
    else
        node->setType(scope->String());
}
void SemanticAnalyzer::visitStringInterpolation(const StringInterpolationPtr &node)
{
    //TODO: check all expressions inside can be converted to string
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->StringInterpolationConvertible()))
        node->setType(t_hint);
    else
        node->setType(scope->String());
}
void SemanticAnalyzer::visitInteger(const IntegerLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    //TODO: it will changed to use standard library's overloaded type constructor to infer type when the facility is mature enough.
    if(t_hint && t_hint->canAssignTo(scope->IntegerLiteralConvertible()))
        node->setType(t_hint);
    else if(t_hint && t_hint->canAssignTo(scope->FloatLiteralConvertible()))
        node->setType(t_hint);
    else
        node->setType(scope->Int());
}
void SemanticAnalyzer::visitFloat(const FloatLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->FloatLiteralConvertible()))
        node->setType(t_hint);
    else
        node->setType(scope->Double());
}

//Will be replaced by stdlib's type constructor
bool SemanticAnalyzer::canConvertTo(const ExpressionPtr& expr, const TypePtr& type)
{
    GlobalScope* global = symbolRegistry->getGlobalScope();
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return type->canAssignTo(global->IntegerLiteralConvertible());
        case NodeType::FloatLiteral:
            return type->canAssignTo(global->FloatLiteralConvertible());
        case NodeType::BooleanLiteral:
            return type->canAssignTo(global->BooleanLiteralConvertible());
        case NodeType::NilLiteral:
            return type->canAssignTo(global->NilLiteralConvertible());
        case NodeType::StringLiteral:
            return type->canAssignTo(global->StringLiteralConvertible());
        default:
            return expr->getType()->canAssignTo(type);
    }
    return false;
}

void SemanticAnalyzer::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    int num = node->numElements();
    GlobalScope* global = symbolRegistry->getGlobalScope();
    if(t_hint)
    {
        //TODO if hint specified, it must be Array type nor conform to ArrayLiteralConvertible protocol
        if(t_hint->getInnerType() != global->Array())
        {
            bool conformToArrayLiteralConvertible = false;
            if(!conformToArrayLiteralConvertible)
            {
                error(node, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, t_hint->getName(), L"ArrayLiteralConvertible");
                return;
            }
        }
    }



    if(num == 0)
    {
        if(!t_hint)//cannot define an empty array without type hint.
            error(node, Errors::E_CANNOT_DEFINE_AN_EMPTY_ARRAY_WITHOUT_CONTEXTUAL_TYPE);
        else
            node->setType(t_hint);
        return;
    }

    TypePtr elementType = t_hint != nullptr ? t_hint->getGenericArguments()->get(0) : nullptr;
    CollectionTypeAnalyzer analyzer(elementType, global);
    for(const ExpressionPtr& el : *node)
    {
        SCOPED_SET(t_hint, analyzer.finalType);
        el->accept(this);
        analyzer.analyze(el);

        if(analyzer.differentTypes > 0)
        {
            error(el, Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES);
        }
        if(!canConvertTo(el, analyzer.finalType))
        {
            error(el, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(el), analyzer.finalType->toString());
        }

    }

    assert(analyzer.finalType != nullptr);
    TypePtr arrayType = global->makeArray(analyzer.finalType);
    node->setType(arrayType);
}
void SemanticAnalyzer::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    TypePtr keyHint, valueHint;
    GlobalScope* global = symbolRegistry->getGlobalScope();

    //if it only contains one entry and both key and value are type, then it's a type constructor
    if(node->numElements() == 1)
    {
        auto entry = *node->begin();
        TypeNodePtr keyTypeNode = expressionToType(entry.first);
        TypeNodePtr valueTypeNode = expressionToType(entry.second);
        TypePtr keyType = lookupType(keyTypeNode, true);
        TypePtr valueType = lookupType(valueTypeNode, true);
        if(keyType && valueType)
        {
            TypePtr dict = global->makeDictionary(keyType, valueType);
            TypePtr ref = Type::newTypeReference(dict);
            node->setType(ref);
            return;
        }
    }

    if(t_hint)
    {
        if(!t_hint->conformTo(global->DictionaryLiteralConvertible()))
        {
            error(node, Errors::E_A_IS_NOT_CONVERTIBLE_TO_B_2, t_hint->toString(), L"DictionaryLiteralConvertible");
            return;
        }
        if(global->isDictionary(t_hint))
        {
            keyHint = t_hint->getGenericArguments()->get(0);
            valueHint = t_hint->getGenericArguments()->get(1);
            if(!keyHint->conformTo(global->Hashable()))
            {
                error(node, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, keyHint->toString(), L"Hashable");
                return;
            }
        }
    }

    if(node->numElements() == 0)
    {
        if(!t_hint)//cannot define an empty array without type hint.
            error(node, Errors::E_CANNOT_DEFINE_AN_EMPTY_DICTIONARY_WITHOUT_CONTEXTUAL_TYPE);
        else
            node->setType(t_hint);
        return;
    }

    CollectionTypeAnalyzer keyAnalyzer(keyHint, global);
    CollectionTypeAnalyzer valueAnalyzer(valueHint, global);

    for(auto entry : *node)
    {
        SCOPED_SET(t_hint, keyHint);
        t_hint = keyHint;
        entry.first->accept(this);
        t_hint = valueHint;
        entry.second->accept(this);
        keyAnalyzer.analyze(entry.first);
        valueAnalyzer.analyze(entry.second);
        entry.first = this->transformExpression(keyAnalyzer.finalType, entry.first);
        entry.second = transformExpression(valueAnalyzer.finalType, entry.second);

        if(keyAnalyzer.differentTypes > 0)
            error(entry.first, Errors::E_DICTIONARY_KEY_CONTAINS_DIFFERENT_TYPES);
        if(valueAnalyzer.differentTypes > 0)
            error(entry.second, Errors::E_DICTIONARY_VALUE_CONTAINS_DIFFERENT_TYPES);
        if(!canConvertTo(entry.first, keyAnalyzer.finalType))
            error(entry.first, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(entry.first), keyAnalyzer.finalType->toString());
        if(!canConvertTo(entry.second, valueAnalyzer.finalType))
            error(entry.second, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(entry.second), valueAnalyzer.finalType->toString());
    }

    if(t_hint)
    {
        node->setType(t_hint);
    }
    else
    {
        assert(keyAnalyzer.finalType != nullptr);
        assert(valueAnalyzer.finalType != nullptr);
        TypePtr type = global->makeDictionary(keyAnalyzer.finalType, valueAnalyzer.finalType);
        node->setType(type);
    }


}
void SemanticAnalyzer::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    //NodeVisitor::visitParenthesizedExpression(node);
    TypePtr hint = t_hint;
    std::vector<TypePtr> types;
    int index = 0;
    for(ParenthesizedExpression::Term& element : *node)
    {
        TypePtr elementHint = nullptr;
        if(t_hint && t_hint->getCategory() == Type::Tuple && index < t_hint->numElementTypes())
        {
            elementHint = t_hint->getElementType(index++);
        }
        SCOPED_SET(t_hint, elementHint);
        element.expression->accept(this);
        element.expression = transformExpression(elementHint, element.expression);
        TypePtr elementType = element.expression->getType();
        assert(elementType != nullptr);
        types.push_back(elementType);
    }
    if(types.size() == 1)
    {
        node->setType(types[0]);
    }
    else
    {
        TypePtr type = Type::newTuple(types);
        node->setType(type);
    }
}
void SemanticAnalyzer::visitTuple(const TuplePtr& node)
{
    NodeVisitor::visitTuple(node);
    std::vector<TypePtr> types;
    auto iter = node->begin();
    int index = 0;
    for(; iter != node->end(); iter++)
    {
        TypePtr elementHint = nullptr;
        if(t_hint && t_hint->getCategory() == Type::Tuple && index < t_hint->numElementTypes())
        {
            elementHint = t_hint->getElementType(index++);
        }
        SCOPED_SET(t_hint, elementHint);
        PatternPtr element = *iter;
        element->accept(this);
        if(ExpressionPtr expr = dynamic_pointer_cast<Expression>(element))
        {
            expr = transformExpression(elementHint, expr);
            element = *iter = expr;
        }
        TypePtr elementType = element->getType();
        assert(elementType != nullptr);
        types.push_back(elementType);
    }
    TypePtr type = Type::newTuple(types);
    node->setType(type);
}


void SemanticAnalyzer::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

}
void SemanticAnalyzer::visitBinaryOperator(const BinaryOperatorPtr& node)
{
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
    visitFunctionCall(funcs, args, node);
    //find for overload
}
void SemanticAnalyzer::visitUnaryOperator(const UnaryOperatorPtr& node)
{
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
    visitFunctionCall(funcs, args, node);
}


void SemanticAnalyzer::visitCompileConstant(const CompileConstantPtr& node)
{
    const std::wstring& name = node->getName();
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(name == L"__LINE__" || name == L"__COLUMN__")
    {
        node->setType(scope->Int());
    }
    else if(name == L"__FUNCTION__" || name == L"__FILE__")
    {
        node->setType(scope->String());
    }
    else
    {
        assert(0 && "Invalid compile constant");
        abort();
    }
}




TypePtr SemanticAnalyzer::evaluateType(const ExpressionPtr& expr)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return symbolRegistry->lookupType(L"Int");
        case NodeType::FloatLiteral:
            return symbolRegistry->lookupType(L"Float");
        case NodeType::StringLiteral:
            return symbolRegistry->lookupType(L"String");
        default:
            return nullptr;
    }
}
void SemanticAnalyzer::visitSubscriptAccess(const SubscriptAccessPtr& node)
{
    node->getSelf()->accept(this);
    TypePtr selfType = node->getSelf()->getType();
    assert(selfType != nullptr);
    FunctionOverloadedSymbolPtr funcs = selfType->getSubscript();
    if(funcs == nullptr)
    {
        //undefined subscript access
        wstring s = this->toString(node);
        error(node, Errors::E_UNDEFINED_SUBSCRIPT_ACCESS_FOR_1, s);
        return;
    }
    vector<SymbolPtr> functions;
    for(const FunctionSymbolPtr& func : *funcs)
        functions.push_back(func);
    //Now inference the type returned by this subscript access
    SymbolPtr func = getOverloadedFunction(node, functions, node->getIndex());
    assert(func && func->getType() && func->getType()->getCategory() == Type::Function);
    node->setType(func->getType()->getReturnType());
}
