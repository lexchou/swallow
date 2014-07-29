#ifndef SYMBOL_SCOPE_H
#define SYMBOL_SCOPE_H
#include "swift_conf.h"
#include <map>
#include <memory>
#include "semantic-types.h"
SWIFT_NS_BEGIN

class Node;
class SymbolScope
{
    friend class SymbolRegistry;
    friend class ScopeOwner;
public:
    SymbolScope();
    ~SymbolScope();
public:
    void addSymbol(SymbolPtr symbol);
    SymbolPtr lookup(const std::wstring& name);
    Node* getOwner();
    SymbolScope* getParentScope() {return parent;}
private:
    typedef std::map<std::wstring, SymbolPtr> SymbolMap;
    Node* owner;
    SymbolScope* parent;
    SymbolMap symbols;
};


SWIFT_NS_END

#endif//SYMBOL_SCOPE_H