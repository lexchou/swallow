/* TupleType.h --
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
#ifndef TUPLE_TYPE_H
#define TUPLE_TYPE_H
#include "TypeNode.h"
#include <string>
#include <vector>

SWALLOW_NS_BEGIN
class Attribute;
class SWALLOW_DLL TupleType : public TypeNode
{
public:
    struct TupleElement
    {
        bool inout;
        std::wstring name;
        TypeNodePtr type;
        TupleElement(bool inout, const std::wstring& name, const TypeNodePtr& type)
        :inout(inout), name(name), type(type){}
    };
public:
    TupleType();
    ~TupleType();
public:
    void add(bool inout, const std::wstring& name, const TypeNodePtr& type);
    int numElements();
    TypeNodePtr getElementType(int i);
    const TupleElement& getElement(int i);
    
    void setVariadicParameters(bool val);
    bool getVariadicParameters()const;

    std::vector<TupleElement>::iterator begin() {return elements.begin();};
    std::vector<TupleElement>::iterator end() {return elements.end();};

private:
    bool variadicParameters;
    std::vector<TupleElement> elements;
};

SWALLOW_NS_END

#endif//TUPLE_TYPE_H
