/* SymbolScope.cpp --
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
#include "semantics/SymbolScope.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Type.h"
#include <cassert>
#include <iostream>

USE_SWALLOW_NS

SymbolScope::SymbolScope()
    :owner(NULL), parent(NULL)
{
    lazySymbolResolver = nullptr;
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
/*!
    * Check if symbol is defined.
    * This will not use lazySymbolResolver to declare it
    */
bool SymbolScope::isSymbolDefined(const std::wstring& name)
{
    SymbolMap::iterator iter = symbols.find(name);
    return iter != symbols.end();
}
static SymbolPtr resolveTypeAlias(const SymbolPtr& sym)
{
    if(!sym || sym->getKind() != SymbolKindType)
        return sym;
    TypePtr t = std::static_pointer_cast<Type>(sym);
    TypePtr ret = t->resolveAlias();
    return ret;
}
SymbolPtr SymbolScope::lookup(const std::wstring& name, bool lazyResolve)
{
    assert(!name.empty());
    SymbolMap::iterator iter = symbols.find(name);
    if(iter != symbols.end())
        return resolveTypeAlias(iter->second);
    //check it in LazySymbolResolver
    if(lazySymbolResolver && lazyResolve)
    {
        bool success = lazySymbolResolver->resolveLazySymbol(name);
        if(success)
        {
            iter = symbols.find(name);
            if(iter != symbols.end())
            {
                return resolveTypeAlias(iter->second);
            }
        }
    }
    return nullptr;
}

/*!
 * Get an extension definition by given name
 */
bool SymbolScope::getExtension(const std::wstring& name, std::vector<TypePtr>** result)
{
    if(result)
        *result = nullptr;
    auto iter = extensions.find(name);
    if(iter == extensions.end())
        return false;
    *result = &iter->second;
    return true;
}

/*!
 * Register an extension to this scope.
 */
void SymbolScope::addExtension(const TypePtr& extension)
{
    assert(extension != nullptr);
    assert(extension->getCategory() == Type::Extension);
    auto iter = extensions.find(extension->getName());
    if(iter == extensions.end())
    {
        iter = extensions.insert(std::make_pair(extension->getName(), std::vector<TypePtr>())).first;
    }
    iter->second.push_back(extension);
}
