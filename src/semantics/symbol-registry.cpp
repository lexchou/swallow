#include "symbol-registry.h"
#include "symbol-scope.h"
#include "type.h"
using namespace Swift;

SymbolRegistry::SymbolRegistry()
:currentScope(NULL)
{
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

    enterScope(&topScope);
    //Register built-in type
    currentScope->addSymbol(Type::newType(L"Int", Type::Primitive));
    currentScope->addSymbol(Type::newType(L"Bool", Type::Primitive));
    currentScope->addSymbol(Type::newType(L"Float", Type::Primitive));
    currentScope->addSymbol(Type::newType(L"Double", Type::Primitive));
    currentScope->addSymbol(Type::newType(L"String", Type::Primitive));
}

bool SymbolRegistry::registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity, int precedence)
{
    OperatorMap::iterator iter = operators.find(name);
    if(iter == operators.end())
    {
        iter = operators.insert(std::make_pair(name, OperatorInfo(name, associativity))).first;
    }
    
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
    OperatorMap::iterator iter = operators.find(name);
    if(iter == operators.end())
        return NULL;
    return &iter->second;
}
bool SymbolRegistry::isPrefixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name);
    return op && (op->precedence.prefix > 0);
}
bool SymbolRegistry::isPostfixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name);
    return op && (op->precedence.postfix > 0);
}
bool SymbolRegistry::isInfixOperator(const std::wstring& name)
{
    OperatorInfo* op = getOperator(name);
    return op && (op->precedence.infix > 0);
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
bool SymbolRegistry::lookupSymbol(const std::wstring& name, SymbolScope** scope, SymbolPtr* ret)
{
    SymbolScope* s = currentScope;
    for(; s; s = s->parent)
    {
        SymbolPtr symbol = s->lookup(name);
        if(symbol)
        {
            if(scope)
                *scope = s;
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