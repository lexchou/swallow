#ifndef SYMBOL_REGISTRY_H
#define SYMBOL_REGISTRY_H
#include "swift_conf.h"
#include "swift_types.h"
#include <string>
#include <map>
#include <stack>
#include "symbol-scope.h"
#include "semantic-types.h"
SWIFT_NS_BEGIN


struct OperatorInfo
{
    std::wstring name;
    Associativity::T associativity;
    struct
    {
        int prefix;
        int infix;
        int postfix;
    } precedence;
    OperatorInfo(const std::wstring& name, Associativity::T associativity)
        :name(name), associativity(associativity)
    {
        this->precedence.prefix = -1;
        this->precedence.infix = -1;
        this->precedence.postfix = -1;
    }
};
class TypeNode;
typedef std::shared_ptr<TypeNode> TypeNodePtr;
class SymbolRegistry
{
    typedef std::map<std::wstring, OperatorInfo> OperatorMap;
    friend class SymbolScope;
public:
    SymbolRegistry();
public:
    bool registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity = Associativity::None, int precedence = 100);
    OperatorInfo* getOperator(const std::wstring& name);

    bool isPrefixOperator(const std::wstring& name);
    bool isPostfixOperator(const std::wstring& name);
    bool isInfixOperator(const std::wstring& name);

    SymbolScope* getCurrentScope();

    TypePtr lookupType(const std::wstring& name);
    bool lookupType(const std::wstring& name, SymbolScope** scope, TypePtr* ret);

    bool lookupSymbol(const std::wstring& name, SymbolScope** scope, SymbolPtr* ret);
    SymbolPtr lookupSymbol(const std::wstring& name);

public:
    void enterScope(SymbolScope* scope);
    void leaveScope();
private:
    OperatorMap operators;
    std::stack<SymbolScope*> scopes;
    SymbolScope* currentScope;
    SymbolScope topScope;
};

SWIFT_NS_END

#endif//SYMBOL_REGISTRY_H
