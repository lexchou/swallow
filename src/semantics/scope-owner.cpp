#include "scope-owner.h"
#include "symbol-scope.h"
#include "ast/node.h"

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
        symbolScope->owner = dynamic_cast<Node*>(this);
    }
    return symbolScope;
}

