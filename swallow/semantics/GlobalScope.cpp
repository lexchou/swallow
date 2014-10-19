#include "GlobalScope.h"
#include "Type.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include "GenericDefinition.h"
#include "TypeBuilder.h"

USE_SWIFT_NS
using namespace std;

GlobalScope::GlobalScope()
{
    initPrimitiveTypes();
    initOperators();
    initProtocols();
}
void GlobalScope::initPrimitiveTypes()
{
    addSymbol(t_Int8 = Type::newType(L"Int8", Type::Aggregate));
    addSymbol(t_UInt8 = Type::newType(L"UInt8", Type::Aggregate));
    addSymbol(t_Int16 = Type::newType(L"Int16", Type::Aggregate));
    addSymbol(t_UInt16 = Type::newType(L"UInt16", Type::Aggregate));
    addSymbol(t_Int32 = Type::newType(L"Int32", Type::Aggregate));
    addSymbol(t_UInt32 = Type::newType(L"UInt32", Type::Aggregate));
    addSymbol(t_Int64 = Type::newType(L"Int64", Type::Aggregate));
    addSymbol(t_UInt64 = Type::newType(L"UInt64", Type::Aggregate));
    addSymbol(t_Int = Type::newType(L"Int", Type::Aggregate));
    addSymbol(t_UInt = Type::newType(L"UInt", Type::Aggregate));
    addSymbol(t_Bool = Type::newType(L"Bool", Type::Aggregate));
    addSymbol(t_Float = Type::newType(L"Float", Type::Aggregate));
    addSymbol(t_Double = Type::newType(L"Double", Type::Aggregate));
    addSymbol(t_String = Type::newType(L"String", Type::Aggregate));
    addSymbol(L"Void", t_Void = Type::newTuple(std::vector<TypePtr>()));

//Register built-in variables
    addSymbol(s_True = SymbolPtr(new SymbolPlaceHolder(L"true", t_Bool, SymbolPlaceHolder::R_LOCAL_VARIABLE, SymbolPlaceHolder::F_INITIALIZED)));
    addSymbol(s_False = SymbolPtr(new SymbolPlaceHolder(L"false", t_Bool, SymbolPlaceHolder::R_LOCAL_VARIABLE, SymbolPlaceHolder::F_INITIALIZED)));
    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        t_Array = Type::newType(L"Array", Type::Struct, nullptr, nullptr, std::vector<TypePtr>(), generic);
        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(t_Array);
        {
            std::vector<Type::Parameter> params = {Type::Parameter(T)};
            TypePtr t_append = Type::newFunction(params, t_Void, false, nullptr);

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
            SymbolPlaceHolderPtr count(new SymbolPlaceHolder(L"count", t_Int, SymbolPlaceHolder::R_PROPERTY, SymbolPlaceHolder::F_INITIALIZED | SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_MEMBER));
            builder->addMember(count);
        }
        {
            std::vector<Type::Parameter> params = {Type::Parameter(t_Int)};
            TypePtr t_append = Type::newFunction(params, T, false, nullptr);

            FunctionSymbolPtr subscript(new FunctionSymbol(L"subscript", t_append, nullptr));
            FunctionOverloadedSymbolPtr subscripts(new FunctionOverloadedSymbol(L"subscript"));
            subscripts->add(subscript);
            builder->addMember(subscripts);
        }
        addSymbol(t_Array);
    }
    {
        TypePtr T = Type::newType(L"T", Type::GenericParameter);
        GenericDefinitionPtr generic(new GenericDefinition());
        generic->add(L"T", T);
        t_Optional = Type::newType(L"Optional", Type::Struct, nullptr, nullptr, std::vector<TypePtr>(), generic);
        addSymbol(t_Optional);
    }
}

void GlobalScope::initOperators()
{
    //Register built-in functions
    //Register built-in operators
    const wchar_t* arithmetics[] = {L"+", L"-", L"*", L"/", L"%", L"&+", L"&-", L"&*", L"&/", L"&%"};
    const wchar_t* bitwises[] = {L"|", L"&", L"^", L"<<", L">>"};
    TypePtr integers[] = {t_Int, t_UInt, t_Int8, t_UInt8, t_Int16, t_UInt16, t_Int32, t_UInt32, t_Int64, t_UInt64};
    TypePtr numbers[] = {t_Int, t_UInt, t_Int8, t_UInt8, t_Int16, t_UInt16, t_Int32, t_UInt32, t_Int64, t_UInt64, t_Float, t_Double};

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
}

void GlobalScope::initProtocols()
{
    t_Equatable = Type::newType(L"Equatable", Type::Protocol, nullptr);
    addSymbol(t_Equatable);
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
