/* SemanticAnalyzer_FunctionCall.cpp --
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

float SemanticAnalyzer::calculateFitScore(bool mutatingSelf, SymbolPtr& func, const ParenthesizedExpressionPtr& arguments, bool supressErrors)
{
    float score = 0;
    TypePtr type = func->getType();
    assert(type != nullptr);
    const std::vector<Type::Parameter>& parameters = type->getParameters();
    bool variadic = type->hasVariadicParameters();

    //TODO: check trailing closure

    //check mutating
    //do not allow to call mutating function in non-mutating context
    if(type->hasFlags(SymbolFlagMember) && type->hasFlags(SymbolFlagMutating))
    {
        TypePtr declaringType = func->getDeclaringType();
        assert(declaringType != nullptr);
        if(!mutatingSelf)
        {
            if(!supressErrors)
                error(arguments, Errors::E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2, declaringType->toString(), func->getName());
            return -1;
        }
    }


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
        bool ret = checkArgument(type, parameter, make_pair(argumentIter->name, argumentIter->transformedExpression), false, score, supressErrors, genericTypes);
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
            bool ret = checkArgument(type, parameter, make_pair(argumentIter->name, argumentIter->transformedExpression), false, score, supressErrors, genericTypes);
            argumentIter++;
            if(!ret)
                return -1;
        }

        //check rest argument
        for(;argumentIter != arguments->end(); argumentIter++)
        {
            bool ret = checkArgument(type, parameter, make_pair(argumentIter->name, argumentIter->transformedExpression), true, score, supressErrors, genericTypes);
            if(!ret)
                return -1;
        }
    }

    //TODO: refactor this, function specialization shouldn't be done here
    if(type->getGenericDefinition())// && !genericTypes.empty())
    {
        GenericDefinitionPtr generic = type->getGenericDefinition();
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
        //Specialization on function call depends on varying type arguments
        CodeBlockPtr definition = nullptr;
        type = type->newSpecializedType(type, genericTypes);
        func = FunctionSymbolPtr(new FunctionSymbol(func->getName(), type, definition));
    }

    if(!arguments->numExpressions())
        return 1;
    return score / arguments->numExpressions();
}

SymbolPtr SemanticAnalyzer::getOverloadedFunction(bool mutatingSelf, const NodePtr& node, const std::vector<SymbolPtr>& funcs, const ParenthesizedExpressionPtr& arguments)
{
    typedef std::tuple<float, SymbolPtr, TypePtr> ScoredFunction;
    std::vector<ScoredFunction> candidates;
    for(SymbolPtr func : funcs)
    {
        assert(func->getType() && func->getType()->getCategory() == Type::Function);
        float score = calculateFitScore(mutatingSelf, func, arguments, true);
        TypePtr type = func->getType();
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

SymbolPtr SemanticAnalyzer::visitFunctionCall(bool mutatingSelf, const std::vector<SymbolPtr>& funcs, const ParenthesizedExpressionPtr& args, const PatternPtr& node)
{
    if(funcs.size() == 1)
    {
        SymbolPtr sym = funcs.front();
        if(SymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
        {
            //verify argument
            std::wstring name = func->getName();
            calculateFitScore(mutatingSelf, func, args, false);
            TypePtr type = func->getType();
            //check mutating function

            node->setType(type->getReturnType());
            return func;
        }
        else if(SymbolPtr symbol = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
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
            calculateFitScore(mutatingSelf, symbol, args, false);
            node->setType(symbol->getType()->getReturnType());
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
        SymbolPtr matched = getOverloadedFunction(mutatingSelf, node, funcs, args);
        assert(matched != nullptr && matched->getType()->getCategory() == Type::Function);
        node->setType(matched->getType()->getReturnType());
        return matched;
    }

}

void SemanticAnalyzer::visitFunctionCall(const FunctionCallPtr& node)
{
    NodeType::T nodeType = node->getFunction()->getNodeType();
    ExpressionPtr func = node->getFunction();

    //merge trailing closure as the last parameter
    if(node->getTrailingClosure())
    {
        node->getArguments()->append(node->getTrailingClosure());
    }


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
            bool mutatingSelf = true;
            if(currentType)
            {
                // TODO: currentFunction might be nullptr while this is called during variable initialization
                Type::Category category = currentType->getCategory();
                if(category == Type::Struct || category == Type::Enum)
                {
                    if(!currentFunction || !currentFunction->hasFlags(SymbolFlagMutating))
                    {
                        mutatingSelf = false;
                    }
                }
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
            visitFunctionCall(mutatingSelf, funcs, node->getArguments(), node);
            break;
        }
        case NodeType::MemberAccess:
        {
            MemberAccessPtr ma = std::static_pointer_cast<MemberAccess>(func);
            ma->getSelf()->accept(this);
            TypePtr selfType = ma->getSelf()->getType();
            bool mutatingSelf = !containsReadonlyNode(ma->getSelf());
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
            SymbolPtr func = visitFunctionCall(mutatingSelf, funcs, node->getArguments(), node);
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
            //make a temporary value for this closure
            SymbolPtr tmp(new SymbolPlaceHolder(L"", type, SymbolPlaceHolder::R_LOCAL_VARIABLE, 0));
            assert(type != nullptr && type->getCategory() == Type::Function);
            bool mutatingSelf = false;
            calculateFitScore(mutatingSelf, tmp, node->getArguments(), false);
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
