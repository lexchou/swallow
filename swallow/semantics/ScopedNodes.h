/* ScopedNodes.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
#include "Symbol.h"

SWALLOW_NS_BEGIN

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





SWALLOW_NS_END


#endif//SCOPED_NODES_H
