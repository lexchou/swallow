/* DeclarationAnalyzer.cpp --
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
#include <cassert>
#include "semantics/SemanticAnalyzer.h"
#include "semantics/SymbolScope.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include <string>
#include "semantics/TypeBuilder.h"
#include "common/CompilerResults.h"
#include "semantics/SymbolRegistry.h"
#include "ast/utils/NodeSerializer.h"
#include "semantics/GlobalScope.h"
#include "ast/NodeFactory.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "common/ScopedValue.h"
#include <set>
#include <semantics/Symbol.h>

USE_SWALLOW_NS
using namespace std;


DeclarationAnalyzer::DeclarationAnalyzer(SemanticAnalyzer* analyzer, SemanticContext* ctx)
:SemanticPass(analyzer), semanticAnalyzer(analyzer), ctx(ctx)
{

}
/*!
 * Parse access level from a set of declaration modifiers
 */
AccessLevel DeclarationAnalyzer::parseAccessLevel(int modifiers)
{
    if(modifiers & DeclarationModifiers::Public)
        return AccessLevelPublic;
    if(modifiers & DeclarationModifiers::Internal)
        return AccessLevelInternal;
    if(modifiers & DeclarationModifiers::Private)
        return AccessLevelPrivate;
    // Handle default access control level
    // The access control level of a type also affects the default access level
    // of that type’s members (its properties, methods, initializers, and
    // subscripts). If you define a type’s access level as private, the default
    // access level of its members will also be private. If you define a type’s
    // access level as internal or public (or use the default access level of
    // internal without specifying an access level explicitly), the default
    // access level of the type’s members will be internal.
    if(ctx->currentType && ctx->currentType->getAccessLevel() == AccessLevelPrivate)
        return AccessLevelPrivate;
    return AccessLevelInternal;
}



void DeclarationAnalyzer::prepareParameters(SymbolScope* scope, const ParametersNodePtr& params)
{
    //check and prepare for parameters

    for(const ParameterNodePtr& param : *params)
    {
        assert(param->getType() != nullptr);
        SymbolPtr sym = scope->lookup(param->getLocalName());
        if(sym)
        {
            error(param, Errors::E_DEFINITION_CONFLICT, param->getLocalName());
            return;
        }
        int flags = SymbolFlagInitialized | SymbolFlagReadable;
        if(param->getAccessibility() == ParameterNode::Variable || param->isInout())
            flags |= SymbolFlagWritable;
        sym = SymbolPtr(new SymbolPlaceHolder(param->getLocalName(), param->getType(), SymbolPlaceHolder::R_PARAMETER, flags));
        scope->addSymbol(sym);
    }
    //prepare for implicit parameter self

}

GenericDefinitionPtr DeclarationAnalyzer::prepareGenericTypes(const GenericParametersDefPtr& params)
{
    GenericDefinitionPtr ret(new GenericDefinition());
    for (const TypeIdentifierPtr &typeId : *params)
    {
        if (typeId->getNestedType())
        {
            error(typeId->getNestedType(), Errors::E_NESTED_TYPE_IS_NOT_ALLOWED_HERE);
            continue;
        }
        std::wstring name = typeId->getName();
        TypePtr old = ret->get(name);
        if (old != nullptr)
        {
            error(typeId, Errors::E_DEFINITION_CONFLICT);
            continue;
        }
        std::vector<TypePtr> protocols;
        TypePtr type = Type::newType(name, Type::GenericParameter, nullptr, nullptr, protocols);
        ret->add(name, type);
    }
    //add constraint
    for(const GenericConstraintDefPtr& constraint : params->getConstraints())
    {
        //constraint->getConstraintType()
        list<wstring> types;
        TypeIdentifierPtr typeId = constraint->getIdentifier();
        TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(ret->get(typeId->getName()));
        TypePtr expectedType = lookupType(constraint->getExpectedType());

        if(type == nullptr)
        {
            error(typeId, Errors::E_USE_OF_UNDECLARED_TYPE_1, typeId->getName());
            continue;
        }
        if(constraint->getConstraintType() == GenericConstraintDef::EqualsTo)
        {
            //Same-type requirement makes generic parameter 'T' non-generic
            if(typeId->getNestedType() == nullptr)
            {
                error(typeId, Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1, typeId->getName());
                continue;
            }
            //check if the reference type is a protocol that contains Self or associated type
            if(expectedType->getCategory() == Type::Protocol)
            {
                if(expectedType->containsSelfType() || expectedType->containsAssociatedType())
                {
                    error(typeId, Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1, expectedType->getName());
                    continue;
                }
            }
        }
        types.push_back(typeId->getName());
        typeId = typeId->getNestedType();
        while(typeId != nullptr)
        {
            wstring name = typeId->getName();
            types.push_back(name);
            TypePtr childType = type->getAssociatedType(name);
            if(!childType)
            {
                //childType = Type::newType(name, Type::Placeholder, nullptr);
                //type->getSymbols()[name] = childType;
                error(typeId, Errors::E_IS_NOT_A_MEMBER_OF_2, name, type->getName());
                return ret;
            }
            type = static_pointer_cast<TypeBuilder>(childType);
            typeId = typeId->getNestedType();
        }
        if(expectedType->getCategory() == Type::Protocol)
        {
            type->addProtocol(expectedType);
        }
        else
        {
            //it's a base type
            if(type->getParentType() != nullptr)
            {
                error(constraint->getIdentifier(), Errors::E_MULTIPLE_INHERITANCE_FROM_CLASS_2_, type->getParentType()->getName(), expectedType->getName());
                continue;
            }
            type->setParentType(expectedType);
        }

        //ret->addConstraint(types);
        GenericDefinition::ConstraintType ct = constraint->getConstraintType() == GenericConstraintDef::AssignableTo ? GenericDefinition::AssignableTo : GenericDefinition::EqualsTo;
        ret->addConstraint(types, ct, expectedType);
    }

    return ret;
}


TypePtr DeclarationAnalyzer::lookupType(const TypeNodePtr& type, bool supressErrors)
{
    return semanticAnalyzer->lookupType(type, supressErrors);
}


/*!
 * Declaration finished, added it as a member to current type or current type extension.
 */
void DeclarationAnalyzer::declarationFinished(const std::wstring& name, const SymbolPtr& decl, const NodePtr& node)
{
    semanticAnalyzer->declarationFinished(name, decl, node);
}

/*!
 * Validate modifiers for declarations.
 */
void DeclarationAnalyzer::validateDeclarationModifiers(const DeclarationPtr& declaration)
{
    semanticAnalyzer->validateDeclarationModifiers(declaration);
}

static wstring accessLevelToString(AccessLevel accessLevel)
{
    switch(accessLevel)
    {
        case AccessLevelInternal:
            return L"internal";
        case AccessLevelPrivate:
            return L"private";
        case AccessLevelPublic:
            return L"public";
        default:
            break;
    }
    return L"<error>";
}

/*!
 * Verify access level
 */
void DeclarationAnalyzer::verifyAccessLevel(const DeclarationPtr& node, const TypePtr& type, int declaration, int component)
{
    int modifiers = node->getModifiers();
    AccessLevel currentLevel = parseAccessLevel(modifiers);
    AccessLevel expectedLevel = type->getAccessLevel();
    if(currentLevel <= expectedLevel)
        return;

    static const wchar_t* decls[] = {L"Variable", L"Property", L"Function", L"Method", L"Subscript", L"Initializer", L"Class", L"Enum"};
    static const wchar_t* comps[] = {L"type", L"superclass", L"parameter", L"result", L"index", L"element type", L"raw type"};
    const wchar_t* sdecl = decls[declaration];
    const wchar_t* scomp = comps[component];
    wstring cur = accessLevelToString(currentLevel);
    wstring parent = accessLevelToString(expectedLevel);
    bool implicitAccessLevel = (modifiers & DeclarationModifiers::AccessModifiers) == 0;

    if(implicitAccessLevel)
        error(node, Errors::E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4, sdecl, parent, scomp, parent);
    else
        error(node, Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4, sdecl, cur, scomp, parent);
}
