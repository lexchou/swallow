#include "scoped-nodes.h"
#include "semantic-node-visitor.h"
#include "symbol-scope.h"
#include "symbol-registry.h"
USE_SWIFT_NS


struct ScopeGuard
{
    ScopeGuard(ScopeOwner* owner, NodeVisitor* visitor)
    {
        symbolRegistry = static_cast<SemanticNodeVisitor*>(visitor)->getSymbolRegistry();
        SymbolScope* scope = owner->getScope();
        symbolRegistry->enterScope(scope);
    }
    ~ScopeGuard()
    {
        symbolRegistry->leaveScope();
    }
    SymbolRegistry* symbolRegistry;
};

void ScopedProgram::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitProgram(this);
}


void ScopedClass::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitClass(this);
}
void ScopedProtocol::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitProtocol(this);
}

void ScopedStruct::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitStruct(this);
}
void ScopedEnum::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitEnum(this);
}
void ScopedExtension::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitExtension(this);
}
void ScopedCodeBlock::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitCodeBlock(this);
}

void ScopedClosure::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitClosure(this);
}
