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
#include "semantics/TypeBuilder.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include <cassert>


USE_SWALLOW_NS

using namespace std;

TypeBuilder::TypeBuilder(Category category)
:Type(category)
{
}
void TypeBuilder::setModuleName(const std::wstring& name)
{
    this->moduleName = name;
}
void TypeBuilder::setInitializer(const FunctionOverloadedSymbolPtr& initializer)
{
    members[L"init"] = initializer;
}
void TypeBuilder::setDeinit(const FunctionSymbolPtr& deinit)
{
    this->deinit = deinit;
    if(deinit)
        deinit->declaringType = self();
}

void TypeBuilder::addParameter(const Parameter& param)
{
    assert(param.type != nullptr);
    parameters.push_back(param);
}
/*!
 * Which type declared this
 */
void TypeBuilder::setDeclaringType(const TypePtr& type)
{
    this->declaringType = type;
}

void TypeBuilder::setParentType(const TypePtr &type)
{
    this->parentType = type;
}
void TypeBuilder::setInnerType(const TypePtr &type)
{
    innerType = type;
}
void TypeBuilder::setGenericArguments(const GenericArgumentPtr& arguments)
{
    genericArguments = arguments;
}
/*!
 * One generic type can be specialized to different concrete types with different generic arguments
 * This can be used to cache the varying final specialized types.
 */
void TypeBuilder::addSpecializedType(const GenericArgumentPtr& arguments, const TypePtr& type)
{
    specializations.insert(make_pair(GenericArgumentKey(arguments), type));
}
void TypeBuilder::addProtocol(const TypePtr &protocol)
{
    assert(protocol != nullptr);
    protocols.push_back(protocol);
    auto iter = parents.find(protocol);
    if(iter == parents.end())
        parents.insert(make_pair(protocol, 1));
    else
        iter->second = 1;
    addParentTypesFrom(protocol);
}
void TypeBuilder::addParentTypesFrom(const TypePtr& type)
{
    assert(type != nullptr);
    for(auto parent : type->parents)
    {
        addParentType(parent.first, parent.second + 1);
    }
    addParentType(type, 1);
}
void TypeBuilder::addParentType(const TypePtr& type, int distance)
{
    assert(type != nullptr);
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
    assert(symbol != nullptr);
    addMember(symbol->getName(), symbol);
}

void TypeBuilder::addMember(const std::wstring& name, const SymbolPtr& member)
{
    assert(!name.empty());
    assert(member != nullptr);
    member->declaringType = self();
    if(member->hasFlags(SymbolFlagStatic))
    {
        staticMembers.insert(make_pair(name, member));
        return;
    }
    if(SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(member))
    {
        members.insert(std::make_pair(name, member));
        //stored property
        storedProperties.push_back(s);
        return;
    }
    else if(ComputedPropertySymbolPtr p = dynamic_pointer_cast<ComputedPropertySymbol>(member))
    {
        members.insert(std::make_pair(name, member));
        //computed property
        computedProperties.push_back(s);
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


void TypeBuilder::addEnumCase(const std::wstring &name, const TypePtr &associatedType)
{
    assert(!name.empty());
    assert(associatedType != nullptr);
    FunctionSymbolPtr constructor;
    /*
    if(associatedType->isNil())
    {
        //create a symbol for it and save it as member
        int flags = SymbolFlagReadable | SymbolFlagHasInitializer | SymbolFlagMember | SymbolFlagStatic;
        SymbolPlaceHolderPtr symb(new SymbolPlaceHolder(name, self(), SymbolPlaceHolder::R_PROPERTY, flags));
        this->addMember(symb);
    }
    else
    */
    {
        //create a constructor for it
        assert(associatedType != nullptr && associatedType->getCategory() == Type::Tuple);
        vector<Parameter> params;
        for(int i = 0; i < associatedType->numElementTypes(); i++)
        {
            TypePtr t = associatedType->getElementType(i);
            params.push_back(Parameter(t));
        }
        TypePtr initializerType = Type::newFunction(params, self(), false, genericDefinition);
        constructor = FunctionSymbolPtr(new FunctionSymbol(name, initializerType, FunctionRoleEnumCase, nullptr));
        constructor->setFlags(SymbolFlagStatic, true);
        constructor->setFlags(SymbolFlagMember, true);
        constructor->setFlags(SymbolFlagEnumCase, true);
        static_pointer_cast<TypeBuilder>(initializerType)->setDeclaringType(self());
        constructor->declaringType = self();
        initializerType->setFlags(SymbolFlagStatic, true);
        initializerType->setFlags(SymbolFlagMember, true);
        initializerType->setFlags(SymbolFlagEnumCase, true);

    }
    EnumCase c = {name, associatedType, constructor};
    enumCases.insert(make_pair(name, c));
}
/*!
 * Add a subscript to this type
 */
void TypeBuilder::addSubscript(const Subscript& subscript)
{
    subscripts.push_back(subscript);
}
