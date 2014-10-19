#include "ScopeGuard.h"
#include "ScopeOwner.h"
#include "SemanticAnalyzer.h"
#include "SymbolRegistry.h"
USE_SWIFT_NS


ScopeGuard::ScopeGuard(ScopeOwner* owner, NodeVisitor* visitor)
    :symbolRegistry(nullptr)
{
    SemanticAnalyzer* semanticNodeVisitor = dynamic_cast<SemanticAnalyzer*>(visitor);
    if(semanticNodeVisitor)
    {
        symbolRegistry = semanticNodeVisitor->getSymbolRegistry();
        SymbolScope *scope = owner->getScope();
        if(symbolRegistry->getCurrentScope() == scope)
        {
            //func and code block will shares the same scope,
            //do not enter the same scope twice
            symbolRegistry = nullptr;
            return;
        }

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

