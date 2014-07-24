#ifndef SCOPED_NODES_H
#define SCOPED_NODES_H
#include "swift_conf.h"
#include "ast/program.h"
#include "ast/class-def.h"
#include "ast/protocol-def.h"
#include "ast/struct-def.h"
#include "ast/enum-def.h"
#include "ast/extension-def.h"
#include "ast/code-block.h"
#include "ast/closure.h"
#include "scope-owner.h"

SWIFT_NS_BEGIN

class ScopedProgram : public Program, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);
};
class ScopedClass : public ClassDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);

};
class ScopedProtocol : public ProtocolDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);

};
class ScopedStruct : public StructDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);

};
class ScopedEnum : public EnumDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);

};
class ScopedExtension : public ExtensionDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);

};
class ScopedCodeBlock : public CodeBlock, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);

};
class ScopedClosure : public Closure, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);
};


SWIFT_NS_END




#endif//SCOPED_NODES_H