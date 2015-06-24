/* SemanticAnalyzer_Variables.cpp --
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
#include <cassert>
#include "semantics/DeclarationAnalyzer.h"
#include "common/Errors.h"
#include "semantics/SemanticContext.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/GlobalScope.h"

USE_SWALLOW_NS
using namespace std;



void SemanticAnalyzer::checkTupleDefinition(const TuplePtr& tuple, const ExpressionPtr& initializer)
{
    //this is a tuple definition, the corresponding declared type must be a tuple type
    TypeNodePtr declaredType = tuple->getDeclaredType();
    TypePtr type = lookupType(declaredType);
    if(!type)
    {
        error(tuple, Errors::E_USE_OF_UNDECLARED_TYPE_1, toString(declaredType));
        return;
    }
    if(!(type->getCategory() == Type::Tuple))
    {
        //tuple definition must have a tuple type definition
        error(tuple, Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1, toString(declaredType));
        return;
    }
    if(tuple->numElements() != type->numElementTypes())
    {
        //tuple pattern has the wrong length for tuple type '%'
        error(tuple, Errors::E_TUPLE_PATTERN_MUST_MATCH_TUPLE_TYPE_1, toString(declaredType));
        return;
    }
    //check if initializer has the same type with the declared type
    if(initializer)
    {
        TypePtr valueType = evaluateType(initializer);
        if(valueType && !Type::equals(valueType, type))
        {
            //tuple pattern has the wrong length for tuple type '%'
            //tuple types '%0' and '%1' have a different number of elements (%2 vs. %3)
            wstring expectedType = type->toString();
            wstring got = toString(valueType->numElementTypes());
            wstring expected = toString(type->numElementTypes());
            error(initializer, Errors::E_TUPLE_TYPES_HAVE_A_DIFFERENT_NUMBER_OF_ELEMENT_4, toString(declaredType), expectedType, got, expected);
            return;
        }
    }


    for(const PatternPtr& p : *tuple)
    {
        NodeType::T nodeType = p->getNodeType();
        if(nodeType != NodeType::Identifier)
        {

        }

    }
}
void SemanticAnalyzer::visitValueBinding(const ValueBindingPtr& node)
{
    PatternPtr name = node->getName();
    //tuple was already exploded in declaration analyzer
    if (name->getNodeType() != NodeType::Identifier)
        return;
    if(node->getOwner()->isReadOnly() && !node->getInitializer() && ctx->currentType == nullptr)
    {
        error(node, Errors::E_LET_REQUIRES_INITIALIZER);
        return;
    }
    //handle type inference for temporary variable
    if(node->isTemporary() && node->getInitializer())
    {
        //temporary variable always has an initializer
        TypePtr initializerType;
        TypePtr declaredType = node->getType();

        SCOPED_SET(ctx->contextualType, declaredType);
        node->getInitializer()->accept(this);
        initializerType = node->getInitializer()->getType();
        assert(initializerType != nullptr);

        if(declaredType)
        {
            //it has both type definition and initializer, then we need to check if the initializer expression matches the type annotation
            if(!initializerType->canAssignTo(declaredType))
            {
                error(node, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, initializerType->toString(), declaredType->toString());
                return;
            }
        }
        else
        {
            node->setType(initializerType);
        }
    }
    

    //add implicitly constructor for Optional
    IdentifierPtr id = static_pointer_cast<Identifier>(node->getName());
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();


    TypePtr declaredType = node->getType() ? node->getType() : lookupType(node->getDeclaredType());
    SCOPED_SET(ctx->contextualType, declaredType);
    if(!declaredType && !node->getInitializer())
    {
        error(node, Errors::E_TYPE_ANNOTATION_MISSING_IN_PATTERN);
        return;
    }
    SymbolPtr sym = currentScope->lookup(id->getIdentifier());
    assert(sym != nullptr);
    SymbolPlaceHolderPtr placeholder = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
    assert(placeholder != nullptr);
    if(declaredType)
    {
        placeholder->setType(declaredType);
    }
    ExpressionPtr initializer = node->getInitializer();
    if(initializer)
    {
        placeholder->setFlags(SymbolFlagInitializing, true);
        ExpressionPtr initializer = transformExpression(declaredType, node->getInitializer());
        node->setInitializer(initializer);
        TypePtr actualType = initializer->getType();
        assert(actualType != nullptr);
        if(declaredType)
        {
            if(!Type::equals(actualType, declaredType) && !canConvertTo(initializer, declaredType))
            {
                error(initializer, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, actualType->toString(), declaredType->toString());
                return;
            }
        }

        if(!declaredType)
            placeholder->setType(actualType);
    }
    assert(placeholder->getType() != nullptr);
    placeholder->setFlags(SymbolFlagInitializing, false);
    //optional type always considered initialized, compiler will make it has a default value nil
    TypePtr symbolType = sym->getType();
    GlobalScope* global = symbolRegistry->getGlobalScope();
    if(initializer || global->isOptional(symbolType) || global->isImplicitlyUnwrappedOptional(symbolType))
        markInitialized(placeholder);

    if (initializer)
        placeholder->setFlags(SymbolFlagHasInitializer, true);
    if(node->isTemporary())
    {
        placeholder->setFlags(SymbolFlagTemporary, true);
        markInitialized(placeholder);
    }

    if(!node->isTemporary())
    {
        //check access control level
        DeclarationType decl = ctx->currentType ? DeclarationTypeProperty : DeclarationTypeVariable;
        declarationAnalyzer->verifyAccessLevel(node->getOwner(), placeholder->getType(), decl, ComponentTypeType);
    }
    if(ctx->currentType && ctx->currentType->getCategory() == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1);
    }
}


