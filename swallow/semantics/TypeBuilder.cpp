/* TypeBuilder.cpp --
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
#include "TypeBuilder.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include <cassert>


USE_SWALLOW_NS

using namespace std;

TypeBuilder::TypeBuilder(Category category)
:Type(category)
{
}
void TypeBuilder::setInitializer(const FunctionOverloadedSymbolPtr& initializer)
{
    this->initializer = initializer;
}

void TypeBuilder::addParameter(const Type::Parameter& param)
{
    parameters.push_back(param);
}

void TypeBuilder::setParentType(const TypePtr &type)
{
    this->parentType = type;
}
void TypeBuilder::setInnerType(const TypePtr &type)
{
    innerType = type;
}
void TypeBuilder::addProtocol(const TypePtr &protocol)
{
    protocols.push_back(protocol);
    auto iter = parents.find(protocol);
    if(iter == parents.end())
        parents.insert(make_pair(protocol, 1));
    else
        iter->second = 1;
}
void TypeBuilder::addParentTypesFrom(const TypePtr& type)
{
    for(auto parent : type->parents)
    {
        addParentType(parent.first, parent.second + 1);
    }
    addParentType(type, 1);
}
void TypeBuilder::addParentType(const TypePtr& type, int distance)
{
    assert(distance > 0);
    auto iter = parents.find(type);
    if(iter == parents.end())
    {
        parents.insert(std::make_pair(type, distance));
    }
    else if(iter->second > distance)
    {
        iter->second = distance;
    }
}

void TypeBuilder::addMember(const SymbolPtr& symbol)
{
    addMember(symbol->getName(), symbol);
}

static bool isStaticMember(const SymbolPtr& member)
{
    if(SymbolPlaceHolderPtr sym = dynamic_pointer_cast<SymbolPlaceHolder>(member))
    {
        if(sym->flags & SymbolPlaceHolder::F_STATIC)
        {
            return true;
        }
    }
    return false;
}

void TypeBuilder::addMember(const std::wstring& name, const SymbolPtr& member)
{
    if(isStaticMember(member))
    {
        staticMembers.insert(make_pair(name, member));
        return;
    }
    if(SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(member))
    {
        members.insert(std::make_pair(name, member));
        if(s->getRole() == SymbolPlaceHolder::R_PROPERTY)
        {
            //computed property
            computedProperties.push_back(s);
        }
        else
        {
            //stored property
            storedProperties.push_back(s);
        }
        return;
    }
    else if(TypePtr type = dynamic_pointer_cast<Type>(member))
    {
        members.insert(std::make_pair(name, member));
        //associated type
        associatedTypes.insert(make_pair(name, type));
        return;
    }
    auto iter = members.find(name);
    if(iter != members.end())
    {
        FunctionOverloadedSymbolPtr oldFuncs = static_pointer_cast<FunctionOverloadedSymbol>(iter->second);
        if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(member))
            oldFuncs->add(func);
        else if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(member))
            oldFuncs->add(funcs);
        else
            assert(0 && "Unknown member type");
    }
    else
    {
        if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(member))
        {
            FunctionOverloadedSymbolPtr funcs(new FunctionOverloadedSymbol(name));
            funcs->add(func);
            members.insert(make_pair(name, funcs));
            functions.push_back(funcs);
        }
        else if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(member))
        {
            members.insert(make_pair(name, funcs));
            functions.push_back(funcs);
        }
        else
        {
            member->getName();
            assert(0 && "Unknown member type");
        }
    }
}


