#include "scoped-nodes.h"
#include "semantic-node-visitor.h"
#include "symbol-scope.h"
#include "symbol-registry.h"
#include "ast/type-identifier.h"
#include <cassert>
#import <ast/identifier.h>

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
    scope.symbolRegistry->setFileScope(this->getScope());
    accept2(visitor, &NodeVisitor::visitProgram);
    scope.symbolRegistry->setFileScope(nullptr);
}


void ScopedClass::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitClass);
}
const std::wstring& ScopedClass::getName()const
{
    return identifier->getName();
}


void ScopedProtocol::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitProtocol);
}
const std::wstring& ScopedProtocol::getName()const
{
    return identifier->getName();
}

void ScopedStruct::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitStruct);
}
const std::wstring& ScopedStruct::getName()const
{
    return identifier->getName();
}
void ScopedEnum::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitEnum);
}
const std::wstring& ScopedEnum::getName()const
{
    return identifier->getName();
}
void ScopedExtension::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitExtension);
}
const std::wstring& ScopedExtension::getName()const
{
    return identifier->getName();
}



void ScopedCodeBlock::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitCodeBlock);
}

void ScopedClosure::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    accept2(visitor, &NodeVisitor::visitClosure);
}


const std::wstring& SymboledFunction::getName()const
{
    return name;
}

const std::wstring& SymboledVariable::getName()const
{
    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(name);
    return id->getIdentifier();
}

const std::wstring& SymboledConstant::getName()const
{
    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(this->name);
    return id->getIdentifier();
}