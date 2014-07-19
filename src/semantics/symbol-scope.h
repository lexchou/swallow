#ifndef SYMBOL_SCOPE_H
#define SYMBOL_SCOPE_H
#include "swift_conf.h"
#include <map>
#include <memory>
SWIFT_NS_BEGIN

class SymbolRegistry;
class Node;
class Type;
typedef std::shared_ptr<Type> TypePtr;
class SymbolScope
{
    friend class SymbolRegistry;
public:
    SymbolScope(SymbolRegistry* registry);
    ~SymbolScope();
public:
    void addSymbol(const std::wstring& name, Node* symbol);
    void addType(TypePtr type);
    Node* lookup(const std::wstring& name);
    TypePtr lookupType(const std::wstring& name);
private:
    typedef std::map<std::wstring, Node*> SymbolMap;
    typedef std::map<std::wstring, TypePtr> TypeMap;
    SymbolScope* parent;
    SymbolRegistry* registry;
    SymbolMap symbols;
    TypeMap types;
};


SWIFT_NS_END

#endif//SYMBOL_SCOPE_H