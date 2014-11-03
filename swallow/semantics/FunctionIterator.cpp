/* FunctionIterator.cpp --
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
#include "FunctionIterator.h"
#include "Type.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"

USE_SWALLOW_NS

using namespace std;

FunctionIterator::iterator::iterator(const TypePtr& type, const std::wstring& name)
:type(type), name(name)
{
    if(type)
    {
        moveNext();
    }
}
bool FunctionIterator::iterator::operator!=(const FunctionIterator::iterator& rhs)const
{
    return value != rhs.value;
}
const FunctionIterator::iterator& FunctionIterator::iterator::operator++()
{
    if(!type)
        return *this;
    value = *begin;
    begin++;
    if(begin == end)
        moveNext();
    return *this;
}
void FunctionIterator::iterator::moveNext()
{
    if(begin == end)
    {
        value = nullptr;
        type = type->getParentType();
    }
    //move to the parent type
    for(;type; type = type->getParentType())
    {
        FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(type->getDeclaredMember(name));
        if(!funcs)
            continue;//this type doesn't defined the function, going upward
        begin = funcs->begin();
        end = funcs->end();
        value = *begin;
        break;
    }
}
FunctionSymbolPtr FunctionIterator::iterator::operator*()
{
    return value;
}

FunctionIterator::FunctionIterator(const TypePtr& type, const std::wstring& name)
:type(type), name(name)
{

}
FunctionIterator::iterator FunctionIterator::begin()
{
    return iterator(type, name);
}
FunctionIterator::iterator FunctionIterator::end()
{
    return iterator(nullptr, L"");
}
