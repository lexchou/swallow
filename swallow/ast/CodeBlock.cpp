/* CodeBlock.cpp --
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
#include "CodeBlock.h"
#include "NodeVisitor.h"
#include <cassert>
USE_SWALLOW_NS


CodeBlock::CodeBlock()
    :Statement(NodeType::CodeBlock)
{
}
CodeBlock::~CodeBlock()
{
}

void CodeBlock::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitCodeBlock);
}


void CodeBlock::addStatement(const StatementPtr& st)
{
    statements.push_back(st);
}
int CodeBlock::numStatements()
{
    return statements.size();
}
StatementPtr CodeBlock::getStatement(int idx)
{
    return statements[idx];
}


void CodeBlock::setAttributes(const Attributes& attrs)
{
    attributes = attrs;
}
const Attributes& CodeBlock::getAttributes()const
{
    return attributes;
}

const TypePtr& CodeBlock::getType()const
{
    return type;
}
void CodeBlock::setType(const TypePtr& type)
{
    this->type = type;
//    assert(type && type->getCategory() == Type::Function);
}
