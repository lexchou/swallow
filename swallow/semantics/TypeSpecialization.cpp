/* TypeSpecialization.cpp --
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
#include "Type.h"
#include "FunctionSymbol.h"
#include "FunctionOverloadedSymbol.h"
#include "GenericArgument.h"
#include "GenericDefinition.h"
#include "TypeBuilder.h"
#include <cassert>

USE_SWALLOW_NS
using namespace std;


static TypePtr specialize(const TypePtr& type, const GenericArgumentPtr& arguments)
{
    //TODO: Fix the void type is nullptr
    if(!type)
        return nullptr;
    if(!type->containsGenericParameters())
        return type;
    Type::Category category = type->getCategory();
    switch(category)
    {
        case Type::GenericParameter:
        {
            TypePtr ret = arguments->get(type->getName());
            return ret;
        }
        case Type::Function:
        case Type::Closure:
        {
            std::vector<Type::Parameter> params;
            TypePtr returnType = specialize(type->getReturnType(), arguments);
            for(auto param : type->getParameters())
            {
                TypePtr paramType = specialize(param.type, arguments);
                params.push_back(Type::Parameter(param.name, param.inout, paramType));
            }
            //Type::newFunction(<#(vector<Type::Parameter> const &)parameters#>, <#(const TypePtr&)returnType#>, <#(bool)hasVariadicParameters#>, <#(shared_ptr<GenericDefinition> const &)generic#>)
            TypePtr ret = Type::newFunction(params, returnType, type->hasVariadicParameters(), type->getGenericDefinition());
            return ret;
        }
        case Type::Specialized:
        {
            GenericArgumentPtr args(new GenericArgument(type->getGenericArguments()->getDefinition()));
            for (const TypePtr &t : *type->getGenericArguments())
            {
                TypePtr arg = specialize(t, arguments);
                args->add(arg);
            }
            TypePtr ret = Type::newSpecializedType(type->getInnerType(), args);
            return ret;
        }
        default:
            assert(0);
    }

    return nullptr;
}

static FunctionSymbolPtr specialize(const FunctionSymbolPtr& func, const GenericArgumentPtr& arguments)
{
    TypePtr funcType = specialize(func->getType(), arguments);
    FunctionSymbolPtr ret(new FunctionSymbol(func->getName(), funcType, func->getDefinition()));
    return ret;
}


TypePtr Type::newSpecializedType(const TypePtr& innerType, const GenericArgumentPtr& arguments)
{
    TypeBuilder* ret = new TypeBuilder(Specialized);
    ret->innerType = innerType;
    ret->genericArguments = arguments;
    //copy members from innerType and update types with given argument
    for(auto entry : innerType->members)
    {
        SymbolPtr sym = entry.second;
        if(TypePtr type = dynamic_pointer_cast<Type>(sym))
        {

            sym = specialize(type, arguments);
            assert(sym != nullptr);
        }
        else if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(sym))
        {
            //rebuild the symbol with specialized type
            sym = specialize(func, arguments);
        }
        else if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
        {
            FunctionOverloadedSymbolPtr newFuncs(new FunctionOverloadedSymbol());
            for(const FunctionSymbolPtr& func : *funcs)
            {
                FunctionSymbolPtr newFunc = specialize(func, arguments);
                newFuncs->add(newFunc);
            }
            sym = newFuncs;
        }
        else if(SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(sym))
        {
            TypePtr t = specialize(s->getType(), arguments);
            SymbolPlaceHolderPtr newSym(new SymbolPlaceHolder(s->getName(), t, s->getRole(), s->flags));
            sym = newSym;
        }
        ret->addMember(entry.first, sym);
    }
    //replace parents and protocols to apply with the generic arguments
    return TypePtr(ret);
}
TypePtr Type::newSpecializedType(const TypePtr& innerType, const TypePtr& argument)
{
    assert(innerType->getGenericDefinition() != nullptr);
    GenericArgumentPtr arguments(new GenericArgument(innerType->getGenericDefinition()));
    arguments->add(argument);

    TypePtr ret = newSpecializedType(innerType, arguments);
    return ret;
}



bool Type::canSpecializeTo(const TypePtr& type, std::map<std::wstring, TypePtr>& typeMap)const
{
    TypePtr self = static_pointer_cast<Type>(const_cast<Type*>(this)->shared_from_this())->unwrap();

    if(self->category == Alias)
    {
        //it's a alias, type inferece only for undefined alias
        assert(self->innerType == nullptr);

        auto iter = typeMap.find(name);
        if (iter == typeMap.end())
        {
            //this matches any type
            typeMap.insert(make_pair(name, type));
            return true;
        }
        else
        {
            //only match the defined/inference type
            if (Type::equals(iter->second, type))
                return true;
            return false;
        }
    }
    if(category != type->category)
        return false;
    switch(category)
    {
        case Aggregate:
        case MetaType:
        case Class:
        case Struct:
        case Enum:
        case Protocol:
        {
            return equals(self, type);
        }
        case Alias:
        {
            assert(0);
            return true;
        }
        case Tuple:
        {
            if(self->elementTypes.size() != type->elementTypes.size())
                return false;
            auto iter1 = self->elementTypes.begin();
            auto iter2 = type->elementTypes.begin();
            for(; iter1 != self->elementTypes.end(); iter1++, iter2++)
            {
                if(!(*iter1)->canSpecializeTo(*iter2, typeMap))
                    return false;
            }
            return true;
        }
        case Specialized:
        {
            if (!equals(innerType, type->innerType))
                return false;
            //check generic argument, every argument must be able to specialized to the corresponding argument in given type
            assert(self->genericArguments != nullptr && type->genericArguments != nullptr);
            auto iter1 = self->genericArguments->begin();
            auto iter2 = type->genericArguments->begin();
            for (; iter1 != self->genericArguments->end(); iter1++, iter2++)
            {
                if (!(*iter1)->canSpecializeTo(*iter2, typeMap))
                    return false;
            }
            return true;
        }
        case Extension:
            return false;
        case Function:
        case Closure:
        {
            if(self->parameters.size() != type->parameters.size())
                return false;
            if(!self->returnType->canSpecializeTo(type->returnType, typeMap))
                return false;
            //the type to test should not be a generic function
            if(type->genericDefinition)
                return false;
            auto iter1 = self->parameters.begin();
            auto iter2 = type->parameters.begin();
            for(; iter1 != self->parameters.end(); iter1++, iter2++)
            {
                if((iter1->name != iter2->name) || (iter1->inout != iter2->inout))
                    return false;
                if(!iter1->type->canSpecializeTo(iter2->type, typeMap))
                    return false;
            }
            return true;
        }
        case GenericParameter://Placeholder for generic type
            assert(0);
            return false;
        case Self:
            auto iter = typeMap.find(L"Self");
            if(iter == typeMap.end())
                return false;
            return Type::equals(iter->second, type);
    }
}
