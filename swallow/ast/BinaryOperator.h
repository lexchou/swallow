/* BinaryOperator.h --
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
#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H
#include "Operator.h"
#include <string>
SWIFT_NS_BEGIN

class BinaryOperator : public Operator
{
public:
    BinaryOperator();
    ~BinaryOperator();
public:
    void setLHS(PatternPtr val){lhs = val;}
    PatternPtr getLHS(){return lhs;}

    void setRHS(PatternPtr val){rhs = val;}
    PatternPtr getRHS(){return rhs;}
    
    const std::wstring& getOperator() const { return op;}
    void setOperator(const std::wstring& op) {this->op = op;}
public:
    
    virtual int numChildren();
    virtual NodePtr get(int i);
    virtual void set(int i, const NodePtr& val);
public:
    virtual void accept(NodeVisitor* visitor);
protected:
    std::wstring op;
    PatternPtr lhs;
    PatternPtr rhs;
};

SWIFT_NS_END

#endif//BINARY_OPERATOR_H
