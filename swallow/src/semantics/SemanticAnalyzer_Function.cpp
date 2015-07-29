/* SemanticAnalyzer_Function.cpp --
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
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/TypeBuilder.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/SymbolScope.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/GlobalScope.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "common/ScopedValue.h"
#include <set>
#include <cassert>
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/SemanticContext.h"
#include "semantics/ReturnStatementValidator.h"
#include "semantics/InitializerValidator.h"
#include "semantics/InitializationTracer.h"

USE_SWALLOW_NS
using namespace std;

void SemanticAnalyzer::checkForFunctionOverriding(const std::wstring& name, const FunctionSymbolPtr& decl, const DeclarationPtr& node)
{
    if(!ctx->currentType)
        return;//not for global function
    if(node->hasModifier(DeclarationModifiers::_Generated))
        return;//do not check for generated functions
    bool staticMember = decl->hasFlags(SymbolFlagStatic);
    //check if there's the same signature exists
    {
        std::vector<SymbolPtr> funcs;
        //std::wstring typeName = ctx.currentType->getName();
        getMethodsFromType(ctx->currentType, name, (MemberFilter)((staticMember ? FilterStaticMember : 0) | FilterLookupInExtension ), funcs);
        for(const SymbolPtr& f : funcs)
        {
            if(f == decl)
                continue;
            if(Type::equals(f->getType(), decl->getType()))
            {
                error(node, Errors::E_INVALID_REDECLARATION_1, name);
                return;
            }
        }
    }

    //check if it override
    if(ctx->currentType->getParentType())
    {
        std::vector<SymbolPtr> funcs;
        getMethodsFromType(ctx->currentType->getParentType(), name, (MemberFilter)((staticMember ? FilterStaticMember : 0) | (FilterLookupInExtension | FilterRecursive)), funcs);
        SymbolPtr matched = nullptr;
        for(const SymbolPtr& func : funcs)
        {
            if(Type::equals(func->getType(), decl->getType()))
            {
                matched = func;
                break;
            }
        }
        assert(node != nullptr);
        {
            if(ctx->currentExtension && matched)
            {
                error(node, Errors::E_DECLARATIONS_IN_EXTENSIONS_CANNOT_OVERRIDE_YET);
                return;
            }
            if (node->hasModifier(DeclarationModifiers::Override))
            {
                if (!matched)
                {
                    error(node, Errors::E_METHOD_DOES_NOT_OVERRIDE_ANY_METHOD_FROM_ITS_SUPERCLASS);
                    return;
                }
                if(matched->hasFlags(SymbolFlagFinal))
                {
                    error(node, Errors::E_INSTANCE_METHOD_OVERRIDES_A_FINAL_INSTANCE_METHOD);
                    return;
                }
            }
            else
            {
                bool supress = false;
                //it's ok to use keyword 'required' without 'override' on an initializer
                if(decl->getType()->hasFlags(SymbolFlagRequired) && matched && matched->getType()->hasFlags(SymbolFlagRequired))
                    supress = true;
                if (matched && !supress)
                {
                    if(matched->getType()->hasFlags(SymbolFlagRequired))
                        error(node, Errors::E_REQUIRED_MODIFIER_MUST_BE_PRESENT_ON_ALL_OVERRIDES_OF_A_REQUIRED_INITIALIZER);
                    else
                        error(node, Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
                    return;
                }
            }
        }
    }
}



void SemanticAnalyzer::visitClosure(const ClosurePtr& node)
{
    ScopedClosurePtr closure = std::static_pointer_cast<ScopedClosure>(node);

    //check contextual type
    if(ctx->contextualType && ctx->contextualType->getCategory() != Type::Function)
    {
        error(node, Errors::E_FUNCTION_PROCEDURES_EXPECTD_TYPE_A_DID_YOU_MEAN_TO_CALL_IT_WITH_1, ctx->contextualType->toString());
        return;
    }

    TypePtr returnedType = nullptr;
    if(node->getReturnType())
        returnedType = declarationAnalyzer->resolveType(node->getReturnType(), true);
    else if(ctx->contextualType)
        returnedType = ctx->contextualType->getReturnType();
    std::vector<Parameter> params;
    bool variadic = false;
    if(node->getParameters())
    {
        variadic = node->getParameters()->isVariadicParameters();
        //infer parameter types if they're ignored
        TypePtr contextualType = ctx->contextualType;
        int i = 0;
        for(ParameterNodePtr param : *node->getParameters())
        {
            if(param->getDeclaredType() == nullptr)
            {
                assert(contextualType != nullptr && contextualType->getCategory() == Type::Function);
                TypePtr paramType = contextualType->getParameters()[i].type;
                assert(paramType != nullptr);
                param->setType(paramType);
            }
            i++;
        }

        node->getParameters()->accept(declarationAnalyzer);
        declarationAnalyzer->prepareParameters(closure->getScope(), node->getParameters());

        //create a function type for this
        for(const ParameterNodePtr& param : *node->getParameters())
        {
            TypePtr type = param->getType();//lookupType(param->getDeclaredType());
            assert(type != nullptr);
            const std::wstring& name = param->isShorthandExternalName() ? param->getLocalName() : param->getExternalName();
            params.push_back(Parameter(name, param->isInout(), type));
        }
    }
    TypePtr closureType = Type::newFunction(params, returnedType, variadic);
    node->setType(closureType);


    if(node->getCapture())
    {
        node->getCapture()->accept(this);
    }
}

void SemanticAnalyzer::visitSubscript(const SubscriptDefPtr &node)
{
    //TODO: validate the getter/setter if it's already exists
}

void SemanticAnalyzer::visitFunction(const FunctionDefPtr& node)
{
    FunctionSymbolPtr func = static_pointer_cast<SymboledFunction>(node)->symbol;
    checkForFunctionOverriding(func->getName(), func, node);
}

void SemanticAnalyzer::visitDeinit(const DeinitializerDefPtr& node)
{
    //TODO: validate the deinit if it's already exists
    //FunctionSymbolPtr func = static_pointer_cast<SymboledFunction>(node)->symbol;
}

void SemanticAnalyzer::visitInit(const InitializerDefPtr& node)
{
    FunctionSymbolPtr init = static_pointer_cast<SymboledInit>(node)->symbol;
    checkForFunctionOverriding(init->getName(), init, node);
}

void SemanticAnalyzer::visitCodeBlock(const CodeBlockPtr &node)
{
    SCOPED_SET(ctx->flags, ctx->flags | SemanticContext::FLAG_PROCESS_IMPLEMENTATION | SemanticContext::FLAG_PROCESS_DECLARATION);
    auto iter = node->begin();
    for(; iter != node->end(); iter++)
    {
        StatementPtr st = *iter;
        if(BinaryOperatorPtr op = dynamic_pointer_cast<BinaryOperator>(st))
            *iter = transformExpression(nullptr, op);
        st->accept(this);
    }
}


void SemanticAnalyzer::visitComputedProperty(const ComputedPropertyPtr& node)
{
    //TODO: validate the getter/setter/willSet/didSet if it's already exists
}
