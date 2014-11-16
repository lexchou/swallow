/* GlobalScope.h --
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
#ifndef GLOBAL_SCOPE_H
#define GLOBAL_SCOPE_H
#include "SymbolScope.h"
#include <vector>
SWALLOW_NS_BEGIN

class SWALLOW_EXPORT GlobalScope : public SymbolScope
{
public:
    GlobalScope();
private:
    void initPrimitiveTypes();
    void initOperators();
    void initProtocols();
    /**
     * Register the implementation of binary operator
     */
    bool registerOperatorFunction(const std::wstring& name, const TypePtr& returnType, const TypePtr& lhs, const TypePtr& rhs);
public:
    /**
     * A short-hand way to declare an external function.
     */
    void declareFunction(const std::wstring&name, int flags, const wchar_t* returnType, ...);

    /**
     * A short-hand way to create a function symbol
     */
    FunctionSymbolPtr createFunction(const std::wstring&name, int flags, const wchar_t* returnType, ...);
private:
    FunctionSymbolPtr vcreateFunction(const std::wstring&name, int flags, const wchar_t* returnType, va_list va);
public:
    SymbolPtr s_True, s_False;

    TypePtr t_Bool, t_Void, t_String;
    TypePtr t_Int, t_UInt, t_Int8, t_UInt8, t_Int16, t_UInt16, t_Int32, t_UInt32, t_Int64, t_UInt64;
    TypePtr t_Float, t_Double;
    TypePtr t_Array, t_Dictionary, t_Optional;
    //Common used protocols
    TypePtr p_BooleanType;
    TypePtr p_Equatable;

    std::vector<TypePtr> t_Numbers;
    std::vector<TypePtr> t_Ints;

};

SWALLOW_NS_END
#endif//GLOBAL_SCOPE_H
