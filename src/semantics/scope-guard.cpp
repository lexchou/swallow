#include "scope-guard.h"
#include "scope-owner.h"
#include "semantic-node-visitor.h"
#include "symbol-registry.h"
USE_SWIFT_NS


ScopeGuard::ScopeGuard(ScopeOwner* owner, NodeVisitor* visitor)
    :symbolRegistry(nullptr)
{
    SemanticNodeVisitor* semanticNodeVisitor = dynamic_cast<SemanticNodeVisitor*>(visitor);
    if(semanticNodeVisitor)
    {
        symbolRegistry = dynamic_cast<SemanticNodeVisitor *>(visitor)->getSymbolRegistry();
        SymbolScope *scope = owner->getScope();
        symbolRegistry->enterScope(scope);
    }
}
ScopeGuard::~ScopeGuard()
{
    if(symbolRegistry)
    {
        symbolRegistry->leaveScope();
    }
}

