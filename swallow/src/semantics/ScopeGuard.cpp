/* ScopeGuard.cpp --
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
#include "semantics/ScopeGuard.h"
#include "semantics/ScopeOwner.h"
#include "semantics/SemanticPass.h"
#include "semantics/SymbolRegistry.h"
USE_SWALLOW_NS


ScopeGuard::ScopeGuard(SymbolRegistry* registry, SymbolScope* newScope)
    :symbolRegistry(nullptr)
{
    if(newScope)
    {
        symbolRegistry = registry;
        registry->enterScope(newScope);
    }
}
ScopeGuard::ScopeGuard(ScopeOwner* owner, NodeVisitor* visitor)
    :symbolRegistry(nullptr)
{
    SemanticPass* semanticNodeVisitor = dynamic_cast<SemanticPass*>(visitor);
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

