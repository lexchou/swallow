/* CodeBlock.h --
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
#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H
#include "Statement.h"
#include "Attribute.h"
#include <string>

SWIFT_NS_BEGIN
typedef std::shared_ptr<class Type> TypePtr;

class CodeBlock : public Statement
{
public:
    CodeBlock();
    ~CodeBlock();
public:
    void addStatement(const StatementPtr& st);
    int numStatements();
    StatementPtr getStatement(int idx);
    
    void setAttributes(const Attributes& attrs);
    const Attributes& getAttributes()const;

    const TypePtr& getType()const;
    void setType(const TypePtr& type);
public:
    std::vector<StatementPtr>::iterator begin() { return statements.begin();}
    std::vector<StatementPtr>::iterator end() { return statements.end();}
public:
    virtual void accept(NodeVisitor* visitor);
private:
    Attributes attributes;
    std::vector<StatementPtr> statements;
    TypePtr type;
};

SWIFT_NS_END

#endif//CODE_BLOCK_H
