/* ValueBindings.cpp --
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
#include "ValueBindings.h"
#include "NodeVisitor.h"
#include "ValueBinding.h"
#include <algorithm>
USE_SWIFT_NS


ValueBindings::ValueBindings()
    :Declaration(NodeType::ValueBindings)
{
}
ValueBindings::~ValueBindings()
{
}

void ValueBindings::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitValueBindings);
}


void ValueBindings::add(const ValueBindingPtr& var)
{
    valueBindings.push_back(var);
    var->owner = std::static_pointer_cast<ValueBindings>(shared_from_this());
}
void ValueBindings::insertAfter(const ValueBindingPtr& binding, const Iterator& iter)
{
    if(iter == valueBindings.end())
    {
        valueBindings.push_back(binding);
    }
    else
    {
        Iterator it = iter;
        it++;
        valueBindings.insert(it, binding);
    }
    binding->owner = std::static_pointer_cast<ValueBindings>(shared_from_this());
}
ValueBindingPtr ValueBindings::get(int i)
{
    Iterator iter = valueBindings.begin();
    std::advance(iter, i);
    return *iter;
}
int ValueBindings::numBindings()
{
    return valueBindings.size();
}
