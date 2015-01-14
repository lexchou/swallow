/* SemanticAnalyzer_Literal.cpp --
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
#include <cassert>
#include <algorithm>
#include "ast/NodeFactory.h"
#include "CollectionTypeAnalyzer.h"
#include <iostream>
#include "common/ScopedValue.h"

USE_SWALLOW_NS
using namespace std;

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
