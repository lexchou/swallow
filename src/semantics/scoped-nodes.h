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
#include "ast/function-def.h"
#include "ast/constant.h"
#include "ast/variable.h"
#include "symbol.h"

SWIFT_NS_BEGIN

class ScopedProgram : public Program, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor);
};
class ScopedClass : public ClassDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor);
    virtual const std::wstring& getName()const;

};
class ScopedProtocol : public ProtocolDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor);
    virtual const std::wstring& getName()const;

};
class ScopedStruct : public StructDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor);
    virtual const std::wstring& getName()const;

};
class ScopedEnum : public EnumDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor);
    virtual const std::wstring& getName()const;

};
class ScopedExtension : public ExtensionDef, public ScopeOwner, public Symbol
{
public:
    virtual void accept(NodeVisitor* visitor);
    virtual const std::wstring& getName()const;

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

class SymboledFunction : public FunctionDef, public Symbol
{
public:
    const std::wstring& getName()const;
};
class SymboledVariable : public Variable, public Symbol
{
public:
    virtual const std::wstring& getName()const;
};
class SymboledConstant : public Constant, public Symbol
{
public:
    virtual const std::wstring& getName()const;
};



SWIFT_NS_END




#endif//SCOPED_NODES_H