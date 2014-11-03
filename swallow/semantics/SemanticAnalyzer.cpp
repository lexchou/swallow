/* SemanticAnalyzer.cpp --
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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
#include <cassert>
#include "SymbolScope.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include <string>
#include "TypeBuilder.h"
#include "common/CompilerResults.h"
#include "SymbolRegistry.h"
#include "ast/NodeSerializer.h"
#include "GlobalScope.h"

USE_SWIFT_NS
using namespace std;


SemanticAnalyzer::SemanticAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
:symbolRegistry(symbolRegistry), compilerResults(compilerResults)
{

}
void SemanticAnalyzer::prepareParameters(SymbolScope* scope, const ParametersPtr& params)
{
    //check and prepare for parameters

    for(const ParameterPtr& param : *params)
    {
        assert(param->getType() != nullptr);
        SymbolPtr sym = scope->lookup(param->getLocalName());
        if(sym)
        {
            error(param, Errors::E_DEFINITION_CONFLICT, param->getLocalName());
            return;
        }
        sym = SymbolPtr(new SymbolPlaceHolder(param->getLocalName(), param->getType(), SymbolPlaceHolder::R_PARAMETER, SymbolPlaceHolder::F_INITIALIZED));
        scope->addSymbol(sym);
    }
    //prepare for implicit parameter self

}

GenericDefinitionPtr SemanticAnalyzer::prepareGenericTypes(const GenericParametersDefPtr& params)
{
    GenericDefinitionPtr ret(new GenericDefinition());
    for (const TypeIdentifierPtr &typeId : *params)
    {
        if (typeId->getNestedType())
        {
            error(typeId->getNestedType(), Errors::E_NESTED_TYPE_IS_NOT_ALLOWED_HERE);
            continue;
        }
        std::wstring name = typeId->getName();
        TypePtr old = ret->get(name);
        if (old != nullptr)
        {
            error(typeId, Errors::E_DEFINITION_CONFLICT);
            continue;
        }
        std::vector<TypePtr> protocols;
        TypePtr type = Type::newType(name, Type::GenericParameter, nullptr, nullptr, protocols);
        ret->add(name, type);
    }
    //add constraint
    for(const GenericConstraintDefPtr& constraint : params->getConstraints())
    {
        //constraint->getConstraintType()
        list<wstring> types;
        TypeIdentifierPtr typeId = constraint->getIdentifier();
        TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(ret->get(typeId->getName()));
        TypePtr expectedType = lookupType(constraint->getExpectedType());

        if(type == nullptr)
        {
            error(typeId, Errors::E_USE_OF_UNDECLARED_TYPE_1, typeId->getName());
            continue;
        }
        if(constraint->getConstraintType() == GenericConstraintDef::EqualsTo)
        {
            //Same-type requirement makes generic parameter 'T' non-generic
            if(typeId->getNestedType() == nullptr)
            {
                error(typeId, Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1, typeId->getName());
                continue;
            }
            //check if the reference type is a protocol that contains Self or associated type
            if(expectedType->getCategory() == Type::Protocol)
            {
                if(expectedType->containsSelfType() || expectedType->containsAssociatedType())
                {
                    error(typeId, Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1, expectedType->getName());
                    continue;
                }
            }
        }
        types.push_back(typeId->getName());
        typeId = typeId->getNestedType();
        while(typeId != nullptr)
        {
            wstring name = typeId->getName();
            types.push_back(name);
            TypePtr childType = type->getAssociatedType(name);
            if(!childType)
            {
                //childType = Type::newType(name, Type::Placeholder, nullptr);
                //type->getSymbols()[name] = childType;
                error(typeId, Errors::E_IS_NOT_A_MEMBER_OF_2, name, type->getName());
                return ret;
            }
            type = static_pointer_cast<TypeBuilder>(childType);
            typeId = typeId->getNestedType();
        }
        if(expectedType->getCategory() == Type::Protocol)
        {
            type->addProtocol(expectedType);
        }
        else
        {
            //it's a base type
            if(type->getParentType() != nullptr)
            {
                error(constraint->getIdentifier(), Errors::E_MULTIPLE_INHERITANCE_FROM_CLASS_2_, type->getParentType()->getName(), expectedType->getName());
                continue;
            }
            type->setParentType(expectedType);
        }

        //ret->addConstraint(types);
        GenericDefinition::ConstraintType ct = constraint->getConstraintType() == GenericConstraintDef::DerivedFrom ? GenericDefinition::DerivedFrom : GenericDefinition::EqualsTo;
        ret->addConstraint(types, ct, expectedType);
    }

    return ret;
}

/**
* Abort the visitor
*/
void SemanticAnalyzer::abort()
{
    throw Abort();
}
void SemanticAnalyzer::error(const NodePtr& node, int code, const std::vector<std::wstring>& items)
{
    assert(node != nullptr);
    compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), code, items);
    abort();
}
/**
* Outputs an compiler error
*/
void SemanticAnalyzer::error(const NodePtr& node, int code, const std::wstring& item)
{
    ResultItems items = {item};
    error(node, code, items);
}
void SemanticAnalyzer::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2)
{
    ResultItems items = {item1, item2};
    error(node, code, items);
}
void SemanticAnalyzer::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3)
{
    ResultItems items = {item1, item2, item3};
    error(node, code, items);
}
void SemanticAnalyzer::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3, const std::wstring& item4)
{
    ResultItems items = {item1, item2, item3, item4};
    error(node, code, items);
}

void SemanticAnalyzer::error(const NodePtr& node, int code)
{
    error(node, code, L"");
}
void SemanticAnalyzer::warning(const NodePtr& node, int code, const std::wstring& item)
{
    compilerResults->add(ErrorLevel::Warning, *node->getSourceInfo(), code, item);
}

std::wstring SemanticAnalyzer::toString(const NodePtr& node)
{
    std::wstringstream out;
    NodeSerializerW serializer(out);
    node->accept(&serializer);
    return out.str();
}
std::wstring SemanticAnalyzer::toString(int i)
{
    std::wstringstream s;
    s<<i;
    return s.str();
}
TypePtr SemanticAnalyzer::lookupType(const TypeNodePtr& type)
{
    if(!type)
        return nullptr;
    TypePtr ret = type->getType();
    if(!ret)
    {
        ret = lookupTypeImpl(type);
        type->setType(ret);
    }
    return ret;
}
TypePtr SemanticAnalyzer::lookupTypeImpl(const TypeNodePtr &type)
{
    if(TypeIdentifierPtr id = std::dynamic_pointer_cast<TypeIdentifier>(type))
    {
        //TODO: make a generic type if possible
        TypePtr ret = symbolRegistry->lookupType(id->getName());
        if(!ret)
        {
            std::wstring str = toString(type);
            error(type, Errors::E_USE_OF_UNDECLARED_TYPE_1, str);
            abort();
        }
        GenericDefinitionPtr generic = ret->getGenericDefinition();
        if(generic == nullptr && id->numGenericArguments() == 0)
            return ret;
        if(generic == nullptr && id->numGenericArguments() > 0)
        {
            std::wstring str = toString(type);
            error(id, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, str);
            return nullptr;
        }
        if(generic != nullptr && id->numGenericArguments() == 0)
        {
            std::wstring str = toString(type);
            error(id, Errors::E_GENERIC_TYPE_ARGUMENT_REQUIRED, str);
            return nullptr;
        }
        if(id->numGenericArguments() > generic->numParameters())
        {
            std::wstring str = toString(type);
            std::wstring got = toString(id->numGenericArguments());
            std::wstring expected = toString(generic->numParameters());
            error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3, str, got, expected);
            return nullptr;
        }
        if(id->numGenericArguments() < generic->numParameters())
        {
            std::wstring str = toString(type);
            std::wstring got = toString(id->numGenericArguments());
            std::wstring expected = toString(generic->numParameters());
            error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3, str, got, expected);
            return nullptr;
        }
        //check type
        GenericArgumentPtr genericArgument(new GenericArgument(generic));
        for(auto arg : *id)
        {
            TypePtr argType = lookupType(arg);
            if(!argType)
                return nullptr;
            genericArgument->add(argType);
        }
        TypePtr base = Type::newSpecializedType(ret, genericArgument);
        ret = base;
        //access rest nested types
        for(TypeIdentifierPtr n = id->getNestedType(); n != nullptr; n = n->getNestedType())
        {
            TypePtr childType = ret->getAssociatedType(n->getName());
            if(!childType)
            {
                error(n, Errors::E_A_IS_NOT_A_MEMBER_TYPE_OF_B_2, n->getName(), ret->toString());
                return nullptr;
            }
            if(n->numGenericArguments())
            {
                //nested type is always a non-generic type
                error(n, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, childType->toString());
                return nullptr;
            }
            ret = childType;
        }

        return ret;
    }
    if(TupleTypePtr tuple = std::dynamic_pointer_cast<TupleType>(type))
    {
        std::vector<TypePtr> elementTypes;
        for(const TupleType::TupleElement& e : *tuple)
        {
            TypePtr t = lookupType(e.type);
            elementTypes.push_back(t);
        }
        return Type::newTuple(elementTypes);
    }
    if(ArrayTypePtr array = std::dynamic_pointer_cast<ArrayType>(type))
    {
        GlobalScope* scope = symbolRegistry->getGlobalScope();
        TypePtr Array = scope->t_Array;
        TypePtr innerType = lookupType(array->getInnerType());
        assert(innerType != nullptr);
        assert(Array != nullptr);
        TypePtr ret = Type::newSpecializedType(Array, innerType);
        return ret;
    }
    if(OptionalTypePtr opt = std::dynamic_pointer_cast<OptionalType>(type))
    {
        GlobalScope* scope = symbolRegistry->getGlobalScope();
        TypePtr Optional = scope->t_Optional;
        TypePtr innerType = lookupType(opt->getInnerType());
        assert(innerType != nullptr);
        assert(Optional != nullptr);
        TypePtr ret = Type::newSpecializedType(Optional, innerType);
        return ret;
    }
    if(FunctionTypePtr func = std::dynamic_pointer_cast<FunctionType>(type))
    {
        TypePtr retType = nullptr;
        if(func->getReturnType())
        {
            retType = lookupType(func->getReturnType());
        }
    }
    assert(0 && "Unsupported type");
    return nullptr;
}
