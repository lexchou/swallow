/* SymbolRegistry.h --
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
#ifndef SYMBOL_REGISTRY_H
#define SYMBOL_REGISTRY_H
#include "swallow_conf.h"
#include "swallow_types.h"
#include <string>
#include <map>
#include <stack>
#include "SymbolScope.h"
#include "semantic-types.h"
SWALLOW_NS_BEGIN

struct OperatorInfo;
class TypeNode;
typedef std::shared_ptr<TypeNode> TypeNodePtr;
class GlobalScope;
class SWALLOW_EXPORT SymbolRegistry
{
    friend class SymbolScope;
public:
    SymbolRegistry();
    ~SymbolRegistry();
public:
    bool registerOperator(const std::wstring& name, OperatorType::T type, Associativity::T associativity = Associativity::None, int precedence = 100, bool assignment = false);
    bool registerOperator(SymbolScope* scope, const std::wstring& name, OperatorType::T type, Associativity::T associativity = Associativity::None, int precedence = 100, bool assignment = false);

    OperatorInfo* getOperator(const std::wstring& name, int typeMask);
    OperatorInfo* getOperator(SymbolScope* scope, const std::wstring& name, int typeMask);

    bool isPrefixOperator(const std::wstring& name);
    bool isPostfixOperator(const std::wstring& name);
    bool isInfixOperator(const std::wstring& name);

    /*!
     * Returns the current scope
     */
    SymbolScope* getCurrentScope();
    /*!
     * Not recommended to use, it will interrupt the internal scope state, use it wisely.
     */
    void setCurrentScope(SymbolScope* scope);
    GlobalScope* getGlobalScope();
    SymbolScope* getFileScope();
    void setFileScope(SymbolScope* scope);

    TypePtr lookupType(const std::wstring& name);
    bool lookupType(const std::wstring& name, SymbolScope** container, TypePtr* ret);

    /*!
     * Lookup symbol in given scope by symbol name and return it's container scope and the symbol
     */
    bool lookupSymbol(SymbolScope* scope, const std::wstring& name, SymbolScope** container, SymbolPtr* ret);
    bool lookupSymbol(const std::wstring& name, SymbolScope** scope, SymbolPtr* ret);
    SymbolPtr lookupSymbol(const std::wstring& name);

public:
    void enterScope(SymbolScope* scope);
    void leaveScope();
private:
    std::stack<SymbolScope*> scopes;
    SymbolScope* currentScope;
    GlobalScope* globalScope;
    SymbolScope* fileScope;
};

SWALLOW_NS_END

#endif//SYMBOL_REGISTRY_H
