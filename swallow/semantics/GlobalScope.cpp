/* GlobalScope.cpp --
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
#include "GlobalScope.h"
#include "Type.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "TypeBuilder.h"
#include <cstdarg>
#include <cassert>

USE_SWALLOW_NS
using namespace std;

GlobalScope::GlobalScope()
{
    initPrimitiveTypes();
    initOperators();
    initProtocols();
}
void GlobalScope::initPrimitiveTypes()
{
    //Protocols
    TypeBuilderPtr type;
    #define DECLARE_TYPE(T, N)  N = type = static_pointer_cast<TypeBuilder>(Type::newType(L###N, Type::T)); addSymbol(N);
    #define IMPLEMENTS(P) type->addProtocol(P);

    DECLARE_TYPE(Protocol, BooleanType);
    DECLARE_TYPE(Protocol, IntegerLiteralConvertible);
    DECLARE_TYPE(Protocol, BooleanLiteralConvertible);
    DECLARE_TYPE(Protocol, StringLiteralConvertible);
    DECLARE_TYPE(Protocol, FloatLiteralConvertible);
    DECLARE_TYPE(Protocol, NilLiteralConvertible);
    DECLARE_TYPE(Protocol, ArrayLiteralConvertible);
    DECLARE_TYPE(Protocol, DictionaryLiteralConvertible);
    DECLARE_TYPE(Protocol, UnicodeScalarLiteralConvertible);
    DECLARE_TYPE(Protocol, ExtendedGraphemeClusterLiteralConvertible);
    IMPLEMENTS(UnicodeScalarLiteralConvertible);

    DECLARE_TYPE(Protocol, Equatable);
    DECLARE_TYPE(Protocol, Hashable);
    DECLARE_TYPE(Protocol, Comparable);

    DECLARE_TYPE(Protocol, RawRepresentable);
    {
        TypePtr RawValue = Type::newType(L"RawValue", Type::Alias);
        type->addMember(RawValue);

        SymbolPlaceHolderPtr rawValue(new SymbolPlaceHolder(L"rawValue", RawValue, SymbolPlaceHolder::R_PROPERTY, SymbolFlagMember | SymbolFlagReadable));
        type->addMember(rawValue);
    }


    DECLARE_TYPE(Protocol, _IntegerType);
    IMPLEMENTS(IntegerLiteralConvertible);

    DECLARE_TYPE(Protocol, SignedIntegerType);
    IMPLEMENTS(_IntegerType);
    IMPLEMENTS(Comparable);
    IMPLEMENTS(Equatable);

    DECLARE_TYPE(Protocol, UnsignedIntegerType);
    IMPLEMENTS(_IntegerType);
    IMPLEMENTS(Comparable);
    IMPLEMENTS(Equatable);

    DECLARE_TYPE(Protocol, FloatingPointType);
    IMPLEMENTS(Comparable);
    IMPLEMENTS(Equatable);

    DECLARE_TYPE(Protocol, StringInterpolationConvertible);




    //Types
    DECLARE_TYPE(Aggregate, Int8);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Aggregate, UInt8);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Aggregate, Int16);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Aggregate, UInt16);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Aggregate, Int32);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Aggregate, UInt32);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Aggregate, Int64);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Aggregate, UInt64);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Aggregate, Int);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Aggregate, UInt);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Struct, _OptionalNilComparisonType);
    IMPLEMENTS(NilLiteralConvertible);


    DECLARE_TYPE(Aggregate, Bool);
    IMPLEMENTS(BooleanType);
    IMPLEMENTS(BooleanLiteralConvertible);
    IMPLEMENTS(Equatable)
    IMPLEMENTS(Hashable)

    DECLARE_TYPE(Aggregate, Float);
    IMPLEMENTS(FloatingPointType);
    IMPLEMENTS(IntegerLiteralConvertible);
    IMPLEMENTS(FloatLiteralConvertible);

    DECLARE_TYPE(Aggregate, Double);
    IMPLEMENTS(FloatingPointType);
    IMPLEMENTS(IntegerLiteralConvertible);
    IMPLEMENTS(FloatLiteralConvertible);

    DECLARE_TYPE(Aggregate, String);
    IMPLEMENTS(StringLiteralConvertible);
    IMPLEMENTS(UnicodeScalarLiteralConvertible);
    IMPLEMENTS(ExtendedGraphemeClusterLiteralConvertible);
    IMPLEMENTS(Hashable);
    IMPLEMENTS(Equatable);
    IMPLEMENTS(StringInterpolationConvertible);

    DECLARE_TYPE(Aggregate, Character);
    IMPLEMENTS(ExtendedGraphemeClusterLiteralConvertible);
    IMPLEMENTS(Equatable);
    IMPLEMENTS(Hashable);
    IMPLEMENTS(Comparable);

    addSymbol(L"Void", Void = Type::newTuple(vector<TypePtr>()));

    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        Array = Type::newType(L"Array", Type::Struct, nullptr, nullptr, std::vector<TypePtr>(), generic);
        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(Array);
        {
            std::vector<Type::Parameter> params = {Type::Parameter(T)};
            TypePtr t_append = Type::newFunction(params, Void, false, nullptr);

            FunctionSymbolPtr append(new FunctionSymbol(L"append", t_append, nullptr));
            builder->addMember(append);
        }
        {
            std::vector<Type::Parameter> params = {};
            TypePtr t_append = Type::newFunction(params, T, false, nullptr);

            FunctionSymbolPtr append(new FunctionSymbol(L"removeLast", t_append, nullptr));
            builder->addMember(append);
        }
        {
            SymbolPlaceHolderPtr count(new SymbolPlaceHolder(L"count", Int, SymbolPlaceHolder::R_PROPERTY, SymbolFlagInitialized | SymbolFlagReadable | SymbolFlagMember));
            builder->addMember(count);
        }
        {
            std::vector<Type::Parameter> params = {Type::Parameter(Int)};
            TypePtr t_append = Type::newFunction(params, T, false, nullptr);

            FunctionSymbolPtr subscript(new FunctionSymbol(L"subscript", t_append, nullptr));
            FunctionOverloadedSymbolPtr subscripts(new FunctionOverloadedSymbol(L"subscript"));
            subscripts->add(subscript);
            builder->addMember(subscripts);
        }
        addSymbol(Array);
    }
    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        Optional = Type::newType(L"Optional", Type::Enum, nullptr, nullptr, std::vector<TypePtr>(), generic);
        type = static_pointer_cast<TypeBuilder>(Optional);
        type->addProtocol(NilLiteralConvertible);

        type->addEnumCase(L"None", Void);
        std::vector<TypePtr> SomeArgs = {T};
        type->addEnumCase(L"Some", Type::newTuple(SomeArgs));
        addSymbol(Optional);
    }
}


void GlobalScope::declareFunction(const std::wstring&name, int flags, const wchar_t* returnType, ...)
{
    va_list va;
    va_start(va, returnType);
    FunctionSymbolPtr fn = vcreateFunction(name, flags, returnType, va);
    va_end(va);

    auto iter = symbols.find(name);
    if(iter == symbols.end())
    {
        addSymbol(fn);
        return;
    }
    if(FunctionOverloadedSymbolPtr overloads = dynamic_pointer_cast<FunctionOverloadedSymbol>(iter->second))
    {
        overloads->add(fn);
    }
    else if(FunctionSymbolPtr old = dynamic_pointer_cast<FunctionSymbol>(iter->second))
    {
        FunctionOverloadedSymbolPtr overloads(new FunctionOverloadedSymbol(name));
        overloads->add(old);
        overloads->add(fn);
        iter->second = overloads;
    }
    else
    {
        assert(0 && "Symbol already defined with other types.");
    }
}

/*!
 * A short-hand way to create a function symbol
 */
FunctionSymbolPtr GlobalScope::createFunction(const std::wstring&name, int flags, const wchar_t* returnType, ...)
{
    va_list va;
    va_start(va, returnType);
    FunctionSymbolPtr ret = vcreateFunction(name, flags, returnType, va);
    va_end(va);
    return ret;
}

FunctionSymbolPtr GlobalScope::vcreateFunction(const std::wstring&name, int flags, const wchar_t* returnType, va_list va)
{
    std::vector<Type::Parameter> params;
    TypePtr retType = dynamic_pointer_cast<Type>(lookup(returnType));
    bool variadicParams = false;
    assert(retType != nullptr);
    while(true)
    {
        const wchar_t* typeName = va_arg(va, const wchar_t*);
        if(!typeName)
            break;
        TypePtr paramType = dynamic_pointer_cast<Type>(lookup(typeName));
        assert(paramType != nullptr);
        params.push_back(Type::Parameter(paramType));
    }
    TypePtr funcType = Type::newFunction(params, retType, variadicParams, nullptr);
    FunctionSymbolPtr ret(new FunctionSymbol(name, funcType, nullptr));
    ret->setFlags(flags);
    return ret;
}

TypePtr GlobalScope::makeArray(const TypePtr& elementType) const
{
    GenericArgumentPtr ga(new GenericArgument(Array->getGenericDefinition()));
    ga->add(elementType);
    return Type::newSpecializedType(Array, ga);
}

/*!
 * A short-hand way to create an Optional type
 */
TypePtr GlobalScope::makeOptional(const TypePtr& elementType) const
{
    GenericArgumentPtr ga(new GenericArgument(Optional->getGenericDefinition()));
    ga->add(elementType);
    return Type::newSpecializedType(Optional, ga);
}

/*!
 * A short-hand way to create a Dictionary type
 */
TypePtr GlobalScope::makeDictionary(const TypePtr& keyType, const TypePtr& valueType) const
{
    GenericArgumentPtr ga(new GenericArgument(Dictionary->getGenericDefinition()));
    ga->add(keyType);
    ga->add(valueType);
    return Type::newSpecializedType(Dictionary, ga);
}

void GlobalScope::initOperators()
{
    //Register built-in functions
    //Register built-in operators
    const wchar_t* arithmetics[] = {L"+", L"-", L"*", L"/", L"%", L"&+", L"&-", L"&*", L"&/", L"&%"};
    const wchar_t* bitwises[] = {L"|", L"&", L"^", L"<<", L">>"};
    TypePtr integers[] = {Int, UInt, Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64};
    TypePtr numbers[] = {Int, UInt, Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64, Float, Double};

    for(const TypePtr& type : integers)
    {
        t_Ints.push_back(type);
    }
    for(const TypePtr& type : numbers)
    {
        t_Numbers.push_back(type);
    }
    for(const wchar_t* arithmetic : arithmetics)
    {
        std::wstring op = arithmetic;
        for(const TypePtr& type : numbers)
        {
            registerOperatorFunction(op, type, type, type);
        }
    }
    for(const wchar_t* bitwise : bitwises)
    {
        std::wstring op = bitwise;
        for(const TypePtr& type : integers)
        {
            registerOperatorFunction(op, type, type, type);
        }
    }

    // T? == nil
    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        TypePtr optionalT = makeOptional(Type::newType(L"T", Type::Alias));
        GenericDefinitionPtr def(new GenericDefinition());
        def->add(L"T", T);
        vector<Type::Parameter> parameterTypes = {optionalT, _OptionalNilComparisonType};
        TypePtr funcType = Type::newFunction(parameterTypes, Bool, false, def);
        FunctionSymbolPtr func(new FunctionSymbol(L"==", funcType, nullptr));

        FunctionOverloadedSymbolPtr overloads(new FunctionOverloadedSymbol(L"=="));
        overloads->add(func);
        this->addSymbol(overloads);
    }
}

void GlobalScope::initProtocols()
{
}

bool GlobalScope::registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& lhs, const TypePtr& rhs)
{
    SymbolPtr sym = lookup(name);
    std::vector<Type::Parameter> parameterTypes = {lhs, rhs};
    TypePtr funcType = Type::newFunction(parameterTypes, returnType, false);
    FunctionSymbolPtr func(new FunctionSymbol(name, funcType, nullptr));

    if(sym)
    {
        FunctionOverloadedSymbolPtr overloadedSymbol = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym);
        if(!overloadedSymbol)
        {
            FunctionSymbolPtr old = std::static_pointer_cast<FunctionSymbol>(sym);
            overloadedSymbol = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(name));
            //replace old function symbol with the overloaded symbol
            removeSymbol(sym);
            addSymbol(overloadedSymbol);
            overloadedSymbol->add(old);
        }
        overloadedSymbol->add(func);
        return true;
    }
    addSymbol(func);
    return true;
}

static TypePtr innerType(const TypePtr& type)
{
    if(type->getCategory() == Type::Specialized)
        return type->getInnerType();
    return nullptr;
}

bool GlobalScope::isArray(const TypePtr& type) const
{
    return innerType(type) == Array;
}
bool GlobalScope::isOptional(const TypePtr& type) const
{
    return innerType(type) == Optional;
}
bool GlobalScope::isDictionary(const TypePtr& type) const
{
    return innerType(type) == Dictionary;
}
