/* ParameterNode.cpp --
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
#include "ast/ParameterNode.h"
#include "ast/NodeVisitor.h"
USE_SWALLOW_NS


ParameterNode::ParameterNode()
:Node(NodeType::Parameter)
{
    inout = false;
    accessibility = None;
    shorthandExternalName = false;
    type = NULL;
    defaultValue = NULL;
}
ParameterNode::~ParameterNode()
{

}

void ParameterNode::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitParameter);

}

void ParameterNode::setInout(bool inout)
{
    this->inout = inout;
}
bool ParameterNode::isInout()const
{
    return inout;
}

void ParameterNode::setAccessibility(Accessibility accessibility)
{
    this->accessibility = accessibility;
}
ParameterNode::Accessibility ParameterNode::getAccessibility() const
{
    return accessibility;
}

void ParameterNode::setExternalName(const std::wstring& name)
{
    this->externalName = name;
}
const std::wstring& ParameterNode::getExternalName()const
{
    return externalName;
}

void ParameterNode::setLocalName(const std::wstring& name)
{
    localName = name;
}
const std::wstring& ParameterNode::getLocalName()const
{
    return localName;
}

void ParameterNode::setShorthandExternalName(bool shorthandExternalName)
{
    this->shorthandExternalName = shorthandExternalName;
}
bool ParameterNode::isShorthandExternalName()const
{
    return shorthandExternalName;
}

void ParameterNode::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& ParameterNode::getTypeAttributes()const
{
    return typeAttributes;
}

void ParameterNode::setType(const TypePtr& type)
{
    this->type = type;
}
TypePtr ParameterNode::getType()
{
    return type;
}
void ParameterNode::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}
TypeNodePtr ParameterNode::getDeclaredType()
{
    return declaredType;
}
void ParameterNode::setDefaultValue(const ExpressionPtr& def)
{
    this->defaultValue = def;
}
ExpressionPtr ParameterNode::getDefaultValue()
{
    return defaultValue;
}
