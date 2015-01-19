/* Parameter.cpp --
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
#include "ast/Parameter.h"
#include "ast/NodeVisitor.h"
USE_SWALLOW_NS


Parameter::Parameter()
:Node(NodeType::Parameter)
{
    inout = false;
    accessibility = None;
    shorthandExternalName = false;
    type = NULL;
    defaultValue = NULL;
}
Parameter::~Parameter()
{

}

void Parameter::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitParameter);

}

void Parameter::setInout(bool inout)
{
    this->inout = inout;
}
bool Parameter::isInout()const
{
    return inout;
}

void Parameter::setAccessibility(Accessibility accessibility)
{
    this->accessibility = accessibility;
}
Parameter::Accessibility Parameter::getAccessibility() const
{
    return accessibility;
}

void Parameter::setExternalName(const std::wstring& name)
{
    this->externalName = name;
}
const std::wstring& Parameter::getExternalName()const
{
    return externalName;
}

void Parameter::setLocalName(const std::wstring& name)
{
    localName = name;
}
const std::wstring& Parameter::getLocalName()const
{
    return localName;
}

void Parameter::setShorthandExternalName(bool shorthandExternalName)
{
    this->shorthandExternalName = shorthandExternalName;
}
bool Parameter::isShorthandExternalName()const
{
    return shorthandExternalName;
}

void Parameter::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& Parameter::getTypeAttributes()const
{
    return typeAttributes;
}

void Parameter::setType(const TypePtr& type)
{
    this->type = type;
}
TypePtr Parameter::getType()
{
    return type;
}
void Parameter::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}
TypeNodePtr Parameter::getDeclaredType()
{
    return declaredType;
}
void Parameter::setDefaultValue(const ExpressionPtr& def)
{
    this->defaultValue = def;
}
ExpressionPtr Parameter::getDefaultValue()
{
    return defaultValue;
}
