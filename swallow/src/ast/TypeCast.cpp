/* TypeCast.cpp --
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
#include "ast/TypeCast.h"
#include "ast/TypeNode.h"
USE_SWALLOW_NS

TypeCast::TypeCast()
{
    op = L"as";
    associativity = Associativity::None;
    precedence = 132;

    optional = false;
}
TypeCast::~TypeCast()
{
}

void TypeCast::setOptional(bool val)
{
    optional = val;
    this->op = val ? L"as?" : L"as";
}
bool TypeCast::isOptional()const
{
    return optional;
}
TypeNodePtr TypeCast::getDeclaredType()
{
    return declaredType;
}
void TypeCast::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}

int TypeCast::numChildren()
{
    return 2;
}
NodePtr TypeCast::get(int i)
{
    switch(i)
    {
        case 0:
            return lhs;
        case 1:
            return declaredType;
        default:
            return NULL;
    }
}
void TypeCast::set(int i, const NodePtr& val)
{
    switch(i)
    {
        case 0:
            lhs = std::dynamic_pointer_cast<Pattern>(val);
            break;
        case 1:
            declaredType = std::dynamic_pointer_cast<TypeNode>(val);
            break;
        default:
            break;
    }
}

