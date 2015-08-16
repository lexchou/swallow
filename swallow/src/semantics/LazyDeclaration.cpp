/* LazyDeclaration.cpp --
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
#include "semantics/LazyDeclaration.h"
#include "semantics/SymbolRegistry.h"
#include "ast/Declaration.h"

USE_SWALLOW_NS

using namespace std;
LazyDeclaration::DeclarationEntry::DeclarationEntry(SymbolScope* currentScope, SymbolScope* fileScope, const DeclarationPtr& node)
    :currentScope(currentScope), fileScope(fileScope), node(node)
{
    forwardDeclared = false;
}

void LazyDeclaration::addDeclaration(SymbolRegistry* registry, const DeclarationPtr& node)
{
    decls.push_back(DeclarationEntry(registry->getCurrentScope(), registry->getFileScope(), node));
}
void LazyDeclaration::clear()
{
    decls.clear();
}
bool LazyDeclaration::isFunc() const
{
    if(decls.empty())
        return false;
    bool ret = decls[0].node->getNodeType() == NodeType::Function;
    return ret;
}
