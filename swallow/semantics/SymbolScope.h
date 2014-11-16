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

SWALLOW_NS_BEGIN

class Node;

struct OperatorInfo
{
    std::wstring name;
    Associativity::T associativity;
    OperatorType::T type;
    struct
    {
        int prefix;
        int infix;
        int postfix;
    } precedence;
    OperatorInfo(const std::wstring& name, Associativity::T associativity)
            :name(name), associativity(associativity), type(OperatorType::_)
    {
        this->precedence.prefix = -1;
        this->precedence.infix = -1;
        this->precedence.postfix = -1;
    }
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

    SymbolPtr lookup(const std::wstring& name);
    Node* getOwner();
    SymbolScope* getParentScope() {return parent;}
    const SymbolMap& getSymbols() {return symbols;}
protected:
    OperatorMap operators;
    Node* owner;
    SymbolScope* parent;
    SymbolMap symbols;
};


SWALLOW_NS_END

#endif//SYMBOL_SCOPE_H
