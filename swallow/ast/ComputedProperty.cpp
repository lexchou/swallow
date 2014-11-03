/* ComputedProperty.cpp --
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
#include "ComputedProperty.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ComputedProperty::ComputedProperty()
:Declaration(NodeType::ComputedProperty)
{

}

ComputedProperty::~ComputedProperty()
{

}

void ComputedProperty::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitComputedProperty);
}


void ComputedProperty::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& ComputedProperty::getTypeAttributes() const
{
    return typeAttributes;
}
void ComputedProperty::setDeclaredType(const TypeNodePtr& t)
{
    this->declaredType = t;
}
TypeNodePtr ComputedProperty::getDeclaredType()
{
    return declaredType;
}

const TypePtr& ComputedProperty::getType()const
{
    return type;
}

void ComputedProperty::setType(const TypePtr& type)
{
    this->type = type;
}

const std::wstring& ComputedProperty::getName()const
{
    return name;
}
void ComputedProperty::setName(const std::wstring& name)
{
    this->name = name;
}

void ComputedProperty::setInitializer(const ExpressionPtr& initializer)
{
    this->initializer = initializer;
}
const ExpressionPtr& ComputedProperty::getInitializer()const
{
    return initializer;
}

void ComputedProperty::setSetter(const CodeBlockPtr& setter)
{
    this->setter = setter;
}
CodeBlockPtr ComputedProperty::getSetter()
{
    return setter;
}

void ComputedProperty::setSetterName(const std::wstring& name)
{
    setterName = name;
}
const std::wstring& ComputedProperty::getSetterName()
{
    return setterName;
}

void ComputedProperty::setGetter(const CodeBlockPtr& getter)
{
    this->getter = getter;
}
CodeBlockPtr ComputedProperty::getGetter()
{
    return getter;
}


void ComputedProperty::setWillSet(const CodeBlockPtr& willSet)
{
    this->willSet = willSet;
}
CodeBlockPtr ComputedProperty::getWillSet()
{
    return willSet;
}

void ComputedProperty::setDidSet(const CodeBlockPtr& didSet)
{
    this->didSet = didSet;
}
CodeBlockPtr ComputedProperty::getDidSet()
{
    return didSet;
}


void ComputedProperty::setWillSetSetter(const std::wstring& name)
{
    willSetSetter = name;
}
const std::wstring& ComputedProperty::getWillSetSetter()const
{
    return willSetSetter;
}


void ComputedProperty::setDidSetSetter(const std::wstring& name)
{
    didSetSetter = name;
}
const std::wstring& ComputedProperty::getDidSetSetter()const
{
    return didSetSetter;
}


