/* Type.cpp --
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
#include "Type.h"
#include <cassert>
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "FunctionOverloadedSymbol.h"
#include "TypeBuilder.h"
#include <sstream>

USE_SWALLOW_NS

using namespace std;

Type::Type(Category category)
:category(category)
{
    _containsSelfType = -1;
    variadicParameters = false;
    inheritantDepth = 0;
}

bool Type::equals(const TypePtr& lhs, const TypePtr& rhs)
{
    if(lhs == rhs)
        return true;
    if(lhs == nullptr || rhs == nullptr)
        return false;
    return *lhs == *rhs;
}
/*!
 * A type place holder for protocol's typealias
 */
const TypePtr& Type::getPlaceHolder()
{
    static TypePtr placeholder(new TypeBuilder(Type::GenericParameter));
    return placeholder;
}

TypePtr Type::newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference, const TypePtr& parentType, const std::vector<TypePtr>& protocols, const GenericDefinitionPtr& generic)
{
    assert(!name.empty());
    TypeBuilder* ret = new TypeBuilder(category);
    ret->name = name;
    ret->reference = reference;
    ret->parentType = parentType;
    ret->protocols = protocols;
    ret->genericDefinition = generic;
    if(parentType)
        ret->addParentTypesFrom(parentType);
    for(const TypePtr& t : protocols)
    {
        ret->addParentTypesFrom(t);
    }
    if(parentType)
        ret->inheritantDepth = parentType->inheritantDepth + 1;
    return TypePtr(ret);
}

TypePtr Type::newTypeReference(const TypePtr& innerType)
{
    TypePtr ret(new TypeBuilder(MetaType));
    ret->innerType = innerType;
    return ret;
}


TypePtr Type::newTuple(const std::vector<TypePtr>& types)
{
    Type* ret = new TypeBuilder(Tuple);
    ret->elementTypes = types;
    return TypePtr(ret);
}

TypePtr Type::newFunction(const std::vector<Parameter>& parameters, const TypePtr& returnType, bool hasVariadicParameters, const GenericDefinitionPtr& generic)
{
    assert(returnType != nullptr);
    Type* ret = new TypeBuilder(Function);
    ret->parameters = parameters;
    ret->returnType = returnType;
    ret->variadicParameters = hasVariadicParameters;
    ret->genericDefinition = generic;
    return TypePtr(ret);
}
/*!
 * Gets the common parent class between current class and rhs with the minimum inheritance distance.
 */
TypePtr Type::getCommonParent(const TypePtr& rhs)
{
    if(rhs == nullptr || this == rhs.get())
        return rhs;
    TypePtr a;
    if(parentType)
        a = parentType->getCommonParent(rhs);

    TypePtr pthis = self();
    TypePtr b = rhs->getCommonParent(parentType);
    if(a && b)
    {
        if(a->inheritantDepth > b->inheritantDepth)
            return a;
        else
            return b;
    }
    if(a)
        return a;
    if(b)
        return b;
    return nullptr;
}



Type::Category Type::getCategory()const
{
    return category;
}


/*!
 * return true if the type is a class/struct/protocol/enum
 */
bool Type::isObjectType()const
{
    return category == Class || category == Struct || category == Enum || category == Protocol;
}

bool Type::isValueType()const
{
    switch(category)
    {
        case Tuple:
        case Struct:
            //TODO: String is not value type
            return true;
        case Aggregate:
            if(name == L"String")
                return false;
            return true;
        default:
            return false;
    }
}

bool Type::isNil() const
{
    return category == Tuple && elementTypes.empty();
}


const std::wstring& Type::getName()const
{
    return name;
}

const std::wstring& Type::getFullName() const
{
    return fullName;
}
TypePtr Type::getParentType()const
{
    return parentType;
}

const std::vector<TypePtr>& Type::getProtocols() const
{
    return protocols;
}

const std::wstring& Type::getModuleName() const
{
    return moduleName;
}
TypePtr Type::getElementType() const
{
    return getElementType(0);
}
TypePtr Type::getElementType(int index) const
{
    if(index >= (int)elementTypes.size() || index < 0)
        return nullptr;
    return elementTypes[index];
}
int Type::numElementTypes()const
{
    return elementTypes.size();
}

int Type::numEnumCases()const
{
    return enumCases.size();
}

const EnumCase* Type::getEnumCase(const std::wstring &name) const
{
    auto iter = enumCases.find(name);
    if(iter == enumCases.end())
        return nullptr;
    return &iter->second;
}

TypeDeclarationPtr Type::getReference()const
{
    if(reference.expired())
        return nullptr;
    return reference.lock();
}

TypePtr Type::getInnerType() const
{
    return innerType;
}


TypePtr Type::getReturnType() const
{
    return returnType;
}


const std::vector<Type::Parameter>& Type::getParameters() const
{
    return parameters;
}

bool Type::hasVariadicParameters()const
{
    return variadicParameters;
}

bool Type::isGenericType()const
{
    return this->category != Specialized && genericDefinition != nullptr;
}

const GenericArgumentPtr& Type::getGenericArguments() const
{
    return genericArguments;
}
const GenericDefinitionPtr& Type::getGenericDefinition() const
{
    return genericDefinition;
}

FunctionOverloadedSymbolPtr Type::getInitializer()
{
    return initializer;
}

FunctionOverloadedSymbolPtr Type::getSubscript()
{
    SymbolPtr s = getMember(L"subscript");
    FunctionOverloadedSymbolPtr ret = dynamic_pointer_cast<FunctionOverloadedSymbol>(s);
    return ret;
}


bool Type::isKindOf(const TypePtr &protocolOrBase) const
{
    assert(protocolOrBase != nullptr);
    if(protocolOrBase->getCategory() != Class && protocolOrBase->getCategory() != Protocol)
        return false;
    auto iter = parents.find(protocolOrBase);
    return iter != parents.end();
}

static bool isGenericDefinitionEquals(const GenericDefinitionPtr& a, const GenericDefinitionPtr& b)
{
    if(a == b)
        return true;
    if(a == nullptr || b == nullptr)
        return false;
    if(a->equals(b))
        return true;
    return false;
}

TypePtr Type::getType()
{
    return nullptr;
}


SymbolPtr Type::getMember(const std::wstring& name) const
{
    SymbolPtr ret = getDeclaredMember(name);
    //look for directly declared member
    if(ret)
        return ret;
    //check in base type
    if(category != Protocol && category != GenericParameter)
    {
        if(parentType != nullptr)
        {
            ret = parentType->getMember(name);
            if(ret)
                return ret;
        }
        //TODO: check from extensions
        return nullptr;
    }
    else
    {
        //look from all protocols
        for(auto entry : parents)
        {
            ret = entry.first->getDeclaredMember(name);
            if(ret)
                return ret;
        }
        return nullptr;
    }
}
SymbolPtr Type::getDeclaredStaticMember(const std::wstring& name)const
{
    auto iter = staticMembers.find(name);
    if(iter == staticMembers.end())
        return nullptr;
    return iter->second;
}
SymbolPtr Type::getDeclaredMember(const std::wstring& name) const
{
    auto iter = members.find(name);
    if(iter == members.end())
        return nullptr;
    return iter->second;
}
const Type::SymbolMap& Type::getDeclaredMembers() const
{
    return members;
}

bool Type::containsSelfType() const
{
    if (_containsSelfType != -1)
        return _containsSelfType == 1;
    bool ret = containsSelfTypeImpl();
    _containsSelfType = ret ? 1 : 0;
    return ret;
}
bool Type::containsSelfTypeImpl() const
{
    if(category == Self)
        return true;
    if(category == Function || category == Closure)
    {
        if(returnType && returnType->containsSelfType())
            return true;
        //check parameter type
        for(auto param : parameters)
        {
            if(param.type->containsSelfType())
                return true;
        }
    }
    else if(category == Specialized)
    {
        if(innerType->containsSelfType())
            return true;
        if(genericDefinition)
        {
            for(const GenericDefinition::Parameter& param : genericDefinition->getParameters())
            {
                if(param.type->containsSelfType())
                    return true;
            }
        }

    }
    else if(category == Tuple)
    {
        for(const TypePtr& t : elementTypes)
        {
            if(t->containsSelfType())
                return true;
        }
    }
    else
    {
        //check all symbols
        for(auto member : members)
        {
            if(TypePtr type = dynamic_pointer_cast<Type>(member.second))
            {
                if(type->containsSelfType())
                    return true;
            }
            else if(TypePtr type = member.second->getType())
            {
                if(type->containsSelfType())
                    return true;
            }
        }
    }
    return false;
}

bool Type::containsAssociatedType() const
{
    return !associatedTypes.empty();
}
/*!
 * Check if this type's generic arguments contains generic parameters
 */
bool Type::containsGenericParameters() const
{
    //TODO: this can be optimized
    if(category == GenericParameter)
        return true;
    if(this->genericDefinition && category != Specialized)//it is already unspecialized type, means it always cotnains a generic parameter
        return true;
    for(const Parameter& param : parameters)
    {
        if(param.type->containsGenericParameters())
            return true;
    }
    if(returnType && returnType->containsGenericParameters())
        return true;
    if(genericArguments == nullptr)
        return false;
    for(const TypePtr& type : *this->genericArguments)
    {
        if(type->category == GenericParameter)
            return true;
        if(type->containsGenericParameters())
            return true;
    }
    return false;
}
TypePtr Type::unwrap() const
{
    TypePtr node = self();
    while(node->category == Alias && node->innerType)
    {
        node = node->innerType;
    }
    return node;
}
TypePtr Type::self() const
{
    TypePtr ret = static_pointer_cast<Type>(const_cast<Type*>(this)->shared_from_this());
    return ret;
}
TypePtr Type::getAssociatedType(const std::wstring& name) const
{
    SymbolPtr symbol = getMember(name);
    if(symbol == nullptr)
        return nullptr;
    TypePtr ret = dynamic_pointer_cast<Type>(symbol);
    return ret;
}
TypePtr Type::getDeclaredAssociatedType(const std::wstring& name) const
{
    SymbolPtr symbol = getDeclaredMember(name);
    if(symbol == nullptr)
        return nullptr;
    TypePtr ret = dynamic_pointer_cast<Type>(symbol);
    return ret;
}
const std::map<std::wstring, TypePtr> Type::getAssociatedTypes() const
{
    return associatedTypes;
}
const std::vector<SymbolPtr>& Type::getDeclaredStoredProperties() const
{
    return storedProperties;
}
const std::vector<FunctionOverloadedSymbolPtr>& Type::getDeclaredFunctions() const
{
    return functions;
}
const std::map<TypePtr, int>& Type::getAllParents() const
{
    return parents;
}

bool Type::canAssignTo(const TypePtr &type) const
{
    if(type == nullptr)
        return false;
    TypePtr self = this->self();
    if(equals(self, type))
        return true;
    if(type->getCategory() == Protocol || type->getCategory() == Class)
    {
        if(this->category == Type::Specialized)
            self = innerType;
        if(self->parents.find(type) != parents.end())
        {
            return true;
        }
        return false;
    }
    /*
    if(type->getCategory() != category)
        return false;
    if(category == Tuple)
    {
        //In tuple type, each element type must can be assigned to corresponding element type in given argument
        if(elementTypes.size() != type->elementTypes.size())
            return false;
        auto iter1 = elementTypes.begin();
        auto iter2 = type->elementTypes.begin();
        for(; iter1 != elementTypes.end(); iter1++, iter2++)
        {
            if(!(*iter1)->canAssignTo(*iter2))
            {
                return false;
            }
        }
    }
    */
    return *this == *type;
}

bool Type::operator ==(const Type& rhs)const
{
    if(category != rhs.category)
        return false;
    if(moduleName != rhs.moduleName)
        return false;
    switch(category)
    {
        case Aggregate:
        case Class:
        case Struct:
        case Protocol:
        case Extension:
        case Enum:
            //check name
            if (fullName != rhs.fullName)
                return false;
            if(name != rhs.name)
                return false;
            if(getReference() != rhs.getReference())
                return false;
            if(!isGenericDefinitionEquals(genericDefinition, rhs.genericDefinition))
                return false;
            break;
        case Alias:
        {
            TypePtr a = this->unwrap();
            TypePtr b = rhs.unwrap();
            if(a->category == Alias)
                a = a->innerType;
            if(b->category == Alias)
                b = b->innerType;
            return equals(a, b);
        }
        case Tuple:
        {
            if (elementTypes.size() != rhs.elementTypes.size())
                return false;
            auto iter = elementTypes.begin(), iter2 = rhs.elementTypes.begin();
            for (; iter != elementTypes.end(); iter++, iter2++)
            {
                if (!Type::equals(*iter, *iter2))
                    return false;
            }
            break;
        }
        case MetaType:
            if(*innerType != *rhs.innerType)
                return false;
            break;
        case Function:
        case Closure:
        {
            if(!Type::equals(returnType, rhs.returnType))
                return false;
            if(parameters.size() != rhs.parameters.size())
                return false;
            if(variadicParameters != rhs.variadicParameters)
                return false;
            if(!isGenericDefinitionEquals(genericDefinition, rhs.genericDefinition))
                return false;
            auto iter = parameters.begin(), iter2 = rhs.parameters.begin();
            for(; iter != parameters.end(); iter++, iter2++)
            {
                if(iter->name != iter2->name)
                    return false;
                if(iter->inout != iter2->inout)
                    return false;
                if(*iter->type != *iter2->type)
                    return false;
            }
            break;
        }
        case Specialized:
        {
            if (!Type::equals(innerType, rhs.innerType))
                return false;
            if(genericArguments != nullptr && rhs.genericArguments != nullptr)
            {
                if (genericArguments->size() != rhs.genericArguments->size())
                    return false;
                auto iter = genericArguments->begin();
                auto iter2 = rhs.genericArguments->begin();
                for (; iter != genericArguments->end(); iter++, iter2++)
                {
                    if (!Type::equals(*iter, *iter2))
                        return false;
                }
            }
            if(genericArguments == nullptr && rhs.genericArguments == nullptr)
                return true;
            return false;
        }
        case GenericParameter:
            return true;
        case Self:
            return true;
    }

    return true;
}

bool Type::operator !=(const Type& rhs)const
{
    return !this->operator==(rhs);
}


/*!
 * convert this type to string representation
 */
std::wstring Type::toString() const
{
    using namespace std;
    switch(category)
    {

        case Aggregate:
        case GenericParameter://Placeholder for generic type
        case Class:
        case Struct:
        case Enum:
        case Protocol:
        case Extension:
        case MetaType:
        case Alias:
            return name;
        case Tuple:
        {
            wstringstream s;
            s<<L"(";
            bool first = true;
            for(const TypePtr& t : elementTypes)
            {
                if(!first)
                    s<<L", ";
                first = false;
                s<<t->toString();
            }
            s<<L")";
            return s.str();
        }

        case Specialized:
        {
            wstringstream s;
            s<<innerType->getName()<<L"<";
            bool first = true;
            for(const TypePtr& t : *genericArguments)
            {
                if(!first)
                    s<<L", ";
                first = false;
                s<<t->toString();
            }
            s<<L">";
            return s.str();
        }
        case Function:
        case Closure:
        {
            wstringstream s;
            s<<L"(";
            bool first = true;
            for(const Parameter& t : parameters)
            {
                if(!first)
                    s<<L", ";
                first = false;
                if(t.inout)
                    s<<L"inout ";
                if(!t.name.empty())
                    s<<t.name<<L" ";
                s<<t.type->toString();
            }
            s<<L") -> ";
            s<<returnType->toString();
            return s.str();
        }
        case Self:// A fake place holder, protocol use this type to present the final type that conform to the protocol
            return L"Self";
    }
    return L"<invalid-type>";
}
