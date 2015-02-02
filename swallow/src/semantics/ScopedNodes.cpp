/* ScopedNodes.cpp --
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
#include "semantics/ScopedNodes.h"
#include "semantics/SymbolScope.h"
#include "semantics/SymbolRegistry.h"
#include "ast/TypeIdentifier.h"
#include <cassert>
#include "ast/Identifier.h"
#include "semantics/ScopeGuard.h"
#include "ast/NodeVisitor.h"

USE_SWALLOW_NS



void ScopedProgram::accept(NodeVisitor* visitor)
{
    ScopeGuard scope(this, visitor);
    if(scope.symbolRegistry)
        scope.symbolRegistry->setFileScope(this->getScope());
    accept2(visitor, &NodeVisitor::visitProgram);
    if(scope.symbolRegistry)
        scope.symbolRegistry->setFileScope(nullptr);
}
void ScopedProgram::setScope(SymbolScope* scope)
{
    this->symbolScope = scope;
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


