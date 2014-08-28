#ifndef SYMBOL_SCOPE_H
#define SYMBOL_SCOPE_H
#include "swift_conf.h"
#include <map>
#include <memory>
#include "semantic-types.h"
#include "swift_types.h"
#include <string>

SWIFT_NS_BEGIN

class Node;

struct OperatorInfo
{
    std::wstring name;
    Associativity::T associativity;
    OperatorType::T type;
    struct
    {
        int prefix;
        int infix;
        int postfix;
    } precedence;
    OperatorInfo(const std::wstring& name, Associativity::T associativity)
            :name(name), associativity(associativity), type(OperatorType::_)
    {
        this->precedence.prefix = -1;
        this->precedence.infix = -1;
        this->precedence.postfix = -1;
    }
};
class SymbolScope
{
    friend class SymbolRegistry;
    friend class ScopeOwner;
    typedef std::map<std::wstring, OperatorInfo> OperatorMap;
public:
    SymbolScope();
    ~SymbolScope();
public:
    void removeSymbol(const SymbolPtr& symbol);
    void addSymbol(const SymbolPtr& symbol);
    void addSymbol(const std::wstring& name, const SymbolPtr& symbol);

    SymbolPtr lookup(const std::wstring& name);
    Node* getOwner();
    SymbolScope* getParentScope() {return parent;}
private:
    OperatorMap operators;
    typedef std::map<std::wstring, SymbolPtr> SymbolMap;
    Node* owner;
    SymbolScope* parent;
    SymbolMap symbols;
};


SWIFT_NS_END

#endif//SYMBOL_SCOPE_H