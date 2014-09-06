#ifndef SCOPED_NODES_H
#define SCOPED_NODES_H
#include "swift_conf.h"
#include "ast/Program.h"
#include "ast/ClassDef.h"
#include "ast/ProtocolDef.h"
#include "ast/StructDef.h"
#include "ast/EnumDef.h"
#include "ast/ExtensionDef.h"
#include "ast/CodeBlock.h"
#include "ast/Closure.h"
#include "ScopeOwner.h"
#include "ast/FunctionDef.h"
#include "Symbol.h"

SWIFT_NS_BEGIN

class ScopedProgram : public Program, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
};
class ScopedClass : public ClassDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    virtual const std::wstring& getName()const override;

};
class ScopedProtocol : public ProtocolDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    virtual const std::wstring& getName()const override;

};
class ScopedStruct : public StructDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    virtual const std::wstring& getName()const override;

};
class ScopedEnum : public EnumDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    virtual const std::wstring& getName()const override;

};
class ScopedExtension : public ExtensionDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    virtual const std::wstring& getName()const override;

};
class ScopedCodeBlock : public CodeBlock, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;

};
class ScopedClosure : public Closure, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
};

class SymboledFunction : public FunctionDef, public Symbol
{
public:
    const std::wstring& getName()const override;
};





SWIFT_NS_END


#endif//SCOPED_NODES_H
