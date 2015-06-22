/* SemanticAnalyzer_Loop.cpp --
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
#include "semantics/GlobalScope.h"
#include "semantics/SymbolRegistry.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/TypeBuilder.h"
#include "semantics/ScopedNodes.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include "ast/utils/ASTHierachyDumper.h"
#include "common/ScopedValue.h"
#include "semantics/ScopeGuard.h"
#include "semantics/SemanticContext.h"

USE_SWALLOW_NS
using namespace std;



void SemanticAnalyzer::visitWhileLoop(const WhileLoopPtr& node)
{
    node->getCondition()->accept(this);
    GlobalScope* global = symbolRegistry->getGlobalScope();
    TypePtr conditionType = node->getCondition()->getType();
    if(!conditionType->conformTo(global->BooleanType()))
    {
        error(node->getCondition(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, conditionType->toString(), L"BooleanType");
        return;
    }
    node->getCodeBlock()->accept(this);
}
void SemanticAnalyzer::visitForIn(const ForInLoopPtr& node)
{
    ScopedCodeBlockPtr codeBlock = static_pointer_cast<ScopedCodeBlock>(node->getCodeBlock());
    SCOPED_SET(ctx->contextualType, nullptr);
    if(node->getDeclaredType())
        ctx->contextualType = lookupType(node->getDeclaredType());
    node->getContainer()->accept(this);
    TypePtr sequenceType = node->getContainer()->getType();
    GlobalScope* global = symbolRegistry->getGlobalScope();
    if(!sequenceType->conformTo(global->SequenceType()))
    {
        error(node->getContainer(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, sequenceType->toString(), L"SequenceType");
        return;
    }


}
void SemanticAnalyzer::visitForLoop(const ForLoopPtr& node)
{
    ScopedCodeBlockPtr codeBlock = static_pointer_cast<ScopedCodeBlock>(node->getCodeBlock());
    if(node->getInitializer())
    {
        ScopeGuard guard(codeBlock.get(), this);
        node->getInitializer()->accept(this);
    }
    else
    {
        for(int i = 0; i < node->numInit(); i++)
        {
            node->getInit(i)->accept(this);
        }
    }
    {
        //condition and step expression should be evaluated under for statement's scope
        ScopeGuard guard(codeBlock.get(), this);

        //check condition
        node->getCondition()->accept(this);
        GlobalScope *global = symbolRegistry->getGlobalScope();
        TypePtr conditionType = node->getCondition()->getType();
        if (!conditionType->conformTo(global->BooleanType()))
        {
            error(node->getCondition(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, conditionType->toString(), L"BooleanType");
            return;
        }
        //visit step expressions
        node->getStep()->accept(this);
    }
    //visit code block
    node->getCodeBlock()->accept(this);
}
void SemanticAnalyzer::visitDoLoop(const DoLoopPtr& node)
{
    node->getCondition()->accept(this);
    GlobalScope* global = symbolRegistry->getGlobalScope();
    TypePtr conditionType = node->getCondition()->getType();
    if(!conditionType->conformTo(global->BooleanType()))
    {
        error(node->getCondition(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, conditionType->toString(), L"BooleanType");
        return;
    }
    node->getCodeBlock()->accept(this);
}

