#include "symbol-scope.h"
#include "symbol-registry.h"

USE_SWIFT_NS

SymbolScope::SymbolScope(SymbolRegistry* registry)
:registry(registry)
{
    parent = registry->getCurrentScope();
    registry->enterScope(this);
}
SymbolScope::~SymbolScope()
{
    registry->leaveScope();
}

void SymbolScope::addSymbol(const std::wstring& name, Node* symbol)
{
    this->symbols.insert(std::make_pair(name, symbol));
}
Node* SymbolScope::lookup(const std::wstring& name)
{
    SymbolMap::iterator iter = symbols.find(name);
    if(iter != symbols.end())
        return iter->second;
    if(parent != NULL)
        return parent->lookup(name);
    return NULL;
}