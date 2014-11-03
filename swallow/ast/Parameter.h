/* Parameter.h --
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
#ifndef PARAMETER_H
#define PARAMETER_H
#include "Node.h"
#include <string>
#include "Attribute.h"
#include "ast-decl.h"

SWIFT_NS_BEGIN

class TypeNode;
class Expression;
class Type;
typedef std::shared_ptr<Type> TypePtr;
class Parameter : public Node
{
public:
    enum Accessibility
    {
        None,
        Constant,
        Variable
    };
public:
    Parameter();
    ~Parameter();
public:
    virtual void accept(NodeVisitor* visitor);
public:
    void setInout(bool inout);
    bool isInout()const;
    
    void setAccessibility(Accessibility accessibility);
    Accessibility getAccessibility() const;
    
    void setExternalName(const std::wstring& name);
    const std::wstring& getExternalName()const;
    
    void setLocalName(const std::wstring& name);
    const std::wstring& getLocalName()const;
    
    void setShorthandExternalName(bool shorthandExternalName);
    bool isShorthandExternalName()const;
    
    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes()const;
    
    void setDeclaredType(const TypeNodePtr& type);
    TypeNodePtr getDeclaredType();


    void setType(const TypePtr& type);
    TypePtr getType();

    void setDefaultValue(const ExpressionPtr& def);
    ExpressionPtr getDefaultValue();
    
private:
    bool inout;
    bool shorthandExternalName;
    Accessibility accessibility;
    std::wstring externalName;
    std::wstring localName;
    Attributes typeAttributes;
    TypeNodePtr declaredType;
    TypePtr type;
    ExpressionPtr defaultValue;
};

SWIFT_NS_END

#endif//PARAMETER_H
