#ifndef SYMBOL_REGISTRY_H
#define SYMBOL_REGISTRY_H
#include "swift_conf.h"
#include "swift_types.h"
#include <string>
#include <map>
#include <stack>
#include "SymbolScope.h"
#include "semantic-types.h"
SWIFT_NS_BEGIN

struct OperatorInfo;
class TypeNode;
typedef std::shared_ptr<TypeNode> TypeNodePtr;
class GlobalScope;
class SymbolRegistry
{
    friend class SymbolScope;
public:
    SymbolRegistry();
    ~SymbolRegistry();
public:
    bool registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity = Associativity::None, int precedence = 100);
    bool registerOperator(SymbolScope* scope, const std::wstring& name, OperatorType::T type, Associativity::T associativity = Associativity::None, int precedence = 100);

    OperatorInfo* getOperator(const std::wstring& name);
    OperatorInfo* getOperator(SymbolScope* scope, const std::wstring& name);

    bool isPrefixOperator(const std::wstring& name);
    bool isPostfixOperator(const std::wstring& name);
    bool isInfixOperator(const std::wstring& name);

    SymbolScope* getCurrentScope();
    GlobalScope* getGlobalScope();
    SymbolScope* getFileScope();
    void setFileScope(SymbolScope* scope);

    TypePtr lookupType(const std::wstring& name);
    bool lookupType(const std::wstring& name, SymbolScope** container, TypePtr* ret);

    /**
     * Lookup symbol in given scope by symbol name and return it's container scope and the symbol
     */
    bool lookupSymbol(SymbolScope* scope, const std::wstring& name, SymbolScope** container, SymbolPtr* ret);
    bool lookupSymbol(const std::wstring& name, SymbolScope** scope, SymbolPtr* ret);
    SymbolPtr lookupSymbol(const std::wstring& name);

public:
    void enterScope(SymbolScope* scope);
    void leaveScope();
private:
    std::stack<SymbolScope*> scopes;
    SymbolScope* currentScope;
    GlobalScope* globalScope;
    SymbolScope* fileScope;
};

SWIFT_NS_END

#endif//SYMBOL_REGISTRY_H
