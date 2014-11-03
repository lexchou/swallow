/* Closure.cpp --
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
#include "Closure.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


Closure::Closure()
    :Expression(NodeType::Closure), captureSpecifier(_), capture(NULL), parameters(NULL), returnType(NULL)
{
}

Closure::~Closure()
{

}


void Closure::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitClosure);
}

Closure::CaptureSpecifier Closure::getCaptureSpecifier()const
{
    return captureSpecifier;
}
void Closure::setCaptureSpecifier(CaptureSpecifier val)
{
    captureSpecifier = val;
}

void Closure::setCapture(const ExpressionPtr& capture)
{
    this->capture = capture;
}
ExpressionPtr Closure::getCapture()
{
    return capture;
}

void Closure::setParameters(const ParametersPtr& val)
{
    parameters = val;
}
ParametersPtr Closure::getParameters()
{
    return parameters;
}

void Closure::setReturnType(const TypeNodePtr& val)
{
    returnType = val;
}
TypeNodePtr Closure::getReturnType()
{
    return returnType;
}

void Closure::addStatement(const StatementPtr& st)
{
    statements.push_back(st);
}
int Closure::numStatement()const
{
    return statements.size();
}
StatementPtr Closure::getStatement(int i)
{
    return statements[i];
}
