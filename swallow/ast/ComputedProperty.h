/* ComputedProperty.h --
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
#ifndef COMPUTED_PROPERTY_H
#define COMPUTED_PROPERTY_H
#include "Declaration.h"
#include "Attribute.h"
#include <string>

SWALLOW_NS_BEGIN
class CodeBlock;
class Expression;
class Pattern;
class TypeNode;
typedef std::shared_ptr<class Type> TypePtr;
class SWALLOW_EXPORT ComputedProperty : public Declaration
{
public:
    ComputedProperty();
    ~ComputedProperty();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:

    void setTypeAttributes(const Attributes& attrs);
    const Attributes& getTypeAttributes() const;

    void setDeclaredType(const TypeNodePtr& t);
    TypeNodePtr getDeclaredType();

    const TypePtr& getType()const;
    void setType(const TypePtr& type);

    const std::wstring& getName()const;
    void setName(const std::wstring&);

    void setInitializer(const ExpressionPtr& initializer);
    const ExpressionPtr& getInitializer()const;
    void setSetter(const CodeBlockPtr& setter);
    CodeBlockPtr getSetter();
    
    void setSetterName(const std::wstring& name);
    const std::wstring& getSetterName();
    
    void setGetter(const CodeBlockPtr& getter);
    CodeBlockPtr getGetter();
    
    void setWillSet(const CodeBlockPtr& willSet);
    CodeBlockPtr getWillSet();
    void setWillSetSetter(const std::wstring& name);
    const std::wstring& getWillSetSetter()const;
    
    void setDidSet(const CodeBlockPtr& didSet);
    CodeBlockPtr getDidSet();
    
    void setDidSetSetter(const std::wstring& name);
    const std::wstring& getDidSetSetter()const;

public:
    virtual void accept(NodeVisitor* visitor) override;
private:
    std::wstring name;
    ExpressionPtr initializer;
    TypePtr type;
    TypeNodePtr declaredType;
    Attributes typeAttributes;
    std::wstring setterName;
    std::wstring willSetSetter;
    std::wstring didSetSetter;
    CodeBlockPtr getter;
    CodeBlockPtr setter;
    CodeBlockPtr willSet;
    CodeBlockPtr didSet;

};

SWALLOW_NS_END

#endif//COMPUTED_PROPERTY_H
