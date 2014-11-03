/* Operator.h --
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
#ifndef OPERATOR_H
#define OPERATOR_H
#include "Expression.h"
#include "tokenizer/Token.h"
SWALLOW_NS_BEGIN

class Operator : public Expression
{
protected:
    Operator(NodeType::T);
public:
    
    OperatorType::T getOperatorType() const { return operatorType;}
    void setOperatorType(OperatorType::T type){this->operatorType = type;}
    Associativity::T getAssociativity() const { return associativity;}
    void setAssociativity(Associativity::T a) { associativity = a;}
    int getPrecedence() const { return precedence;}
    void setPrecedence(int v) { precedence = v;}
    
public:
    virtual int numChildren() = 0;
    virtual NodePtr get(int i) = 0;
    virtual void set(int i, const NodePtr& val) = 0;
protected:
    OperatorType::T operatorType;
    Associativity::T associativity;
    int precedence;
    
};

SWALLOW_NS_END


#endif//OPERATOR_H
