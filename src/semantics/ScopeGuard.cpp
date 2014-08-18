#include "ScopeGuard.h"
#include "ScopeOwner.h"
#include "SemanticNodeVisitor.h"
#include "SymbolRegistry.h"
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

