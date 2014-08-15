#include "symbol-registry.h"
#include "symbol-scope.h"
#include "type.h"
#include "function-overloaded-symbol.h"
#include "function-symbol.h"

using namespace Swift;

SymbolRegistry::SymbolRegistry()
:currentScope(nullptr), fileScope(nullptr)
{
    enterScope(&topScope);
    //Assignment operator
    registerOperator(L"=", OperatorType::InfixBinary, Associativity::Right, 90);
    //Compound assignment operators
    registerOperator(L"+=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"-=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"*=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"/=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"%=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"<<=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L">>=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"&=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"|=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"&&=", OperatorType::InfixBinary, Associativity::Right, 90);
    registerOperator(L"||=", OperatorType::InfixBinary, Associativity::Right, 90);

    //Arithmetic operators
    registerOperator(L"+", OperatorType::InfixBinary, Associativity::Left, 140);
    registerOperator(L"-", OperatorType::InfixBinary, Associativity::Left, 140);
    registerOperator(L"*", OperatorType::InfixBinary, Associativity::Left, 150);
    registerOperator(L"/", OperatorType::InfixBinary, Associativity::Left, 150);
    //remainder operator
    registerOperator(L"%", OperatorType::InfixBinary, Associativity::Left, 150);
    //Increment and decrement operators
    registerOperator(L"++", (OperatorType::T)(OperatorType::PostfixUnary | OperatorType::PrefixUnary));
    registerOperator(L"--", (OperatorType::T)(OperatorType::PostfixUnary | OperatorType::PrefixUnary));
    //Unary minus operator
    registerOperator(L"-", OperatorType::PrefixUnary, Associativity::None, 100);
    registerOperator(L"+", OperatorType::PrefixUnary, Associativity::None, 100);
    //Comparison operators
    registerOperator(L"==", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"!=", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"===", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"!==", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"!===", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L">", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"<", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"<=", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L">=", OperatorType::InfixBinary, Associativity::None, 130);
    registerOperator(L"~=", OperatorType::InfixBinary, Associativity::None, 130);
    //Range operator
    registerOperator(L"..", OperatorType::InfixBinary, Associativity::None, 135);
    registerOperator(L"...", OperatorType::InfixBinary, Associativity::None, 135);
    //Cast operator
    registerOperator(L"is", OperatorType::InfixBinary, Associativity::None, 132);
    registerOperator(L"as", OperatorType::InfixBinary, Associativity::None, 132);
    //Logical operator
    registerOperator(L"!", OperatorType::InfixBinary, Associativity::None, 100);
    registerOperator(L"&&", OperatorType::InfixBinary, Associativity::Left, 120);
    registerOperator(L"||", OperatorType::InfixBinary, Associativity::Left, 110);
    //Bitwise operator
    registerOperator(L"~", OperatorType::InfixBinary, Associativity::None, 100);
    registerOperator(L"&", OperatorType::InfixBinary, Associativity::Left, 150);
    registerOperator(L"|", OperatorType::InfixBinary, Associativity::Left, 140);
    registerOperator(L"^", OperatorType::InfixBinary, Associativity::Left, 140);
    registerOperator(L"<<", OperatorType::InfixBinary, Associativity::None, 160);
    registerOperator(L">>", OperatorType::InfixBinary, Associativity::None, 160);
    //Overflow operators
    registerOperator(L"&+", OperatorType::InfixBinary, Associativity::Left, 140);
    registerOperator(L"&-", OperatorType::InfixBinary, Associativity::Left, 140);
    registerOperator(L"&*", OperatorType::InfixBinary, Associativity::Left, 150);
    registerOperator(L"&/", OperatorType::InfixBinary, Associativity::Left, 150);
    registerOperator(L"&%", OperatorType::InfixBinary, Associativity::Left, 150);
    

    //?:  Right associative, precedence level 100

    //Register built-in type
    TypePtr t_int8, t_uint8, t_int16, t_uint16, t_int32, t_uint32, t_int64, t_uint64, t_int, t_uint;
    TypePtr t_bool, t_float, t_double, t_string, t_void;
    currentScope->addSymbol(t_int8 = Type::newType(L"Int8", Type::Aggregate));
    currentScope->addSymbol(t_uint8 = Type::newType(L"UInt8", Type::Aggregate));
    currentScope->addSymbol(t_int16 = Type::newType(L"Int16", Type::Aggregate));
    currentScope->addSymbol(t_uint16 = Type::newType(L"UInt16", Type::Aggregate));
    currentScope->addSymbol(t_int32 = Type::newType(L"Int32", Type::Aggregate));
    currentScope->addSymbol(t_uint32 = Type::newType(L"UInt32", Type::Aggregate));
    currentScope->addSymbol(t_int64 = Type::newType(L"Int64", Type::Aggregate));
    currentScope->addSymbol(t_uint64 = Type::newType(L"UInt64", Type::Aggregate));
    currentScope->addSymbol(t_int = Type::newType(L"Int", Type::Aggregate));
    currentScope->addSymbol(t_uint = Type::newType(L"UInt", Type::Aggregate));
    currentScope->addSymbol(t_bool = Type::newType(L"Bool", Type::Aggregate));
    currentScope->addSymbol(t_float = Type::newType(L"Float", Type::Aggregate));
    currentScope->addSymbol(t_double = Type::newType(L"Double", Type::Aggregate));
    currentScope->addSymbol(t_string = Type::newType(L"String", Type::Aggregate));
    currentScope->addSymbol(t_void = Type::newType(L"Void", Type::Aggregate));
    //Register built-in variables
    currentScope->addSymbol(SymbolPtr(new SymbolPlaceHolder(L"true", t_bool, SymbolPlaceHolder::F_INITIALIZED)));
    currentScope->addSymbol(SymbolPtr(new SymbolPlaceHolder(L"false", t_bool, SymbolPlaceHolder::F_INITIALIZED)));



    //Register built-in functions
    //Register built-in operators
    const wchar_t* arithmetics[] = {L"+", L"-", L"*", L"/", L"%", L"&+", L"&-", L"&*", L"&/", L"&%"};
    const wchar_t* bitwises[] = {L"|", L"&", L"^", L"<<", L">>"};
    TypePtr integers[] = {t_int, t_uint, t_int8, t_uint8, t_int16, t_uint16, t_int32, t_uint32, t_int64, t_uint64};
    TypePtr numbers[] = {t_int, t_uint, t_int8, t_uint8, t_int16, t_uint16, t_int32, t_uint32, t_int64, t_uint64, t_float, t_double};
    for(const wchar_t* arithmetic : arithmetics)
    {
        std::wstring op = arithmetic;
        for(const TypePtr& type : numbers)
        {
            registerOperatorFunction(currentScope, op, type, type, type);
        }
    }
    for(const wchar_t* bitwise : bitwises)
    {
        std::wstring op = bitwise;
        for(const TypePtr& type : integers)
        {
            registerOperatorFunction(currentScope, op, type, type, type);
        }
    }

}

bool SymbolRegistry::registerOperatorFunction(SymbolScope* scope, const std::wstring& name, const TypePtr& returnType, const TypePtr& lhs, const TypePtr& rhs)
{
    SymbolPtr sym = scope->lookup(name);
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
            scope->removeSymbol(sym);
            scope->addSymbol(overloadedSymbol);
            overloadedSymbol->add(old);
        }
        overloadedSymbol->add(func);
        return true;
    }
    scope->addSymbol(func);
    return true;
}

bool SymbolRegistry::registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity, int precedence)
{
    return registerOperator(currentScope, name, type, associativity, precedence);
}
bool SymbolRegistry::registerOperator(SymbolScope* scope, const std::wstring& name, OperatorType::T type, Associativity::T associativity, int precedence)
{
    SymbolScope::OperatorMap::iterator iter = scope->operators.find(name);
    if(iter == scope->operators.end())
    {
        iter = scope->operators.insert(std::make_pair(name, OperatorInfo(name, associativity))).first;
    }
    else
    {
        if((iter->second.type & type) != 0)
            return false;//operator redeclared
    }

    iter->second.type = (OperatorType::T)(iter->second.type | type);
    
    if(type & OperatorType::PrefixUnary)
        iter->second.precedence.prefix = precedence;
    if(type & OperatorType::InfixBinary)
        iter->second.precedence.infix = precedence;
    if(type & OperatorType::PostfixUnary)
        iter->second.precedence.postfix = precedence;
    return true;
}
OperatorInfo* SymbolRegistry::getOperator(const std::wstring& name)
{
    OperatorInfo* ret = NULL;
    if(fileScope)
    {
        ret = getOperator(fileScope, name);
    }
    if(!ret)
        ret = getOperator(&topScope, name);
    return ret;
}
OperatorInfo* SymbolRegistry::getOperator(SymbolScope* scope, const std::wstring& name)
{
    SymbolScope::OperatorMap::iterator iter = scope->operators.find(name);
    if(iter == currentScope->operators.end())
        return NULL;
    return &iter->second;
}
bool SymbolRegistry::isPrefixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name);
    return op && ((op->type & OperatorType::PrefixUnary) != 0);
}
bool SymbolRegistry::isPostfixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name);
    return op && ((op->type & OperatorType::PostfixUnary) != 0);
}
bool SymbolRegistry::isInfixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name);
    return op && ((op->type & OperatorType::InfixBinary) != 0);
}


SymbolScope* SymbolRegistry::getFileScope()
{
    return fileScope;
}
void SymbolRegistry::setFileScope(SymbolScope* scope)
{
    fileScope = scope;
}

SymbolScope* SymbolRegistry::getCurrentScope()
{
    return currentScope;
}
void SymbolRegistry::enterScope(SymbolScope* scope)
{
    scopes.push(currentScope);
    scope->parent = currentScope;
    currentScope = scope;
}
void SymbolRegistry::leaveScope()
{
    currentScope = scopes.top();
    scopes.pop();
}

SymbolPtr SymbolRegistry::lookupSymbol(const std::wstring& name)
{
    SymbolPtr ret = NULL;
    lookupSymbol(name, NULL, &ret);
    return ret;
}
bool SymbolRegistry::lookupSymbol(const std::wstring& name, SymbolScope** container, SymbolPtr* ret)
{
    return lookupSymbol(currentScope, name, container, ret);
}
bool SymbolRegistry::lookupSymbol(SymbolScope* scope, const std::wstring& name, SymbolScope** container, SymbolPtr* ret)
{
    SymbolScope* s = scope;
    for(; s; s = s->parent)
    {
        SymbolPtr symbol = s->lookup(name);
        if(symbol)
        {
            if(container)
                *container = s;
            if(ret)
                *ret = symbol;
            return true;
        }
    }
    return false;
}
TypePtr SymbolRegistry::lookupType(const std::wstring& name)
{
    TypePtr ret;
    if(lookupType(name, NULL, &ret))
        return ret;
    return NULL;
}
bool SymbolRegistry::lookupType(const std::wstring& name, SymbolScope** scope, TypePtr* ret)
{
    SymbolPtr symbol = NULL;
    bool r = lookupSymbol(name, scope, &symbol);
    if(ret)
        *ret = std::dynamic_pointer_cast<Type>(symbol);
    return r;
}