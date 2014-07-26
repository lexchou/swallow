#include "symbol-scope.h"
#include "symbol-registry.h"
#include "type.h"

USE_SWIFT_NS

SymbolScope::SymbolScope()
    :owner(NULL), parent(NULL)
{
}
SymbolScope::~SymbolScope()
{
}
Node* SymbolScope::getOwner()
{
    return owner;
}
void SymbolScope::addSymbol(SymbolPtr symbol)
{
    this->symbols.insert(std::make_pair(symbol->getName(), symbol));
}

Symbol* SymbolScope::lookup(const std::wstring& name)
{
    SymbolMap::iterator iter = symbols.find(name);
    if(iter != symbols.end())
        return iter->second;
    return NULL;
}