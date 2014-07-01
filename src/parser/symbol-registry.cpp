#include "symbol-registry.h"
using namespace Swift;

SymbolRegistry::SymbolRegistry()
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

