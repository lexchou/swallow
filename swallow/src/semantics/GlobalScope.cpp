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
#include "semantics/GlobalScope.h"
#include "semantics/Type.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/TypeBuilder.h"
#include <cstdarg>
#include <cassert>
#include "semantics/SymbolRegistry.h"

//#define USE_RUNTIME_FILE

#ifdef USE_RUNTIME_FILE
#include <iostream>
#include "ScopedNodeFactory.h"
#include "parser/Parser.h"
#include "common/CompilerResults.h"
#include "SemanticAnalyzer.h"
#include "ScopedNodes.h"
#include "common/SwallowUtils.h"
#endif

USE_SWALLOW_NS
using namespace std;




GlobalScope::GlobalScope()
{


}



void GlobalScope::initRuntime(SymbolRegistry* symbolRegistry)
{
#ifdef USE_RUNTIME_FILE

    //cout<<"Loading runtime file"<<endl;
    wstring str = SwallowUtils::readFile("runtime.swift");
    ScopedNodeFactory nodeFactory;
    CompilerResults compilerResults;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ScopedProgramPtr ret(new ScopedProgram());
    ret->setScope(this);

    try
    {
        if(!parser.parse(str.c_str(), ret))
            throw Abort();
        SemanticAnalyzer analyzer(symbolRegistry, &compilerResults);
        ret->accept(&analyzer);
    }
    catch(Abort&)
    {
        ret->setScope(nullptr);
        SwallowUtils::dumpCompilerResults(str, compilerResults, std::wcout);
        assert(0 && "Failed to load runtime.swift");
    }
    ret->setScope(nullptr);

    #define VALIDATE_TYPE(T) assert(T() && #T " is not defined.");

    VALIDATE_TYPE(Bool);
    VALIDATE_TYPE(Void);
    VALIDATE_TYPE(String);
    VALIDATE_TYPE(Character);
    VALIDATE_TYPE(Int);
    VALIDATE_TYPE(UInt);
    VALIDATE_TYPE(Int8);
    VALIDATE_TYPE(UInt8);
    VALIDATE_TYPE(Int16);
    VALIDATE_TYPE(UInt16);
    VALIDATE_TYPE(Int32);
    VALIDATE_TYPE(UInt32);
    VALIDATE_TYPE(Int64);
    VALIDATE_TYPE(UInt64);
    VALIDATE_TYPE(Float);
    VALIDATE_TYPE(Double);
    VALIDATE_TYPE(Array);
    VALIDATE_TYPE(Dictionary);
    VALIDATE_TYPE(Optional);
    VALIDATE_TYPE(BooleanType);
    VALIDATE_TYPE(Equatable);
    VALIDATE_TYPE(Comparable);
    VALIDATE_TYPE(Hashable);
    VALIDATE_TYPE(RawRepresentable);
    VALIDATE_TYPE(_OptionalNilComparisonType);
    VALIDATE_TYPE(_IntegerType);
    VALIDATE_TYPE(UnsignedIntegerType);
    VALIDATE_TYPE(SignedIntegerType);
    VALIDATE_TYPE(FloatingPointType);
    VALIDATE_TYPE(StringInterpolationConvertible);
    VALIDATE_TYPE(IntegerLiteralConvertible);
    VALIDATE_TYPE(BooleanLiteralConvertible);
    VALIDATE_TYPE(StringLiteralConvertible);
    VALIDATE_TYPE(FloatLiteralConvertible);
    VALIDATE_TYPE(NilLiteralConvertible);
    VALIDATE_TYPE(ArrayLiteralConvertible);
    VALIDATE_TYPE(DictionaryLiteralConvertible);
    VALIDATE_TYPE(UnicodeScalarLiteralConvertible);
    VALIDATE_TYPE(ExtendedGraphemeClusterLiteralConvertible);
    VALIDATE_TYPE(SequenceType);
    VALIDATE_TYPE(CollectionType);

    //cout<<"Runtime file loaded."<<endl;
#else
    initPrimitiveTypes();
    initOperators(symbolRegistry);
    initProtocols();
#endif

}




void GlobalScope::initPrimitiveTypes()
{
    //Protocols
    TypeBuilderPtr type;
    std::wstring moduleName = L"Swift";
    #define DECLARE_TYPE(T, N)  _##N = type = static_pointer_cast<TypeBuilder>(Type::newType(L###N, Type::T)); addSymbol(_##N); type->setAccessLevel(AccessLevelPublic); type->setModuleName(moduleName);
    #define ALIAS(NewType, OldType) addSymbol(L###NewType, _##OldType);
    #define IMPLEMENTS(P) type->addProtocol(_##P);

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

        ComputedPropertySymbolPtr rawValue(new ComputedPropertySymbol(L"rawValue", RawValue, SymbolFlagMember | SymbolFlagReadable));
        type->addMember(rawValue);
    }


    DECLARE_TYPE(Protocol, SequenceType);

    DECLARE_TYPE(Protocol, CollectionType);
    IMPLEMENTS(SequenceType);

    DECLARE_TYPE(Protocol, _IntegerType);
    IMPLEMENTS(IntegerLiteralConvertible);
    IMPLEMENTS(Hashable);

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
    IMPLEMENTS(Hashable);

    DECLARE_TYPE(Protocol, StringInterpolationConvertible);




    //Types
    DECLARE_TYPE(Struct, Int8);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Struct, UInt8);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Struct, Int16);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Struct, UInt16);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Struct, Int32);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Struct, UInt32);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Struct, Int64);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Struct, UInt64);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Struct, Int);
    IMPLEMENTS(SignedIntegerType);

    DECLARE_TYPE(Struct, UInt);
    IMPLEMENTS(UnsignedIntegerType);

    DECLARE_TYPE(Struct, _OptionalNilComparisonType);
    IMPLEMENTS(NilLiteralConvertible);


    DECLARE_TYPE(Struct, Bool);
    IMPLEMENTS(BooleanType);
    IMPLEMENTS(BooleanLiteralConvertible);
    IMPLEMENTS(Equatable)
    IMPLEMENTS(Hashable)

    DECLARE_TYPE(Struct, Float);
    IMPLEMENTS(FloatingPointType);
    IMPLEMENTS(IntegerLiteralConvertible);
    IMPLEMENTS(FloatLiteralConvertible);

    DECLARE_TYPE(Struct, Float80);
    IMPLEMENTS(FloatingPointType);
    IMPLEMENTS(IntegerLiteralConvertible);
    IMPLEMENTS(FloatLiteralConvertible);

    DECLARE_TYPE(Struct, Double);
    IMPLEMENTS(FloatingPointType);
    IMPLEMENTS(IntegerLiteralConvertible);
    IMPLEMENTS(FloatLiteralConvertible);

    ALIAS(Float32, Float);
    ALIAS(Float64, Double);

    DECLARE_TYPE(Protocol, Printable);
    DECLARE_TYPE(Protocol, DebugPrintable);

    DECLARE_TYPE(Struct, String);
    IMPLEMENTS(StringLiteralConvertible);
    IMPLEMENTS(UnicodeScalarLiteralConvertible);
    IMPLEMENTS(ExtendedGraphemeClusterLiteralConvertible);
    IMPLEMENTS(Hashable);
    IMPLEMENTS(Equatable);
    IMPLEMENTS(StringInterpolationConvertible);
    {
        {
            std::vector<Parameter> params = {Parameter(_String)};
            TypePtr t_hasPrefix = Type::newFunction(params, _Bool, false, nullptr);

            FunctionSymbolPtr hasPrefix(new FunctionSymbol(L"hasPrefix",  t_hasPrefix, FunctionRoleNormal, nullptr));
            type->addMember(hasPrefix);
        }

        {
            ComputedPropertySymbolPtr isEmpty(new ComputedPropertySymbol(L"isEmpty", _Bool, SymbolFlagMember | SymbolFlagReadable));
            type->addMember(isEmpty);
        }
    }



    DECLARE_TYPE(Enum, Character);
    IMPLEMENTS(ExtendedGraphemeClusterLiteralConvertible);
    IMPLEMENTS(Equatable);
    IMPLEMENTS(Hashable);
    IMPLEMENTS(Comparable);


    addSymbol(L"Void", _Void = Type::newTuple(vector<TypePtr>()));

    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        _ImplicitlyUnwrappedOptional = Type::newType(L"ImplicitlyUnwrappedOptional", Type::Enum, nullptr, nullptr, std::vector<TypePtr>(), generic);
        type = static_pointer_cast<TypeBuilder>(_ImplicitlyUnwrappedOptional);
        type->setModuleName(moduleName);
        type->addProtocol(_NilLiteralConvertible);

        type->addEnumCase(L"None", _Void);
        std::vector<TypePtr> SomeArgs = {T};
        type->addEnumCase(L"Some", Type::newTuple(SomeArgs));
        addSymbol(type);
    }

    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        _Optional = Type::newType(L"Optional", Type::Enum, nullptr, nullptr, std::vector<TypePtr>(), generic);
        type = static_pointer_cast<TypeBuilder>(_Optional);
        type->setModuleName(moduleName);
        type->addProtocol(_NilLiteralConvertible);

        type->addEnumCase(L"None", _Void);
        std::vector<TypePtr> SomeArgs = {T};
        type->addEnumCase(L"Some", Type::newTuple(SomeArgs));
        addSymbol(_Optional);
    }
    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        _Array = Type::newType(L"Array", Type::Struct, nullptr, nullptr, std::vector<TypePtr>(), generic);
        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(_Array);
        builder->setModuleName(moduleName);
        builder->addProtocol(_CollectionType);
        {
            std::vector<Parameter> params = {Parameter(T)};
            TypePtr t_append = Type::newFunction(params, _Void, false, nullptr);

            FunctionSymbolPtr append(new FunctionSymbol(L"append", t_append, FunctionRoleNormal, nullptr));
            builder->addMember(append);
        }
        {
            std::vector<Parameter> params = {};
            TypePtr t_append = Type::newFunction(params, T, false, nullptr);

            FunctionSymbolPtr append(new FunctionSymbol(L"removeLast", t_append, FunctionRoleNormal, nullptr));
            builder->addMember(append);
        }
        {
            ComputedPropertySymbolPtr count(new ComputedPropertySymbol(L"count", _Int, SymbolFlagMember | SymbolFlagReadable));
            builder->addMember(count);
        }
        {
            std::vector<Parameter> getterParams = {Parameter(_Int)};
            TypePtr getterType = Type::newFunction(getterParams, T, false, nullptr);
            std::vector<Parameter> setterParams = {Parameter(_Int), Parameter(T)};
            TypePtr setterType = Type::newFunction(setterParams, _Void, false, nullptr);

            FunctionSymbolPtr getter(new FunctionSymbol(L"subscript", getterType, FunctionRoleGetter, nullptr));
            FunctionSymbolPtr setter(new FunctionSymbol(L"subscript", setterType, FunctionRoleSetter, nullptr));
            FunctionOverloadedSymbolPtr subscripts(new FunctionOverloadedSymbol(L"subscript"));
            subscripts->add(getter);
            subscripts->add(setter);
            builder->addMember(subscripts);
        }
        addSymbol(_Array);
    }

    {
        TypePtr Key = Type::newType(L"Key", Type::GenericParameter);
        TypePtr Value = Type::newType(L"Value", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"Key", Key);
        generic->add(L"Value", Value);
        _Dictionary = Type::newType(L"Dictionary", Type::Struct, nullptr, nullptr, std::vector<TypePtr>(), generic);
        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(_Dictionary);
        builder->setModuleName(moduleName);
        builder->addProtocol(_CollectionType);
        builder->addProtocol(_DictionaryLiteralConvertible);
        {
            ComputedPropertySymbolPtr count(new ComputedPropertySymbol(L"count", _Int, SymbolFlagMember | SymbolFlagReadable));
            builder->addMember(count);
        }
        {
            ComputedPropertySymbolPtr isEmpty(new ComputedPropertySymbol(L"isEmpty", _Bool, SymbolFlagMember | SymbolFlagReadable));
            builder->addMember(isEmpty);
        }
        {
            std::vector<Parameter> getterParams = {Parameter(Key)};
            TypePtr getter = Type::newFunction(getterParams, makeOptional(Value), false, nullptr);

            std::vector<Parameter> setterParams = {Parameter(Key), Parameter(makeOptional(Value))};
            TypePtr setter = Type::newFunction(setterParams, _Void, false, nullptr);

            FunctionSymbolPtr subscriptGet(new FunctionSymbol(L"subscript", getter, FunctionRoleGetter, nullptr));
            FunctionSymbolPtr subscriptSet(new FunctionSymbol(L"subscript", setter, FunctionRoleSetter, nullptr));


            FunctionOverloadedSymbolPtr subscripts(new FunctionOverloadedSymbol(L"subscript"));
            subscripts->add(subscriptGet);
            subscripts->add(subscriptSet);
            builder->addMember(subscripts);
        }
        addSymbol(_Dictionary);
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
    std::vector<Parameter> params;
    TypePtr retType = dynamic_pointer_cast<Type>(lookup(returnType));
    bool variadicParams = false;
    assert(retType != nullptr);
    while(true)
    {
        const wchar_t* typeName = va_arg(va, const wchar_t*);
        bool inout = false;
        if(!typeName)
            break;
        if(*typeName == '&')
        {
            inout = true;
            typeName++;
        }
        TypePtr paramType = dynamic_pointer_cast<Type>(lookup(typeName));
        assert(paramType != nullptr);
        params.push_back(Parameter(L"", inout, paramType));
    }
    TypePtr funcType = Type::newFunction(params, retType, variadicParams, nullptr);
    FunctionSymbolPtr ret(new FunctionSymbol(name, funcType, FunctionRoleNormal, nullptr));
    ret->setFlags(flags);
    return ret;
}

TypePtr GlobalScope::makeArray(const TypePtr& elementType) const
{
    GenericArgumentPtr ga(new GenericArgument(_Array->getGenericDefinition()));
    ga->add(elementType);
    return Type::newSpecializedType(_Array, ga);
}

/*!
 * A short-hand way to create an Optional type
 */
TypePtr GlobalScope::makeOptional(const TypePtr& elementType) const
{
    assert(elementType != nullptr);
    GenericArgumentPtr ga(new GenericArgument(_Optional->getGenericDefinition()));
    ga->add(elementType);
    return Type::newSpecializedType(_Optional, ga);
}
/*!
 * A short-hand way to create an implicitly unwrapped Optional type
 */
TypePtr GlobalScope::makeImplicitlyUnwrappedOptional(const TypePtr& elementType) const
{
    assert(elementType != nullptr);
    GenericArgumentPtr ga(new GenericArgument(_Optional->getGenericDefinition()));
    ga->add(elementType);
    return Type::newSpecializedType(_ImplicitlyUnwrappedOptional, ga);
}

/*!
 * A short-hand way to create a Dictionary type
 */
TypePtr GlobalScope::makeDictionary(const TypePtr& keyType, const TypePtr& valueType) const
{
    GenericArgumentPtr ga(new GenericArgument(_Dictionary->getGenericDefinition()));
    ga->add(keyType);
    ga->add(valueType);
    return Type::newSpecializedType(_Dictionary, ga);
}

void GlobalScope::initOperators(SymbolRegistry* registry)
{
    //Register built-in functions
    //Register built-in operators
    const wchar_t* arithmetics[] = {L"+", L"-", L"*", L"/", L"%", L"&+", L"&-", L"&*", L"&/", L"&%"};
    const wchar_t* bitwises[] = {L"|", L"&", L"^", L"<<", L">>"};
    const wchar_t* unaries[] = {L"-", L"+"};
    const wchar_t* logics[] = {L"||", L"&&"};
    const wchar_t* comparisons[] = {L"==", L"!=", L"<", L">", L">=", L"<="};
    TypePtr integers[] = {_Int, _UInt, _Int8, _UInt8, _Int16, _UInt16, _Int32, _UInt32, _Int64, _UInt64};
    TypePtr numbers[] = {_Int, _UInt, _Int8, _UInt8, _Int16, _UInt16, _Int32, _UInt32, _Int64, _UInt64, _Float, _Double};

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

    for(const wchar_t* comparison : comparisons)
    {
        std::wstring op = comparison;
        for(const TypePtr& type : numbers)
        {
            registerOperatorFunction(op, _Bool, type, type);
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

    for(const wchar_t* logic : logics)
    {
        registerOperatorFunction(logic, _Bool, _Bool, _Bool);
    }
    registerOperatorFunction(L"!", _Bool, _Bool, OperatorType::PrefixUnary);
    for(const wchar_t* unary : unaries)
    {
        for(const TypePtr& type : numbers)
        {
            std::vector<Parameter> params = {Parameter(type)};
            TypePtr funcType = Type::newFunction(params, type, false, nullptr);
            FunctionSymbolPtr func(new FunctionSymbol(unary, funcType, FunctionRoleOperator, nullptr));
            func->setFlags(SymbolFlagPostfix, true);
            registerFunction(unary, func);

            funcType = Type::newFunction(params, type, false, nullptr);
            func = FunctionSymbolPtr(new FunctionSymbol(unary, funcType, FunctionRoleOperator, nullptr));
            func->setFlags(SymbolFlagPrefix, true);
            registerFunction(unary, func);
        }
    }

    this->declareFunction(L"++", SymbolFlagPostfix, L"Int", L"&Int", NULL);

    // T? == nil
    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        TypePtr optionalT = makeOptional(Type::newType(L"T", Type::Alias));
        GenericDefinitionPtr def(new GenericDefinition());
        def->add(L"T", T);
        vector<Parameter> parameterTypes = {optionalT, __OptionalNilComparisonType};
        TypePtr funcType = Type::newFunction(parameterTypes, _Bool, false, def);
        FunctionSymbolPtr func(new FunctionSymbol(L"==", funcType, FunctionRoleOperator, nullptr));
        func->setFlags(SymbolFlagInfix);

        FunctionOverloadedSymbolPtr overloads = static_pointer_cast<FunctionOverloadedSymbol>(this->lookup(L"=="));
        overloads->add(func);
    }

    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        TypePtr optionalT = makeOptional(Type::newType(L"T", Type::Alias));
        GenericDefinitionPtr def(new GenericDefinition());
        def->add(L"T", T);
        vector<Parameter> parameterTypes = {optionalT, __OptionalNilComparisonType};
        TypePtr funcType = Type::newFunction(parameterTypes, _Bool, false, def);
        FunctionSymbolPtr func(new FunctionSymbol(L"!=", funcType, FunctionRoleOperator, nullptr));
        func->setFlags(SymbolFlagInfix);

        FunctionOverloadedSymbolPtr overloads = static_pointer_cast<FunctionOverloadedSymbol>(this->lookup(L"!="));
        overloads->add(func);
    }
    //Assignment operator
    registry->registerOperator(this, L"=", OperatorType::InfixBinary, Associativity::Right, 90);
    //Compound assignment operators
    registry->registerOperator(this, L"+=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"-=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"*=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"/=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"%=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"<<=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L">>=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"&=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"|=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"&&=", OperatorType::InfixBinary, Associativity::Right, 90);
    registry->registerOperator(this, L"||=", OperatorType::InfixBinary, Associativity::Right, 90);

    //Arithmetic operators
    registry->registerOperator(this, L"+", OperatorType::InfixBinary, Associativity::Left, 140);
    registry->registerOperator(this, L"-", OperatorType::InfixBinary, Associativity::Left, 140);
    registry->registerOperator(this, L"*", OperatorType::InfixBinary, Associativity::Left, 150);
    registry->registerOperator(this, L"/", OperatorType::InfixBinary, Associativity::Left, 150);
    //remainder operator
    registry->registerOperator(this, L"%", OperatorType::InfixBinary, Associativity::Left, 150);
    //Increment and decrement operators
    registry->registerOperator(this, L"++", (OperatorType::T)(OperatorType::PostfixUnary | OperatorType::PrefixUnary));
    registry->registerOperator(this, L"--", (OperatorType::T)(OperatorType::PostfixUnary | OperatorType::PrefixUnary));
    //Unary minus operator
    registry->registerOperator(this, L"-", OperatorType::PrefixUnary, Associativity::None, 100);
    registry->registerOperator(this, L"+", OperatorType::PrefixUnary, Associativity::None, 100);
    //Comparison operators
    registry->registerOperator(this, L"==", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"!=", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"===", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"!==", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"!===", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L">", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"<", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"<=", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L">=", OperatorType::InfixBinary, Associativity::None, 130);
    registry->registerOperator(this, L"~=", OperatorType::InfixBinary, Associativity::None, 130);
    //Range operator
    registry->registerOperator(this, L"..", OperatorType::InfixBinary, Associativity::None, 135);
    registry->registerOperator(this, L"...", OperatorType::InfixBinary, Associativity::None, 135);
    //Cast operator
    registry->registerOperator(this, L"is", OperatorType::InfixBinary, Associativity::None, 132);
    registry->registerOperator(this, L"as", OperatorType::InfixBinary, Associativity::None, 132);
    //Logical operator
    registry->registerOperator(this, L"!", OperatorType::InfixBinary, Associativity::None, 100);
    registry->registerOperator(this, L"&&", OperatorType::InfixBinary, Associativity::Left, 120);
    registry->registerOperator(this, L"||", OperatorType::InfixBinary, Associativity::Left, 110);
    //Bitwise operator
    registry->registerOperator(this, L"~", OperatorType::InfixBinary, Associativity::None, 100);
    registry->registerOperator(this, L"&", OperatorType::InfixBinary, Associativity::Left, 150);
    registry->registerOperator(this, L"|", OperatorType::InfixBinary, Associativity::Left, 140);
    registry->registerOperator(this, L"^", OperatorType::InfixBinary, Associativity::Left, 140);
    registry->registerOperator(this, L"<<", OperatorType::InfixBinary, Associativity::None, 160);
    registry->registerOperator(this, L">>", OperatorType::InfixBinary, Associativity::None, 160);
    //Overflow operators
    registry->registerOperator(this, L"&+", OperatorType::InfixBinary, Associativity::Left, 140);
    registry->registerOperator(this, L"&-", OperatorType::InfixBinary, Associativity::Left, 140);
    registry->registerOperator(this, L"&*", OperatorType::InfixBinary, Associativity::Left, 150);
    registry->registerOperator(this, L"&/", OperatorType::InfixBinary, Associativity::Left, 150);
    registry->registerOperator(this, L"&%", OperatorType::InfixBinary, Associativity::Left, 150);


}

void GlobalScope::initProtocols()
{
}

bool GlobalScope::registerFunction(const std::wstring& name, const FunctionSymbolPtr& func)
{
    SymbolPtr sym = lookup(name);
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
bool GlobalScope::registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& operand, OperatorType::T operatorType)
{
    std::vector<Parameter> parameterTypes = {operand};
    TypePtr funcType = Type::newFunction(parameterTypes, returnType, false);
    FunctionSymbolPtr func(new FunctionSymbol(name, funcType, FunctionRoleOperator, nullptr));
    switch(operatorType)
    {
        case OperatorType::PrefixUnary:
            func->setFlags(SymbolFlagPrefix);
            break;
        case OperatorType::PostfixUnary:
            func->setFlags(SymbolFlagPostfix);
            break;
        default:
            assert(0 && "Invalid unary operator to register");
            break;
    }
    registerFunction(name, func);
    return true;
}
bool GlobalScope::registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& lhs, const TypePtr& rhs)
{

    std::vector<Parameter> parameterTypes = {lhs, rhs};
    TypePtr funcType = Type::newFunction(parameterTypes, returnType, false);
    FunctionSymbolPtr func(new FunctionSymbol(name, funcType, FunctionRoleOperator, nullptr));
    func->setFlags(SymbolFlagInfix);

    registerFunction(name, func);
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
    return innerType(type) == _Array;
}
bool GlobalScope::isOptional(const TypePtr& type) const
{
    return innerType(type) == _Optional;
}
bool GlobalScope::isImplicitlyUnwrappedOptional(const TypePtr& type) const
{
    return innerType(type) == _ImplicitlyUnwrappedOptional;
}
bool GlobalScope::isDictionary(const TypePtr& type) const
{
    return innerType(type) == _Dictionary;
}

#ifdef USE_RUNTIME_FILE
#define IMPLEMENT_GETTER(T) TypePtr GlobalScope::T() const { \
  GlobalScope* self = const_cast<GlobalScope*>(this); \
  if(!self->_##T){self->_##T = static_pointer_cast<Type>(self->lookup(L## #T ));} \
  assert(_##T != nullptr); \
  return _##T; \
}
#else
#define IMPLEMENT_GETTER(T) TypePtr GlobalScope::T() const { return _##T;}
#endif


IMPLEMENT_GETTER(Bool);
IMPLEMENT_GETTER(Void);
IMPLEMENT_GETTER(String);
IMPLEMENT_GETTER(Character);
IMPLEMENT_GETTER(Int);
IMPLEMENT_GETTER(UInt);
IMPLEMENT_GETTER(Int8);
IMPLEMENT_GETTER(UInt8);
IMPLEMENT_GETTER(Int16);
IMPLEMENT_GETTER(UInt16);
IMPLEMENT_GETTER(Int32);
IMPLEMENT_GETTER(UInt32);
IMPLEMENT_GETTER(Int64);
IMPLEMENT_GETTER(UInt64);
IMPLEMENT_GETTER(Float);
IMPLEMENT_GETTER(Float80);
IMPLEMENT_GETTER(Double);
IMPLEMENT_GETTER(Array);
IMPLEMENT_GETTER(Dictionary);
IMPLEMENT_GETTER(Optional);
IMPLEMENT_GETTER(ImplicitlyUnwrappedOptional);
IMPLEMENT_GETTER(BooleanType);
IMPLEMENT_GETTER(Equatable);
IMPLEMENT_GETTER(Comparable);
IMPLEMENT_GETTER(Hashable);
IMPLEMENT_GETTER(RawRepresentable);
IMPLEMENT_GETTER(_OptionalNilComparisonType);
IMPLEMENT_GETTER(_IntegerType);
IMPLEMENT_GETTER(UnsignedIntegerType);
IMPLEMENT_GETTER(SignedIntegerType);
IMPLEMENT_GETTER(FloatingPointType);
IMPLEMENT_GETTER(StringInterpolationConvertible);
IMPLEMENT_GETTER(IntegerLiteralConvertible);
IMPLEMENT_GETTER(BooleanLiteralConvertible);
IMPLEMENT_GETTER(StringLiteralConvertible);
IMPLEMENT_GETTER(FloatLiteralConvertible);
IMPLEMENT_GETTER(NilLiteralConvertible);
IMPLEMENT_GETTER(ArrayLiteralConvertible);
IMPLEMENT_GETTER(DictionaryLiteralConvertible);
IMPLEMENT_GETTER(UnicodeScalarLiteralConvertible);
IMPLEMENT_GETTER(ExtendedGraphemeClusterLiteralConvertible);
IMPLEMENT_GETTER(SequenceType);
IMPLEMENT_GETTER(CollectionType);
IMPLEMENT_GETTER(Printable);
IMPLEMENT_GETTER(DebugPrintable);
