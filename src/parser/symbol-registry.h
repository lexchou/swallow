#ifndef SYMBOL_REGISTRY_H
#define SYMBOL_REGISTRY_H
#include "swift_conf.h"
#include "tokenizer/tokens.h"
#include "swift_types.h"
#include <string>
#include <map>

SWIFT_NS_BEGIN


struct Operator
{
    std::wstring name;
    OperatorType::T type;
    Associativity::T associativity;
    int precedence;
    Operator(const std::wstring& name, OperatorType::T type, Associativity::T associativity, int precedence)
        :name(name), type(type), associativity(associativity), precedence(precedence)
    {}
};
class SymbolRegistry
{
    typedef std::map<std::wstring, Operator> OperatorMap;
public:
    SymbolRegistry();
public:
    bool registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity = Associativity::None, int precedence = 100);
    Operator* getOperator(const std::wstring& name);

    bool isPrefixOperator(const std::wstring& name);
    bool isPostfixOperator(const std::wstring& name);
    bool isInfixOperator(const std::wstring& name);
private:
    OperatorMap operators;
};

SWIFT_NS_END

#endif//SYMBOL_REGISTRY_H
