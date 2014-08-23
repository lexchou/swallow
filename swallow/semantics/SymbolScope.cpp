#include "SymbolScope.h"
#include "SymbolRegistry.h"
#include "Type.h"
#include <cassert>

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
void SymbolScope::addSymbol(const SymbolPtr& symbol)
{
    SymbolMap::iterator iter = symbols.find(symbol->getName());
    assert(iter == symbols.end() && "The symbol already exists with the same name.");
    this->symbols.insert(std::make_pair(symbol->getName(), symbol));
}

void SymbolScope::removeSymbol(const SymbolPtr& symbol)
{
    SymbolMap::iterator iter = symbols.find(symbol->getName());
    if(iter != symbols.end() && iter->second == symbol)
        symbols.erase(iter);

}
SymbolPtr SymbolScope::lookup(const std::wstring& name)
{
    SymbolMap::iterator iter = symbols.find(name);
    if(iter != symbols.end())
        return iter->second;
    return NULL;
}
