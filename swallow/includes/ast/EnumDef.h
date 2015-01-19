/* EnumDef.h --
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
#ifndef ENUM_DEF_H
#define ENUM_DEF_H
#include "TypeDeclaration.h"
#include <string>

SWALLOW_NS_BEGIN
class Expression;
class TupleType;
class SWALLOW_EXPORT EnumDef : public TypeDeclaration
{
public:
    enum ValueStyle
    {
        Undefined,
        AssociatedValues,
        RawValues
    };
public:
    struct Case
    {
        std::wstring name;
        NodePtr value;
        Case(const std::wstring& name, const NodePtr& value)
        :name(name), value(value){}
    };
public:
    EnumDef();
    ~EnumDef();
public:
    using Declaration::setGenericParametersDef;
    using Declaration::getGenericParametersDef;
public:
    ValueStyle getValueStyle()const;
    void setValueStyle(ValueStyle style);
    
    void addCase(const std::wstring& name, const NodePtr& value);
    int numCases() const;
    const Case& getCase(int i) const;
    const std::vector<Case>& getCases() const;


public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::vector<Case> cases;
    ValueStyle valueStyle;
};

SWALLOW_NS_END

#endif//ENUM_DEF_H
