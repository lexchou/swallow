/* SemanticAnalyzer_TypeCreation.cpp --
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
#include "semantics/SemanticAnalyzer.h"
#include "ast/ast.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "common/Errors.h"
#include "semantics/TypeBuilder.h"
#include "semantics/GlobalScope.h"
#include "semantics/ScopeGuard.h"
#include "semantics/SemanticContext.h"
#include "semantics/DeclarationAnalyzer.h"
#include <cassert>
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"

USE_SWALLOW_NS
using namespace std;


void SemanticAnalyzer::visitImplementation(const TypeDeclarationPtr& node)
{
    vector<TypeAliasPtr> aliasNodes;
    vector<InitializerDefPtr> designatedInits;
    vector<InitializerDefPtr> convenienceInits;
    vector<DeclarationPtr> others;

    for(const DeclarationPtr& decl : *node)
    {
        switch(decl->getNodeType())
        {
            case NodeType::TypeAlias:
                aliasNodes.push_back(static_pointer_cast<TypeAlias>(decl));
                break;
            case NodeType::Init:
            {
                InitializerDefPtr init = static_pointer_cast<InitializerDef>(decl);
                if(init->hasModifier(DeclarationModifiers::Convenience))
                    convenienceInits.push_back(init);
                else
                    designatedInits.push_back(init);
                break;
            }
            default:
                others.push_back(decl);
                break;
        }
    }

    for(auto decl : aliasNodes)
    {
        decl->accept(this);
    }
    for(auto decl : designatedInits)
    {
        decl->accept(this);
    }
    //mark all stored properties initialized after designated initializer visited.
    assert(ctx->currentType);
    for(const SymbolPtr& p : ctx->currentType->getDeclaredStoredProperties())
    {
        markInitialized(p);
    }
    
    
    for(auto decl : convenienceInits)
    {
        decl->accept(this);
    }
    for(auto decl : others)
    {
        decl->accept(this);
    }
}
void SemanticAnalyzer::visitTypeAlias(const TypeAliasPtr& node)
{
    //bring type from forward declaration to current scope
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    scope->addSymbol(node->getName(), sym);
    SemanticPass::visitTypeAlias(node);
}
void SemanticAnalyzer::visitEnum(const EnumDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    scope->addSymbol(node->getIdentifier()->getName(), sym);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);

    declarationAnalyzer->verifyProtocolConform(type, true);
    visitImplementation(node);
}
void SemanticAnalyzer::visitClass(const ClassDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    scope->addSymbol(node->getIdentifier()->getName(), sym);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    declarationAnalyzer->verifyProtocolConform(type, true);
    visitImplementation(node);
}
void SemanticAnalyzer::visitStruct(const StructDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    scope->addSymbol(node->getIdentifier()->getName(), sym);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    declarationAnalyzer->verifyProtocolConform(type, true);
    visitImplementation(node);
}
void SemanticAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    SymbolPtr sym = scope->getForwardDeclaration(node->getIdentifier()->getName());
    assert(sym && sym->getKind() == SymbolKindType);
    scope->addSymbol(node->getIdentifier()->getName(), sym);
    TypePtr type = static_pointer_cast<Type>(sym);
    ScopeGuard guard(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);

    visitImplementation(node);
}
void SemanticAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    if(ctx->currentFunction || ctx->currentType)
    {
        error(node, Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, node->getIdentifier()->getName());
        return;
    }
    /*
    //check if this type is already registered
    if(ctx->lazyDeclaration)
    {
        bool defined = symbolRegistry->isSymbolDefined(node->getIdentifier()->getName());
        if(!defined)
        {
            delayDeclare(node);
            return;
        }
    }
    */
    TypePtr type;
    symbolRegistry->lookupType(node->getIdentifier()->getName(), nullptr, &type, false);
    assert(type != nullptr);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);

    declarationAnalyzer->verifyProtocolConform(type, true);
    //SemanticPass::visitExtension(node);
    visitImplementation(node);
}
void SemanticAnalyzer::visitOptionalType(const OptionalTypePtr& node)
{
    TypePtr innerType = lookupType(node->getInnerType());
    TypePtr type = symbolRegistry->getGlobalScope()->makeOptional(innerType);
    node->setType(type);
}
