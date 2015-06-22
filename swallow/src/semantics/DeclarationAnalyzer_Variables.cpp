/* DeclarationAnalyzer_Variables.cpp --
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
#include "ast/ast.h"
#include "semantics/Symbol.h"
#include "semantics/SymbolScope.h"
#include "semantics/SymbolRegistry.h"
#include "common/Errors.h"
#include "semantics/ScopedNodes.h"
#include "semantics/TypeBuilder.h"
#include "common/CompilerResults.h"
#include <cassert>
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/GlobalScope.h"
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"
#include "semantics/SemanticContext.h"
#include "semantics/SemanticAnalyzer.h"
#include "semantics/SemanticUtils.h"

USE_SWALLOW_NS
using namespace std;

void DeclarationAnalyzer::registerSymbol(const SymbolPlaceHolderPtr& symbol, const NodePtr& node)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    scope->addSymbol(symbol);
    if(!ctx->currentFunction && ctx->currentType)
    {
        declarationFinished(symbol->getName(), symbol, node);
    }
}



static void explodeValueBinding(SemanticAnalyzer* semanticAnalyzer, const ValueBindingsPtr& valueBindings, ValueBindings::Iterator& iter)
{
    ValueBindingPtr var = *iter;
    TuplePtr name = dynamic_pointer_cast<Tuple>(var->getName());
    TypePtr declaredType = var->getDeclaredType() ? semanticAnalyzer->lookupType(var->getDeclaredType()) : nullptr;
    //expand it into tuple assignment
    NodeFactory* nodeFactory = valueBindings->getNodeFactory();
    //need a temporay variable to hold the initializer
    std::wstring tempName = semanticAnalyzer->generateTempName();
    ValueBindingPtr tempVar = nodeFactory->createValueBinding(*var->getSourceInfo());
    IdentifierPtr tempVarId = nodeFactory->createIdentifier(*var->getSourceInfo());
    tempVarId->setIdentifier(tempName);
    tempVar->setName(tempVarId);
    tempVar->setInitializer(var->getInitializer());
    tempVar->setType(declaredType ? declaredType : nullptr);
    tempVar->setTemporary(true);
    valueBindings->insertBefore(tempVar, iter);
    //now expand tuples
    vector<TupleExtractionResult> result;
    vector<int> indices;
    semanticAnalyzer->expandTuple(result, indices, name, tempName, tempVar->getType(), valueBindings->isReadOnly() ? AccessibilityConstant : AccessibilityVariable);
    for(auto v : result)
    {
        if(v.name->getIdentifier() == L"_")
            continue;//ignore the placeholder
        ValueBindingPtr var = nodeFactory->createValueBinding(*v.name->getSourceInfo());
        var->setName(v.name);
        var->setType(v.type);
        var->setInitializer(v.initializer);
        valueBindings->add(var);
    }
}

/*!
 * Need to explode a tuple variable definition into a sequence of single variable definitions
 */
void DeclarationAnalyzer::explodeValueBindings(const ValueBindingsPtr& node)
{
    auto iter = node->begin();
    for(; iter != node->end(); iter++)
    {
        ValueBindingPtr var = *iter;
        TuplePtr tuple = dynamic_pointer_cast<Tuple>(var->getName());
        if(!tuple)
            continue;
        explodeValueBinding(semanticAnalyzer, node, iter);
    }
}

void DeclarationAnalyzer::visitValueBindings(const ValueBindingsPtr& node)
{
    explodeValueBindings(node);

    //this will make untyped bindings has the type in following forms:
    //let a, b, c : Int
    //a and b will both be Int
    TypeNodePtr lastType = nullptr;
    for(auto iter = node->rbegin(); iter != node->rend(); iter++)
    {
        ValueBindingPtr binding = *iter;
        if(binding->getDeclaredType())
            lastType = binding->getDeclaredType();
        else
            binding->setDeclaredType(lastType);

    }

    if(node->isReadOnly() && ctx->currentType && ctx->currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1);
        return;
    }

    int flags = SymbolFlagReadable | SymbolFlagInitializing;
    if(ctx->currentType)
        flags |= SymbolFlagMember;
    if(node->isReadOnly())
        flags |= SymbolFlagNonmutating;
    else
        flags |= SymbolFlagWritable;
    if(node->hasModifier(DeclarationModifiers::Static) || node->hasModifier(DeclarationModifiers::Class))
        flags |= SymbolFlagStatic;
    if(node->hasModifier(DeclarationModifiers::Lazy))
        flags |= SymbolFlagLazy;

    SymbolPlaceHolder::Role role = SymbolPlaceHolder::R_LOCAL_VARIABLE;
    if(!ctx->currentFunction)
    {
        if (ctx->currentType)
            role = SymbolPlaceHolder::R_PROPERTY;
        else
            role = SymbolPlaceHolder::R_TOP_LEVEL_VARIABLE;
    }

    if(role == SymbolPlaceHolder::R_PROPERTY)
        flags |= SymbolFlagStoredProperty;

    validateDeclarationModifiers(node);
    AccessLevel accessLevel = parseAccessLevel(node->getModifiers());
    for(const ValueBindingPtr& var : *node)
    {
        PatternPtr name = var->getName();
        if(name->getNodeType() != NodeType::Identifier)
            continue;//The tuple has been exploded into a sequence of single variable bindings, no need to handle tuple again
        IdentifierPtr id = std::static_pointer_cast<Identifier>(name);
        SymbolPtr s = nullptr;
        SymbolScope* scope = nullptr;
        symbolRegistry->lookupSymbol(id->getIdentifier(), &scope, &s);
        if(s && scope == symbolRegistry->getCurrentScope())
        {
            //already defined in current scope
            error(id, Errors::E_DEFINITION_CONFLICT, id->getIdentifier());
        }
        else
        {
            int vflags = flags;
            SymbolPlaceHolderPtr pattern(new SymbolPlaceHolder(id->getIdentifier(), id->getType(), role, vflags));
            pattern->setAccessLevel(accessLevel);
            registerSymbol(pattern, id);
        }
    }
    for(const ValueBindingPtr& v : *node)
    {
        PatternPtr name = v->getName();
        if (name->getNodeType() != NodeType::Identifier)
            continue;
        //skip placeholder
        IdentifierPtr id = std::static_pointer_cast<Identifier>(name);
        SymbolPtr s = symbolRegistry->lookupSymbol(id->getIdentifier());
        assert(s != nullptr);
        ExpressionPtr initializer = v->getInitializer();
        SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(s);
        assert(placeholder != nullptr);
        v->accept(this);

    }
}
