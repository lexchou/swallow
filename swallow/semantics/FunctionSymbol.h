/* FunctionSymbol.h --
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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
#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H
#include "swift_conf.h"
#include "Symbol.h"
#include <vector>

SWIFT_NS_BEGIN
typedef std::shared_ptr<Symbol> SymbolPtr;
typedef std::shared_ptr<class FunctionDef> FunctionDefPtr;
typedef std::weak_ptr<class FunctionDef> FunctionDefWeakPtr;
typedef std::shared_ptr<class CodeBlock> CodeBlockPtr;
typedef std::weak_ptr<class CodeBlock> CodeBlockWeakPtr;
class FunctionSymbol : public Symbol
{
    friend class FunctionOverloadedSymbol;
    friend class SymbolRegistry;
public:
    FunctionSymbol(const std::wstring& name, const TypePtr& functionType, const CodeBlockPtr& definition);
public:
    virtual const std::wstring& getName() const;
    TypePtr getReturnType()const;
    TypePtr getType();
    CodeBlockPtr getDefinition();
private:
    std::wstring name;
    TypePtr type;
    CodeBlockWeakPtr definition;
};


SWIFT_NS_END
#endif//FUNCTION_SYMBOL_H
