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

USE_SWALLOW_NS
using namespace std;



void SemanticAnalyzer::visitClosure(const ClosurePtr& node)
{
    node->accept(declarationAnalyzer);
}

void SemanticAnalyzer::visitAccessor(const CodeBlockPtr& accessor, const ParametersPtr& params, const SymbolPtr& setter)
{
    declarationAnalyzer->visitAccessor(accessor, params, setter);
}
void SemanticAnalyzer::visitSubscript(const SubscriptDefPtr &node)
{
    node->accept(declarationAnalyzer);
}
void SemanticAnalyzer::visitFunction(const FunctionDefPtr& node)
{
    if(lazyDeclaration && isGlobal(node))
    {
        //mark it as lazy declaration
        delayDeclare(node);
        return;
    }
    node->accept(declarationAnalyzer);
}

void SemanticAnalyzer::visitDeinit(const DeinitializerDefPtr& node)
{
    node->accept(declarationAnalyzer);
}

void SemanticAnalyzer::visitInit(const InitializerDefPtr& node)
{
    node->accept(declarationAnalyzer);
}

void SemanticAnalyzer::visitCodeBlock(const CodeBlockPtr &node)
{
    SCOPED_SET(ctx.flags, ctx.flags | SemanticContext::FLAG_PROCESS_IMPLEMENTATION | SemanticContext::FLAG_PROCESS_DECLARATION);
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
    node->accept(declarationAnalyzer);
}
