/* ScopedNodes.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SCOPED_NODES_H
#define SCOPED_NODES_H
#include "swallow_conf.h"
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
#include "ast/ComputedProperty.h"
#include "Symbol.h"

SWALLOW_NS_BEGIN

class SWALLOW_EXPORT ScopedProgram : public Program, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    void setScope(SymbolScope* scope);
};
class SWALLOW_EXPORT ScopedClass : public ClassDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    const std::wstring& getName()const;
public:
    TypePtr type;

};
class SWALLOW_EXPORT ScopedProtocol : public ProtocolDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    const std::wstring& getName()const;
public:
    TypePtr type;
};
class SWALLOW_EXPORT ScopedStruct : public StructDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    const std::wstring& getName()const;
public:
    TypePtr type;
};
class SWALLOW_EXPORT ScopedEnum : public EnumDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    const std::wstring& getName()const;
public:
    TypePtr type;
};
class SWALLOW_EXPORT ScopedExtension : public ExtensionDef, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
    const std::wstring& getName()const;

};
class SWALLOW_EXPORT ScopedCodeBlock : public CodeBlock, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
};
class SWALLOW_EXPORT ScopedClosure : public Closure, public ScopeOwner
{
public:
    virtual void accept(NodeVisitor* visitor) override;
};

template<class NodeType, class SymbolType>
class SWALLOW_EXPORT SymboledNode : public NodeType
{
public:
    std::shared_ptr<SymbolType> symbol;
};
typedef SymboledNode<FunctionDef, class FunctionSymbol> SymboledFunction;
typedef SymboledNode<InitializerDef, class FunctionSymbol> SymboledInit;
typedef std::shared_ptr<SymboledFunction> SymboledFunctionPtr;
class SWALLOW_EXPORT ComposedComputedProperty : public ComputedProperty
{
public:
    struct
    {
        SymboledFunctionPtr getter;
        SymboledFunctionPtr setter;
        SymboledFunctionPtr didSet;
        SymboledFunctionPtr willSet;
    } functions;

};





SWALLOW_NS_END


#endif//SCOPED_NODES_H
