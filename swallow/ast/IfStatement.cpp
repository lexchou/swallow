/* IfStatement.cpp --
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
#include "IfStatement.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


IfStatement::IfStatement()
    :Statement(NodeType::If), condition(NULL), thenPart(NULL), elsePart(NULL)
{
}

IfStatement::~IfStatement()
{

}


void IfStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitIf);
}


void IfStatement::setCondition(const ExpressionPtr& expr)
{
    this->condition = expr;
}
ExpressionPtr IfStatement::getCondition()
{
    return condition;
}

void IfStatement::setThen(const CodeBlockPtr& thenPart)
{
    this->thenPart = thenPart;
}
CodeBlockPtr IfStatement::getThen()
{
    return thenPart;
}

void IfStatement::setElse(const StatementPtr& elsePart)
{
    this->elsePart = elsePart;
}
StatementPtr IfStatement::getElse()
{
    return elsePart;
}
