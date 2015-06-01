/* TypeResolver.cpp --
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
#include "semantics/TypeResolver.h"
#include "semantics/CompilerResultEmitter.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/GlobalScope.h"
#include "semantics/Type.h"
#include "semantics/SemanticContext.h"
#include "common/Errors.h"
#include "common/SwallowUtils.h"
#include "ast/ast.h"
#include <cassert>


USE_SWALLOW_NS
using namespace std;

TypeResolver::TypeResolver(SymbolRegistry* registry, CompilerResultEmitter* resultEmitter, LazySymbolResolver* symbolResolver, SemanticContext* ctx)
    :emitter(resultEmitter), symbolRegistry(registry), symbolResolver(symbolResolver), ctx(ctx)
{
}
TypePtr TypeResolver::lookupType(const TypeNodePtr& type)
{
    if(!type)
        return nullptr;
    TypePtr ret = type->getType();
    if(!ret)
    {
        ret = resolve(type);
        type->setType(ret);
    }
    return ret;
}
bool TypeResolver::assertGenericArgumentMatches(CompilerResultEmitter* emitter, const NodePtr& node, int expected, int actual)
{
    if (actual > expected)
    {
        if (emitter)
        {
            std::wstring str = SwallowUtils::toString(node);
            std::wstring got = SwallowUtils::toString(actual);
            std::wstring sexpected = SwallowUtils::toString(expected);
            emitter->error(node, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3, str, got, sexpected);
        }
        return false;
    }
    if (actual < expected)
    {
        if (emitter)
        {
            std::wstring str = SwallowUtils::toString(node);
            std::wstring got = SwallowUtils::toString(actual);
            std::wstring sexpected = SwallowUtils::toString(expected);
            emitter->error(node, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3, str, got, sexpected);
        }
        return false;
    }
    return true;
}
TypePtr TypeResolver::handleGeneric(const TypePtr& type, const TypeIdentifierPtr& id)
{
    if(type->getCategory() == Type::GenericParameter)
        return type;//do nothing for generic parameter.
    
    GenericDefinitionPtr generic = type->getGenericDefinition();
    //not a generic type
    if (generic == nullptr && id->numGenericArguments() == 0)
    {
        //load nested types
        //return Type::newGenericParameter(type->getName, nestedTypes());
        return type;
    }
    if (generic == nullptr && id->numGenericArguments() > 0)
    {
        if (emitter)
        {
            std::wstring str = SwallowUtils::toString(id);
            emitter->error(id, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, str);
        }
        return nullptr;
    }
    if (generic != nullptr && id->numGenericArguments() == 0)
    {
        if (emitter)
        {
            std::wstring str = SwallowUtils::toString(id);
            emitter->error(id, Errors::E_GENERIC_TYPE_ARGUMENT_REQUIRED, str);
        }
        return nullptr;
    }
    if(!assertGenericArgumentMatches(emitter, id, generic->numParameters(), id->numGenericArguments()))
        return nullptr;
    //check type
    GenericArgumentPtr genericArgument(new GenericArgument(generic));
    for (auto arg : *id)
    {
        TypePtr argType = lookupType(arg);
        if (!argType)
            return nullptr;
        genericArgument->add(argType);
    }
    TypePtr base = Type::newSpecializedType(type, genericArgument);
    return base;
}
TypePtr TypeResolver::resolveIdentifier(const ModulePtr& module, TypeIdentifierPtr id)
{
    TypePtr ret = nullptr;
    if(module)
    {
        ret = dynamic_pointer_cast<Type>(module->getSymbol(id->getName()));
    }
    else
    {
        ret = symbolRegistry->lookupType(id->getName());
    }
    //type is undeclared
    if(!ret)
    {
        if(emitter)
        {
            if(module)
            {
                emitter->error(id, Errors::E_NO_TYPE_NAMED_A_IN_MODULE_B_2, id->getName(), module->getName());
            }
            else
            {
                std::wstring str = SwallowUtils::toString(id);
                emitter->error(id, Errors::E_USE_OF_UNDECLARED_TYPE_1, str);
            }
        }
        return nullptr;
    }

    ret = handleGeneric(ret, id);
    if(ret->getCategory() == Type::GenericParameter)
        return ret;//nested type access is treated as generic parameter, will be handled when type specialized


    for (TypeIdentifierPtr n = id->getNestedType(); n != nullptr; n = n->getNestedType())
    {
        const std::wstring& name = n->getName();
        TypePtr childType;
        if(name == L"Type")
            childType = Type::newMetaType(ret);
        else
            childType = ret->getAssociatedType(name);
        if (!childType)
        {
            if (emitter)
                emitter->error(n, Errors::E_A_IS_NOT_A_MEMBER_TYPE_OF_B_2, name, wstring(ret->toString()));
            return nullptr;
        }
        if (n->numGenericArguments())
        {
            //nested type is always a non-generic type
            if (emitter)
                emitter->error(n, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, childType->toString());
            return nullptr;
        }
        ret = childType;
    }
    return ret;
}
TypePtr TypeResolver::resolveIdentifier(TypeIdentifierPtr id)
{
    const wstring& name = id->getName();
    symbolResolver->resolveLazySymbol(name);
    SymbolPtr s = symbolRegistry->lookupSymbol(name);
    if(s && s->getKind() == SymbolKindModule)//type declared inside a module
    {
        //it's a module, access it's type
        if(id->getNestedType() == nullptr)
        {
            if(emitter)
                emitter->error(id, Errors::E_USE_OF_UNDECLARED_TYPE_1, name);
            return nullptr;
        }
        ModulePtr module = static_pointer_cast<Module>(s);
        return resolveIdentifier(module, id->getNestedType());
    }
    //Self type
    if(name == L"Self")
    {
        if(!ctx || !ctx->currentType)
        {
            if(emitter)
                emitter->error(id, Errors::E_USE_OF_UNDECLARED_TYPE_1, name);
            return nullptr;
        }
        return ctx->currentType;
    }
    return resolveIdentifier(nullptr, id);
}
TypePtr TypeResolver::resolveTuple(const TupleTypePtr& tuple)
{
    std::vector<TypePtr> elementTypes;
    for (const TupleType::TupleElement &e : *tuple)
    {
        TypePtr t = lookupType(e.type);
        elementTypes.push_back(t);
    }
    return Type::newTuple(elementTypes);
}

TypePtr TypeResolver::resolveArray(const ArrayTypePtr& array)
{
    GlobalScope *global = symbolRegistry->getGlobalScope();
    TypePtr innerType = lookupType(array->getInnerType());
    assert(innerType != nullptr);
    TypePtr ret = global->makeArray(innerType);
    return ret;
}
TypePtr TypeResolver::resolveDictionaryType(const DictionaryTypePtr& dict)
{
    GlobalScope *scope = symbolRegistry->getGlobalScope();
    TypePtr keyType = lookupType(dict->getKeyType());
    TypePtr valueType = lookupType(dict->getValueType());
    assert(keyType != nullptr);
    assert(valueType != nullptr);
    TypePtr ret = scope->makeDictionary(keyType, valueType);
    return ret;
}
TypePtr TypeResolver::resolveOptionalType(const OptionalTypePtr& opt)
{
    GlobalScope *global = symbolRegistry->getGlobalScope();
    TypePtr innerType = lookupType(opt->getInnerType());
    assert(innerType != nullptr);
    TypePtr ret = global->makeOptional(innerType);
    return ret;
}
TypePtr TypeResolver::resolveImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptionalPtr& opt)
{
    GlobalScope *global = symbolRegistry->getGlobalScope();
    TypePtr innerType = lookupType(opt->getInnerType());
    assert(innerType != nullptr);
    TypePtr ret = global->makeImplicitlyUnwrappedOptional(innerType);
    return ret;
}
TypePtr TypeResolver::resolveFunctionType(const FunctionTypePtr& func)
{
    TypePtr retType = nullptr;
    if (func->getReturnType())
    {
        retType = lookupType(func->getReturnType());
    }
    vector<Parameter> params;
    for (auto p : *func->getArgumentsType())
    {
        TypePtr paramType = lookupType(p.type);
        params.push_back(Parameter(p.name, p.inout, paramType));
    }
    TypePtr ret = Type::newFunction(params, retType, false, nullptr);
    return ret;
}
TypePtr TypeResolver::resolveProtocolComposition(const ProtocolCompositionPtr& composition)
{
    vector<TypePtr> protocols;
    for(const TypeIdentifierPtr& p : *composition)
    {
        TypePtr protocol = lookupType(p);
        //it must be a protocol type
        assert(protocol != nullptr);
        if(protocol->getCategory() != Type::Protocol)
        {
            if(emitter)
                emitter->error(p, Errors::E_NON_PROTOCOL_TYPE_A_CANNOT_BE_USED_WITHIN_PROTOCOL_COMPOSITION_1, p->getName());
            return nullptr;
        }
        protocols.push_back(protocol);
    }
    TypePtr ret = Type::newProtocolComposition(protocols);
    return ret;
}
TypePtr TypeResolver::resolve(const TypeNodePtr& type)
{
    NodeType::T nodeType = type->getNodeType();
    switch(nodeType)
    {

        case NodeType::TypeIdentifier:
        {
            TypeIdentifierPtr id = std::static_pointer_cast<TypeIdentifier>(type);
            TypePtr ret = resolveIdentifier(id);
            if(ret)
            {
                ret = ret->resolveAlias();
                if(!ret && emitter)
                {
                    emitter->error(id, Errors::E_USE_OF_UNDECLARED_TYPE_1, SwallowUtils::toString(type));
                }
            }
            return ret;
        }
        case NodeType::TupleType:
        {
            TupleTypePtr tuple = std::static_pointer_cast<TupleType>(type);
            return resolveTuple(tuple);
        }
        case NodeType::ArrayType:
        {
            ArrayTypePtr array = std::static_pointer_cast<ArrayType>(type);
            return resolveArray(array);
        }
        case NodeType::DictionaryType:
        {
            DictionaryTypePtr dict = std::static_pointer_cast<DictionaryType>(type);
            return resolveDictionaryType(dict);
        }
        case NodeType::OptionalType:
        {
            OptionalTypePtr opt = std::static_pointer_cast<OptionalType>(type);
            return resolveOptionalType(opt);
        }
        case NodeType::ImplicitlyUnwrappedOptional:
        {
            ImplicitlyUnwrappedOptionalPtr opt = std::static_pointer_cast<ImplicitlyUnwrappedOptional>(type);
            return resolveImplicitlyUnwrappedOptional(opt);
        };
        case NodeType::FunctionType:
        {
            FunctionTypePtr func = std::static_pointer_cast<FunctionType>(type);
            return resolveFunctionType(func);
        }
        case NodeType::ProtocolComposition:
        {
            ProtocolCompositionPtr composition = static_pointer_cast<ProtocolComposition>(type);
            return resolveProtocolComposition(composition);
        };
        default:
        {
            assert(0 && "Unsupported type");
            return nullptr;
        }
    }
}
