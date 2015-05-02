/* SymbolScope.h --
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
#ifndef SYMBOL_SCOPE_H
#define SYMBOL_SCOPE_H
#include "swallow_conf.h"
#include <map>
#include <memory>
#include "semantic-types.h"
#include "swallow_types.h"
#include <string>
#include <vector>

SWALLOW_NS_BEGIN

class Node;

struct OperatorInfo
{
    std::wstring name;
    Associativity::T associativity;
    OperatorType::T type;
    bool assignment;
    struct
    {
        int prefix;
        int infix;
        int postfix;
    } precedence;
    OperatorInfo(const std::wstring& name, Associativity::T associativity, bool assignment)
            :name(name), associativity(associativity), type(OperatorType::_), assignment(assignment)
    {
        this->precedence.prefix = -1;
        this->precedence.infix = -1;
        this->precedence.postfix = -1;
    }
};


/*!
 * Implement this interface to allow a SymbolScope to resolve lazy symbols
 */
struct SWALLOW_EXPORT LazySymbolResolver
{
    /*!
     * Resolve a lazy symbol by its name, or return null if its undefined
     */
    virtual bool resolveLazySymbol(const std::wstring& name) = 0;
};

class SWALLOW_EXPORT SymbolScope
{
    friend class SymbolRegistry;
    friend class ScopeOwner;
public:
    typedef std::map<std::wstring, OperatorInfo> OperatorMap;
    typedef std::map<std::wstring, SymbolPtr> SymbolMap;
public:
    SymbolScope();
    ~SymbolScope();
public:
    void removeSymbol(const SymbolPtr& symbol);
    void addSymbol(const SymbolPtr& symbol);
    void addSymbol(const std::wstring& name, const SymbolPtr& symbol);

    /*!
     * Lookup for a symbol in current scope.
     * If lazySymbolResolve is specified and symbol is not found,
     * will try to use lazySymbolResolver to declare it
     */
    SymbolPtr lookup(const std::wstring& name, bool lazyResolve = true);
    /*!
     * Check if symbol is defined.
     * This will not use lazySymbolResolver to declare it
     */
    bool isSymbolDefined(const std::wstring& name);

    Node* getOwner();
    SymbolScope* getParentScope() {return parent;}
    const SymbolMap& getSymbols() {return symbols;}

    /*!
     * If SymbolScope failed to look-up a symbol, it will try to use LazySymbolResolver to find it
     * and cache the resolved symbol
     */
    void setLazySymbolResolver(LazySymbolResolver* resolver) {lazySymbolResolver = resolver;}
    LazySymbolResolver* getLazySymbolResolver() {return lazySymbolResolver;}

    /*!
     * Get an extension definition by given name
     */
    bool getExtension(const std::wstring& name, std::vector<TypePtr>** result);

    /*!
     * Register an extension to this scope.
     */
    void addExtension(const TypePtr& extension);
protected:
    OperatorMap operators;
    Node* owner;
    SymbolScope* parent;
    SymbolMap symbols;
    LazySymbolResolver* lazySymbolResolver;
    std::map<std::wstring, std::vector<TypePtr>> extensions;
};


SWALLOW_NS_END

#endif//SYMBOL_SCOPE_H
