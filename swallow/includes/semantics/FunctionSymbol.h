/* FunctionSymbol.h --
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
#ifndef FUNCTION_SYMBOL_H
#define FUNCTION_SYMBOL_H
#include "swallow_conf.h"
#include "Symbol.h"
#include <vector>

SWALLOW_NS_BEGIN
typedef std::shared_ptr<Symbol> SymbolPtr;
typedef std::shared_ptr<class FunctionDef> FunctionDefPtr;
typedef std::weak_ptr<class FunctionDef> FunctionDefWeakPtr;
typedef std::shared_ptr<class CodeBlock> CodeBlockPtr;
typedef std::weak_ptr<class CodeBlock> CodeBlockWeakPtr;

enum FunctionRole
{
    /*!
     * It's a normal function
     */
    FunctionRoleNormal,
    /*!
     * It's a getter for a property
     */
    FunctionRoleGetter,
    /*!
     * It's a setter for a property
     */
    FunctionRoleSetter,
    /*!
     * It's a willSet for a property
     */
    FunctionRoleWillSet,
    /*!
     * It's a didSet for a property
     */
    FunctionRoleDidSet,
    /*!
     * It's a initializer for a type
     */
    FunctionRoleInit,
    /*!
     * It's a generated allocating initializer for a type
     */
    FunctionRoleAllocatingInit,
    /*!
     * It's a deinitializer for a type
     */
    FunctionRoleDeinit,
    /*!
     * It's a generated deallocating initializer for a type
     */
    FunctionRoleDeallocatingInit,

    /*!
     * It's an operator overload function.
     */
    FunctionRoleOperator,

    /*!
     * It's an enum case constructor function
     */
    FunctionRoleEnumCase,

};

class SWALLOW_EXPORT FunctionSymbol : public Symbol
{
    friend class FunctionOverloadedSymbol;
    friend class SymbolRegistry;
public:
    FunctionSymbol(const std::wstring& name, const TypePtr& signature, FunctionRole role, const CodeBlockPtr& definition);
public:
    virtual const std::wstring& getName() const;
    TypePtr getReturnType()const;
    TypePtr getType();
    CodeBlockPtr getDefinition();
    FunctionRole getRole() const { return role;}
private:
    std::wstring name;
    TypePtr type;
    FunctionRole role;
    CodeBlockWeakPtr definition;
};


SWALLOW_NS_END
#endif//FUNCTION_SYMBOL_H
