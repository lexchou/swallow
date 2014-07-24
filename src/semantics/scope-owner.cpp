#include "scope-owner.h"
#include "symbol-scope.h"

USE_SWIFT_NS


ScopeOwner::ScopeOwner()
{
    symbolScope = NULL;
}
ScopeOwner::~ScopeOwner()
{
    if(symbolScope)
    {
        delete symbolScope;
        symbolScope = NULL;
    }
}
SymbolScope* ScopeOwner::getScope()
{
    if(!symbolScope)
    {
        symbolScope = new SymbolScope();
    }
    return symbolScope;
}

