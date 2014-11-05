/* FunctionOverloadedSymbol.cpp --
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
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "Type.h"
USE_SWALLOW_NS

FunctionOverloadedSymbol::FunctionOverloadedSymbol(const std::wstring& name)
:name(name)
{

}
FunctionOverloadedSymbol::FunctionOverloadedSymbol()
{

}
const std::wstring& FunctionOverloadedSymbol::getName() const
{
    return name;
}
int FunctionOverloadedSymbol::numOverloads()const
{
    return functions.size();
}
FunctionSymbolPtr FunctionOverloadedSymbol::lookupOverload(int argc, TypePtr argv[])
{
    for(const FunctionSymbolPtr& func : functions)
    {
        TypePtr signature = func->getType();
        const std::vector<Type::Parameter>& parameterTypes = signature->getParameters();

        if(parameterTypes.size() != (size_t)argc)
            continue;
        //check each parameter's type
        bool matched = true;
        int i = 0;
        for(const Type::Parameter& parameter : parameterTypes)
        {
            if(*parameter.type != *argv[i++])
            {
                matched = false;
                break;
            }
        }
        if(matched)
            return func;
    }
    return nullptr;
}
void FunctionOverloadedSymbol::add(const FunctionSymbolPtr& func)
{
    functions.push_back(func);
}
void FunctionOverloadedSymbol::add(const FunctionOverloadedSymbolPtr& funcs)
{
    for(const FunctionSymbolPtr& func : funcs->functions)
        functions.push_back(func);
}
