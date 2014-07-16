#ifndef SYMBOL_SCOPE_H
#define SYMBOL_SCOPE_H
#include "swift_conf.h"
#include <map>
SWIFT_NS_BEGIN

class SymbolRegistry;
class Node;
class SymbolScope
{
public:
    SymbolScope(SymbolRegistry* registry);
    ~SymbolScope();
public:
    void addSymbol(const std::wstring& name, Node* symbol);
    Node* lookup(const std::wstring& name);
private:
    typedef std::map<std::wstring, Node*> SymbolMap;
    SymbolScope* parent;
    SymbolRegistry* registry;
    SymbolMap symbols;
};


SWIFT_NS_END

#endif//SYMBOL_SCOPE_H