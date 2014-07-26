#ifndef SYMBOL_SCOPE_H
#define SYMBOL_SCOPE_H
#include "swift_conf.h"
#include <map>
#include <memory>
SWIFT_NS_BEGIN

class SymbolRegistry;
class Symbol;
class Node;
class Type;
typedef Symbol* SymbolPtr;
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