/* SymbolScope.cpp --
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
#include "SymbolScope.h"
#include "SymbolRegistry.h"
#include "Type.h"
#include <cassert>

USE_SWALLOW_NS

SymbolScope::SymbolScope()
    :owner(NULL), parent(NULL)
{
}
SymbolScope::~SymbolScope()
{
}
Node* SymbolScope::getOwner()
{
    return owner;
}

void SymbolScope::addSymbol(const std::wstring& name, const SymbolPtr& symbol)
{
    assert(!name.empty());
    SymbolMap::iterator iter = symbols.find(name);
    assert(iter == symbols.end() && "The symbol already exists with the same name.");
    this->symbols.insert(std::make_pair(name, symbol));
}
void SymbolScope::addSymbol(const SymbolPtr& symbol)
{
    addSymbol(symbol->getName(), symbol);
}

void SymbolScope::removeSymbol(const SymbolPtr& symbol)
{
    SymbolMap::iterator iter = symbols.find(symbol->getName());
    if(iter != symbols.end() && iter->second == symbol)
        symbols.erase(iter);

}
SymbolPtr SymbolScope::lookup(const std::wstring& name)
{
    SymbolMap::iterator iter = symbols.find(name);
    if(iter != symbols.end())
        return iter->second;
    return NULL;
}
