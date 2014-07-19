#include "symbol-scope.h"
#include "symbol-registry.h"
#include "type.h"

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
void SymbolScope::addType(TypePtr type)
{
    types.insert(std::make_pair(type->getName(), type));
}
TypePtr SymbolScope::lookupType(const std::wstring& name)
{
    TypeMap::iterator iter = types.find(name);
    if(iter != types.end())
        return iter->second;
    return NULL;
}
Node* SymbolScope::lookup(const std::wstring& name)
{
    SymbolMap::iterator iter = symbols.find(name);
    if(iter != symbols.end())
        return iter->second;
    return NULL;
}