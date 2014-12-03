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


USE_SWALLOW_NS
using namespace std;
bool SemanticAnalyzer::isInteger(const TypePtr& type)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    for(const TypePtr& t : scope->t_Ints)
    {
        if(t == type)
            return true;
    }
    return false;
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
    if(type == scope->Float || type == scope->Double)
        return true;
    return false;
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
        if(valueType && *valueType != *type)
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
        if(hint->canAssignTo(scope->FloatingPointType))
        {
            score  = 0.5;
            return hint;
        }
        if(hint->canAssignTo(scope->_IntegerType))
        {
            if(hint == scope->Int)
                score = 1;
            return hint;
        }

    }
    if(expr->getNodeType() == NodeType::FloatLiteral && hint != nullptr)
    {
        FloatLiteralPtr literal = std::static_pointer_cast<FloatLiteral>(expr);
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
                {
                    error(argument.second, Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL_1, parameter.name);
                }
                else
                {
                    error(argument.second, Errors::E_EXTRANEOUS_ARGUMENT_LABEL_IN_CALL_1, name);
                }
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
        StackedValueGuard<TypePtr> contextualType(t_hint);
        contextualType.set(parameter.type);
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

FunctionSymbolPtr SemanticAnalyzer::getOverloadedFunction(const NodePtr& node, const FunctionOverloadedSymbolPtr& funcs, const ParenthesizedExpressionPtr& arguments)
{
    typedef std::tuple<float, FunctionSymbolPtr, TypePtr> ScoredFunction;
    std::vector<ScoredFunction> candidates;
    for(const FunctionSymbolPtr& func : *funcs)
    {
        TypePtr type = func->getType();
        wstring s = func->getName() + L" : " + type->toString();
        float score = calculateFitScore(type, arguments, true);
        if(score > 0)
            candidates.push_back(std::make_tuple(score, func, type));
    }
    if(candidates.empty())
    {
        error(node, Errors::E_NO_MATCHED_OVERLOAD);
        abort();
    }
    //sort by fit score
    if(candidates.size() > 1)
    {
        sort(candidates.begin(), candidates.end(), [](const ScoredFunction& lhs, const ScoredFunction& rhs ){
            return get<0>(rhs) < get<0>(lhs);
        });
        if(get<0>(candidates[0]) == get<0>(candidates[1]))
        {
            error(node, Errors::E_AMBIGUOUS_USE_1, funcs->getName());
            abort();
        }
    }
    FunctionSymbolPtr matched = get<1>(candidates.front());
    return matched;
}

void SemanticAnalyzer::visitFunctionCall(const SymbolPtr& sym, const ParenthesizedExpressionPtr& args, const PatternPtr& node)
{

    if(FunctionSymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        //verify argument
        std::wstring name = func->getName();
        TypePtr type = func->getType();
        calculateFitScore(type, args, false);
        node->setType(type->getReturnType());
    }
    else if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
    {
        if(funcs->numOverloads() == 1)
        {
            FunctionSymbolPtr func = *funcs->begin();
            //verify argument
            std::wstring name = func->getName();
            TypePtr type = func->getType();
            calculateFitScore(type, args, false);
            node->setType(type->getReturnType());
        }
        else
        {
            FunctionSymbolPtr matched = getOverloadedFunction(node, funcs, args);
            node->setType(matched->getReturnType());
        }
    }
    else if(SymbolPlaceHolderPtr symbol = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
    {
        //it must be a function type to call
        TypePtr type = symbol->getType();
        assert(type != nullptr);
        if(type->getCategory() != Type::Function && type->getCategory() != Type::Closure)
        {
            error(node, Errors::E_INVALID_USE_OF_A_TO_CALL_A_VALUE_OF_NON_FUNCTION_TYPE_B_2, toString(node), type->toString());
            abort();
            return;
        }
        calculateFitScore(type, args, false);
        node->setType(type->getReturnType());
    }
    else
    {
        assert(0 && "Unsupported function to call");
    }

}

void SemanticAnalyzer::visitReturn(const ReturnStatementPtr& node)
{
    NodeVisitor::visitReturn(node);
    Node* owner = symbolRegistry->getCurrentScope()->getOwner();
    assert(owner != nullptr);
    TypePtr funcType;
    switch(owner->getNodeType())
    {
        case NodeType::Program:
            return;//return type checking is ignored in program scope
        case NodeType::CodeBlock:
            funcType = static_cast<CodeBlock*>(owner)->getType();
            assert(funcType != nullptr);
            break;
        case NodeType::Closure:
            funcType = static_cast<Closure*>(owner)->getType();
            assert(funcType != nullptr);
            break;
        default:
            assert(0 && "Unsupported scope of return keyword detected.");
            break;

    }
    float score = 0;
    TypePtr retType = this->getExpressionType(node->getExpression(), funcType->getReturnType(), score);
    TypePtr expectedType = funcType->getReturnType();
    if(!Type::equals(retType, expectedType))
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
            SymbolPtr sym = symbolRegistry->lookupSymbol(symbolName);
            if (!sym)
            {
                error(id, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, symbolName);
                return;
            }
            //if symbol points to a type, then redirect it to a initializer
            if (TypePtr type = std::dynamic_pointer_cast<Type>(sym))
            {
                if (type->getCategory() == Type::Class || type->getCategory() == Type::Struct)
                {
                    sym = type->getInitializer();
                }
            }
            visitFunctionCall(sym, node->getArguments(), node);
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
            if(selfType->getCategory() == Type::MetaType)
            {
                selfType = selfType->getInnerType();
                if(selfType->getCategory() == Type::Enum && (c = selfType->getEnumCase(identifier)) && c->constructor)
                {
                    //constructing an enum
                    sym = c->constructor;
                }
                else
                {
                    sym = selfType->getDeclaredStaticMember(identifier);
                }
            }
            else
            {
                sym = selfType->getMember(identifier);
            }
            if(!sym)
            {
                error(ma, Errors::E_DOES_NOT_HAVE_A_MEMBER_2, selfType->toString(), identifier);
                abort();
                return;
            }
            assert(sym != nullptr);//
            visitFunctionCall(sym, node->getArguments(), node);
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
    if(!symbolRegistry->getGlobalScope()->isOptional(type))
    {
        error(node, Errors::E_OPERAND_OF_POSTFIX_A_SHOULD_HAVE_OPTIONAL_TYPE_TYPE_IS_B_2, L"?", type->toString());
        return;
    }
    node->setType(type->getGenericArguments()->get(0));
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
        SymbolPtr member;
        if (staticAccess)
            member = selfType->getDeclaredStaticMember(fieldName);
        else
            member = selfType->getMember(fieldName);
        if (member == nullptr)
        {
            //diagnose why it's not found
            if(staticAccess && selfType->getCategory() == Type::Enum && selfType->getEnumCase(fieldName))
            {
                //it's a enum's case access, it's not allowed to be accessed directly
                error(node, Errors::E_PARTIAL_APPLICATION_OF_ENUM_CONSTRUCTOR_IS_NOT_ALLOWED);
                abort();
            }
            error(node->getField(), Errors::E_DOES_NOT_HAVE_A_MEMBER_2, selfType->toString(), fieldName);
            return;
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
    if(!parentNode || (parentNode->getNodeType() != NodeType::MemberAccess && parentNode->getNodeType() != NodeType::Assignment))
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
    if(!t_hint || !t_hint->canAssignTo(scope->NilLiteralConvertible))
    {
        error(node, Errors::E_EXPRESSION_DOES_NOT_CONFORM_TO_PROTOCOL_1, L"NilLiteralConvertible");
        return;
    }
    node->setType(t_hint);
}
void SemanticAnalyzer::visitBooleanLiteral(const BooleanLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->BooleanLiteralConvertible))
        node->setType(t_hint);
    else
        node->setType(scope->Bool);
}
void SemanticAnalyzer::visitString(const StringLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->StringLiteralConvertible))
        node->setType(t_hint);
    else if(t_hint && node->value.length() == 1 && t_hint->canAssignTo(scope->UnicodeScalarLiteralConvertible))
        node->setType(t_hint);
    else
        node->setType(scope->String);
}
void SemanticAnalyzer::visitStringInterpolation(const StringInterpolationPtr &node)
{
    //TODO: check all expressions inside can be converted to string
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->StringInterpolationConvertible))
        node->setType(t_hint);
    else
        node->setType(scope->String);
}
void SemanticAnalyzer::visitInteger(const IntegerLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    //TODO: it will changed to use standard library's overloaded type constructor to infer type when the facility is mature enough.
    if(t_hint && t_hint->canAssignTo(scope->IntegerLiteralConvertible))
        node->setType(t_hint);
    else if(t_hint && t_hint->canAssignTo(scope->FloatLiteralConvertible))
        node->setType(t_hint);
    else
        node->setType(scope->Int);
}
void SemanticAnalyzer::visitFloat(const FloatLiteralPtr& node)
{
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint && t_hint->canAssignTo(scope->FloatLiteralConvertible))
        node->setType(t_hint);
    else
        node->setType(scope->Double);
}

//Will be replaced by stdlib's type constructor
bool SemanticAnalyzer::canConvertTo(const ExpressionPtr& expr, const TypePtr& type)
{
    switch(expr->getNodeType())
    {
        case NodeType::IntegerLiteral:
            return isNumber(type);
        case NodeType::FloatLiteral:
            return isFloat(type);
        default:
            return false;
    }
    return false;
}

void SemanticAnalyzer::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    int num = node->numElements();
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(t_hint)
    {
        //TODO if hint specified, it must be Array type nor conform to ArrayLiteralConvertible protocol
        if(t_hint->getInnerType() != scope->Array)
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
            error(node, Errors::E_CANNOT_DEFINE_EMPTY_ARRAY_WITHOUT_TYPE);
        return;
    }
    //bool hasHint = t_hint != nullptr;
    TypePtr type = t_hint != nullptr ? t_hint->getInnerType() : nullptr;
    TypePtr hint;
    for(const ExpressionPtr& el : *node)
    {
        el->accept(this);
        if(type != nullptr)
        {
            //check if element can be converted into given type
            if(!canConvertTo(el, type))
            {
                error(el, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(el), type->getName());
            }
        }
        else
        {
            switch(el->getNodeType())
            {
                case NodeType::IntegerLiteral:
                    hint = scope->Int;
                    break;
                case NodeType::FloatLiteral:
                    hint = scope->Double;
                    break;
                default:
                    type = el->getType();
                    if(hint)
                    {
                        //check if the hint can be converted to type
                        bool success = (hint == scope->Double && isFloat(type)) || (hint == scope->Int && isNumber(type));
                        if(!success)
                        {
                            error(el, Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES);
                        }

                    }
                    if(type->getCategory() == Type::MetaType)
                        type = type->getInnerType();
                    break;
            }
        }
    }

    assert(type != nullptr || hint != nullptr);

    if(!type && hint)
        type = hint;
    TypePtr arrayType = scope->makeArray(type);
    node->setType(arrayType);
}
void SemanticAnalyzer::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{

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
        StackedValueGuard<TypePtr> hintGuard(t_hint);
        hintGuard.set(elementHint);
        element.expression->accept(this);
        element.expression = transformExpression(elementHint, element.expression);
        TypePtr elementType = element.expression->getType();
        assert(elementType != nullptr);
        types.push_back(elementType);
    }
    TypePtr type = Type::newTuple(types);
    node->setType(type);
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
        StackedValueGuard<TypePtr> hintGuard(t_hint);
        hintGuard.set(elementHint);
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

void SemanticAnalyzer::visitOperator(const OperatorDefPtr& node)
{
    //register operator
    if(node->getType() == OperatorType::InfixBinary)
    {
        //check precedence range
        if(node->getPrecedence() < 0 || node->getPrecedence() > 255)
        {
            error(node, Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE);
            abort();
        }
    }

    bool r = symbolRegistry->registerOperator(node->getName(), node->getType(), node->getAssociativity(), node->getPrecedence());
    if(!r)
    {
        error(node, Errors::E_OPERATOR_REDECLARED);
        abort();
    }
}
void SemanticAnalyzer::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

}
void SemanticAnalyzer::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    //look for binary function that matches
    OperatorInfo* op = symbolRegistry->getOperator(node->getOperator());
    SymbolPtr sym = symbolRegistry->lookupSymbol(node->getOperator());
    if(!op || !sym)
    {
        error(node, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, node->getOperator());
        error(node, Errors::E_UNKNOWN_BINARY_OPERATOR_1, node->getOperator());
        abort();
    }
    if((op->type & OperatorType::InfixBinary) == 0)
    {
        error(node, Errors::E_IS_NOT_BINARY_OPERATOR_1, node->getOperator());
        error(node, Errors::E_UNKNOWN_BINARY_OPERATOR_1, node->getOperator());
        abort();
    }

    ExpressionPtr lhs = dynamic_pointer_cast<Expression>(node->getLHS());
    ExpressionPtr rhs = dynamic_pointer_cast<Expression>(node->getRHS());
    assert(lhs != nullptr);
    assert(rhs != nullptr);
    ParenthesizedExpressionPtr args(node->getNodeFactory()->createParenthesizedExpression(*node->getSourceInfo()));
    args->append(lhs);
    args->append(rhs);
    this->visitFunctionCall(sym, args, node);
    //find for overload
}
void SemanticAnalyzer::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    error(node, Errors::E_NO_OVERLOAD_ACCEPTS_ARGUMENTS_1, node->getOperator());
}


void SemanticAnalyzer::visitCompileConstant(const CompileConstantPtr& node)
{
    const std::wstring& name = node->getName();
    GlobalScope* scope = symbolRegistry->getGlobalScope();
    if(name == L"__LINE__" || name == L"__COLUMN__")
    {
        node->setType(scope->Int);
    }
    else if(name == L"__FUNCTION__" || name == L"__FILE__")
    {
        node->setType(scope->String);
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
        abort();
    }
    //Now inference the type returned by this subscript access
    FunctionSymbolPtr func = getOverloadedFunction(node, funcs, node->getIndex());
    node->setType(func->getReturnType());
}
