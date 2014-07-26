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
    visitor->visitProgram(this);
}


void ScopedClass::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitClass(this);
}
const std::wstring& ScopedClass::getName()const
{
    return identifier->getName();
}


void ScopedProtocol::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitProtocol(this);
}
const std::wstring& ScopedProtocol::getName()const
{
    return identifier->getName();
}

void ScopedStruct::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitStruct(this);
}
const std::wstring& ScopedStruct::getName()const
{
    return identifier->getName();
}
void ScopedEnum::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitEnum(this);
}
const std::wstring& ScopedEnum::getName()const
{
    return identifier->getName();
}
void ScopedExtension::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    visitor->visitExtension(this);
}
const std::wstring& ScopedExtension::getName()const
{
    return identifier->getName();
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


const std::wstring& SymboledFunction::getName()const
{
    return name;
}

const std::wstring& SymboledVariable::getName()const
{
    Identifier* id = dynamic_cast<Identifier*>(name);
    return id->getIdentifier();
}

const std::wstring& SymboledConstant::getName()const
{
    Identifier* id = dynamic_cast<Identifier*>(this->name);
    return id->getIdentifier();
}