/* Closure.h --
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
#ifndef CLOSURE_H
#define CLOSURE_H
#include "Expression.h"
#include <string>

SWIFT_NS_BEGIN

class Parameters;
class TypeNode;
class Statement;
class Closure : public Expression
{
public:
    enum CaptureSpecifier
    {
        _,
        Weak,
        Unowned,
        Unowned_Safe,
        Unowned_Unsafe
    };
public:
    Closure();
    ~Closure();
public:
    virtual void accept(NodeVisitor* visitor);
public:
    CaptureSpecifier getCaptureSpecifier()const;
    void setCaptureSpecifier(CaptureSpecifier val);
    
    void setCapture(const ExpressionPtr& capture);
    ExpressionPtr getCapture();
    
    void setParameters(const ParametersPtr& val);
    ParametersPtr getParameters();
    
    void setReturnType(const TypeNodePtr& val);
    TypeNodePtr getReturnType();
    
    void addStatement(const StatementPtr& st);
    int numStatement()const;
    StatementPtr getStatement(int i);

    std::vector<StatementPtr>::iterator begin(){return statements.begin();}
    std::vector<StatementPtr>::iterator end(){return statements.end();}
public:
    CaptureSpecifier captureSpecifier;
    ExpressionPtr capture;
    ParametersPtr parameters;
    TypeNodePtr returnType;
    std::vector<StatementPtr> statements;
    
};

SWIFT_NS_END

#endif//CLOSURE_H
