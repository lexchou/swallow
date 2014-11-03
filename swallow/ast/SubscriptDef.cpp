/* SubscriptDef.cpp --
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
#include "SubscriptDef.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


SubscriptDef::SubscriptDef()
    :Declaration(NodeType::Subscript), parameters(NULL), returnType(NULL), getter(NULL), setter(NULL)
{
}

SubscriptDef::~SubscriptDef()
{

}

void SubscriptDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSubscript);
}

void SubscriptDef::setParameters(const ParametersPtr& params)
{
    this->parameters = params;
}
ParametersPtr SubscriptDef::getParameters()
{
    return parameters;
}

void SubscriptDef::setReturnType(const TypeNodePtr& type)
{
    this->returnType = type;
}
TypeNodePtr SubscriptDef::getReturnType()
{
    return returnType;
}

void SubscriptDef::setReturnTypeAttributes(const Attributes& attrs)
{
    returnTypeAttributes = attrs;
}
const Attributes& SubscriptDef::getReturnTypeAttributes()const
{
    return returnTypeAttributes;
}

void SubscriptDef::setGetter(const CodeBlockPtr& getter)
{
    this->getter = getter;
}
CodeBlockPtr SubscriptDef::getGetter()
{
    return getter;
}

void SubscriptDef::setSetter(const CodeBlockPtr& setter)
{
    this->setter = setter;
}
CodeBlockPtr SubscriptDef::getSetter()
{
    return setter;
}

void SubscriptDef::setSetterName(const std::wstring& name)
{
    setterName = name;
}
const std::wstring& SubscriptDef::getSetterName()const
{
    return setterName;
}
