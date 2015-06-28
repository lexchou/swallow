/* DeclarationAnalyzer_ControlFlow.cpp --
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



void DeclarationAnalyzer::visitWhileLoop(const WhileLoopPtr& node)
{
    node->getCodeBlock()->accept(this);
}
void DeclarationAnalyzer::visitForIn(const ForInLoopPtr& node)
{
    node->getCodeBlock()->accept(this);
}
void DeclarationAnalyzer::visitForLoop(const ForLoopPtr& node)
{
    ScopedCodeBlockPtr codeBlock = static_pointer_cast<ScopedCodeBlock>(node->getCodeBlock());
    SCOPED_SET(ctx->contextualType, nullptr);

    if(node->getInitializer())
    {
        ScopeGuard guard(codeBlock.get(), this);
        node->getInitializer()->accept(this);
    }
    node->getCodeBlock()->accept(this);
}
