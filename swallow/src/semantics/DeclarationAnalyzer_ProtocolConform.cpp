/* DeclarationAnalyzer_ProtocolConform.cpp --
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
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/SemanticAnalyzer.h"
#include "ast/ast.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "common/Errors.h"
#include "semantics/TypeBuilder.h"
#include "semantics/GlobalScope.h"
#include <cassert>
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"
#include "semantics/TypeResolver.h"

USE_SWALLOW_NS
using namespace std;
/*!
 * Verify if the specified type conform to the given protocol
 */
void DeclarationAnalyzer::verifyProtocolConform(const TypePtr& type)
{
    for(const TypePtr& protocol : type->getProtocols())
    {
        verifyProtocolConform(type, protocol);
    }
}

/*!
 * Check if the actual type can conform to requirement type where actual type is declared inside an owner type
 */
static bool checkTypeConform(const TypePtr& ownerType, TypePtr requirementType, const TypePtr& actualType)
{
    if(requirementType->getCategory() == Type::Alias)
    {
        if(requirementType->getName() == L"Self")
            requirementType = ownerType;
        else
            requirementType = ownerType->getAssociatedType(requirementType->getName());
        if(requirementType == nullptr)
            return false;
    }
    Type::Category category = requirementType->getCategory();
    if(category != actualType->getCategory())
        return false;
    switch(category)
    {
        case Type::Class:
        case Type::Struct:
        case Type::Protocol:
        case Type::Enum:
        case Type::Aggregate:
            return actualType == requirementType;
        case Type::GenericParameter:
            return actualType->getName() == requirementType->getName();
        case Type::MetaType:
            return checkTypeConform(ownerType, requirementType->getInnerType(), actualType->getInnerType());
        case Type::Tuple:
        {
            if(actualType->numElementTypes() != requirementType->numElementTypes())
                return false;
            int num = actualType->numElementTypes();
            for(int i = 0; i < num; i++)
            {
                TypePtr req = requirementType->getElementType(i);
                TypePtr act = actualType->getElementType(i);
                if(!checkTypeConform(ownerType, req, act))
                    return false;
            }
            return true;
        }
        case Type::Function:
        {
            size_t num = requirementType->getParameters().size();
            if(num != actualType->getParameters().size())
                return false;
            if(!checkTypeConform(ownerType, requirementType->getReturnType(), actualType->getReturnType()))
                return false;
            auto iter = requirementType->getParameters().begin();
            auto iter2 = actualType->getParameters().begin();
            for(; iter != requirementType->getParameters().end(); iter++, iter2++)
            {
                if(iter->name != iter2->name)
                    return false;
                if(iter->inout != iter2->inout)
                    return false;
                TypePtr req = iter->type;
                TypePtr act = iter2->type;
                if(!checkTypeConform(ownerType, req, act))
                    return false;
            }
            return true;
        }
        case Type::Specialized:
        default:
            assert(0 && "Unsupported type category");
            return false;
            
    }
}

void DeclarationAnalyzer::verifyProtocolConform(const TypePtr& type, const TypePtr& protocol)
{
    for(auto entry : protocol->getDeclaredMembers())
    {
        SymbolPtr requirement = entry.second;
        if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(requirement))
        {
            //verify function
            for(auto func : *funcs)
            {
                verifyProtocolFunction(type, protocol, func);
            }
        }
        else if(FunctionSymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(requirement))
        {
            //verify function
            verifyProtocolFunction(type, protocol, func);
        }
        /*
         else if(requirement == Type::getPlaceHolder())
         {
         //verify inner type
         SymbolPtr sym = type->getAssociatedType(entry.first);
         if(!(std::dynamic_pointer_cast<Type>(sym)))
         {
         //Type %0 does not conform to protocol %1, unimplemented type %2
         error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3, type->getName(), protocol->getName(), entry.first);
         }
         }*/
        else if(TypePtr t = std::dynamic_pointer_cast<Type>(requirement))
        {
            //type can be ignored
        }
        else if(dynamic_pointer_cast<ComputedPropertySymbol>(requirement) || dynamic_pointer_cast<SymbolPlaceHolder>(requirement))
        {
            //verify computed properties
            SymbolPtr sym = type->getMember(entry.first);
            //ComputedPropertySymbolPtr sp = std::dynamic_pointer_cast<ComputedPropertySymbol>(sym);
            if(!sym || !checkTypeConform(type, requirement->getType(), sym->getType()))
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
            bool expectedSetter = requirement->hasFlags(SymbolFlagWritable);
            bool actualSetter = sym->hasFlags(SymbolFlagWritable);
            if(expectedSetter && !actualSetter)
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
        }
    }
}
void DeclarationAnalyzer::verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected)
{
    std::vector<SymbolPtr> results;
    int filter = FilterLookupInExtension | FilterRecursive;
    if(expected->hasFlags(SymbolFlagStatic))
        filter |= FilterStaticMember;
    semanticAnalyzer->getMethodsFromType(type, expected->getName(), (MemberFilter)filter, results);
    TypePtr expectedType = expected->getType();
    assert(expectedType != nullptr);
    //verify if they're the same type
    bool found = false;
    for(const SymbolPtr& func : results)
    {
        if(checkTypeConform(type, expectedType, func->getType()))
        {
            found = true;
            break;
        }
    }
    if(!found)
        error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
}
