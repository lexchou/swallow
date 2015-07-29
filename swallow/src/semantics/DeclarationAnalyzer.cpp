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
#include "semantics/TypeResolver.h"
#include "semantics/SemanticContext.h"
#include <set>
#include "semantics/Symbol.h"
#include "semantics/LazyDeclaration.h"

USE_SWALLOW_NS
using namespace std;


DeclarationAnalyzer::DeclarationAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults, SemanticContext* ctx)
:SemanticPass(symbolRegistry, compilerResults), semanticAnalyzer(nullptr), ctx(ctx)
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
    GenericDefinitionPtr parentGeneric = nullptr;
    if(ctx->currentType)
        parentGeneric = ctx->currentType->getGenericDefinition();
    GenericDefinitionPtr ret(new GenericDefinition(parentGeneric));
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
        TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(Type::newGenericParameter(name));
        type->setGenericDefinition(ret);
        ret->add(name, type);
    }
    //add constraint
    for(const GenericConstraintDefPtr& constraint : params->getConstraints())
    {
        //constraint->getConstraintType()
        list<wstring> types;
        TypeIdentifierPtr typeId = constraint->getIdentifier();
        TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(ret->get(typeId->getName()));
        TypePtr expectedType = resolveType(constraint->getExpectedType(), true);

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
    return resolveType(type, false, supressErrors);
}
TypePtr DeclarationAnalyzer::resolveType(const TypeNodePtr& type, bool allowForwardDeclaration, bool supressErrors)
{
    TypeResolver resolver(symbolRegistry, supressErrors ? nullptr : this, this, ctx, allowForwardDeclaration);
    return resolver.lookupType(type);
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
void DeclarationAnalyzer::verifyAccessLevel(const DeclarationPtr& node, const TypePtr& type, DeclarationType declaration, ComponentType component)
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

static std::wstring getDeclarationName(const DeclarationPtr& node)
{
    if(FunctionDefPtr func = dynamic_pointer_cast<FunctionDef>(node))
    {
        return func->getName();
    }
    if(TypeDeclarationPtr type = dynamic_pointer_cast<TypeDeclaration>(node))
    {
        return type->getIdentifier()->getName();
    }
    if(TypeAliasPtr alias = dynamic_pointer_cast<TypeAlias>(node))
    {
        return alias->getName();
    }
    assert(0 && "Invalid declaration type");
    return L"";
}
/*!
  * Mark this declaration node as lazy declaration, it will be processed only when being used or after the end of the program.
  */
void DeclarationAnalyzer::delayDeclare(const DeclarationPtr& node)
{
    //map<wstring, list<DeclarationPtr>> lazyDeclarations;
    std::wstring name = getDeclarationName(node);
    auto iter = ctx->lazyDeclarations.find(name);
    //wprintf(L"Delay declare %S\n", name.c_str());
    if(iter == ctx->lazyDeclarations.end())
    {
        LazyDeclarationPtr decls(new LazyDeclaration());
        ctx->lazyDeclarations.insert(make_pair(name, decls));
        decls->addDeclaration(symbolRegistry, node);
        return;
    }
    iter->second->addDeclaration(symbolRegistry, node);
}
static void resolveTypeAlias(const TypePtr& type)
{
    if(type->getCategory() == Type::Alias)
    {
        type->resolveAlias();
        return;
    }
    //check nested types
    for(auto entry : type->getAssociatedTypes())
    {
        resolveTypeAlias(entry.second);
    }
}

void DeclarationAnalyzer::finalizeLazyDeclaration()
{
    ctx->lazyDeclaration = false;
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    while(!ctx->lazyDeclarations.empty())
    {
        std::wstring symbolName = ctx->lazyDeclarations.begin()->first;
        //lazyDeclarations.erase(lazyDeclarations.begin());
        declareImmediately(symbolName);
    }
    symbolRegistry->setCurrentScope(scope);
    //now we make all typealias to resolve its type
    for(auto entry : scope->getSymbols())
    {
        if(entry.second->getKind() != SymbolKindType)
            continue;
        TypePtr type = static_pointer_cast<Type>(entry.second);
        resolveTypeAlias(type);
    }
}

/*!
 * The declarations that marked as lazy will be declared immediately
 */
void DeclarationAnalyzer::declareImmediately(const std::wstring& name)
{
    auto entry = ctx->lazyDeclarations.find(name);
    if(entry == ctx->lazyDeclarations.end())
        return;
    LazyDeclarationPtr decls = entry->second;
    ctx->lazyDeclarations.erase(entry);
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();
    SymbolScope* fileScope = symbolRegistry->getFileScope();
    try
    {
        SCOPED_SET(ctx->lazyDeclaration, false);
        SymbolPtr symbol = nullptr;
        //wprintf(L"Declare immediately %S %d definitions\n", name.c_str(), decls->size());
        for(auto decl : *decls)
        {
            //wprintf(L"   fs:%p cs:%p\n", decl.fileScope, decl.currentScope);
            symbolRegistry->setCurrentScope(decl.currentScope);
            symbolRegistry->setFileScope(decl.fileScope);
            SCOPED_SET(this->ctx->currentType, nullptr);
            SCOPED_SET(this->ctx->currentFunction, nullptr);
            SCOPED_SET(this->ctx->contextualType, nullptr);
            SCOPED_SET(this->ctx->currentExtension, nullptr);
            SCOPED_SET(this->ctx->currentCodeBlock, nullptr);
            SCOPED_SET(this->ctx->currentInitializationTracer, nullptr);
            SCOPED_SET(this->ctx->flags, SemanticContext::FLAG_PROCESS_DECLARATION | SemanticContext::FLAG_PROCESS_IMPLEMENTATION);
            decl.node->accept(this);
            if(!symbol)
                symbol = decl.currentScope->lookup(name, false);

        }
        if(symbol && symbol->getKind() == SymbolKindType)
        {
            TypePtr type = static_pointer_cast<Type>(symbol);
            verifyProtocolConform(type, true);
        }
    }
    catch(...)
    {
        symbolRegistry->setCurrentScope(currentScope);
        symbolRegistry->setFileScope(fileScope);
        throw;
    }
    symbolRegistry->setCurrentScope(currentScope);
    symbolRegistry->setFileScope(fileScope);
}

bool DeclarationAnalyzer::resolveLazySymbol(const std::wstring& name)
{
    auto entry = ctx->lazyDeclarations.find(name);
    if(entry == ctx->lazyDeclarations.end())
    {
        //wprintf(L"Cannot resolve lazy symbol %S\n", name.c_str());
        /*
        for(auto entry : lazyDeclarations)
        {
            wprintf(L"%S, ", entry.first.c_str());
        }
        wprintf(L"\n");
        */
        return false;
    }
    declareImmediately(name);
    return true;
}

void DeclarationAnalyzer::visitProgram(const ProgramPtr& node)
{
    SemanticPass::visitProgram(node);
    //now we'll deal with the lazy declaration of functions and classes
    finalizeLazyDeclaration();
}
