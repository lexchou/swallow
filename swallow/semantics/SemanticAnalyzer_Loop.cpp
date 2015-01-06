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
#include "SemanticAnalyzer.h"
#include "GlobalScope.h"
#include "SymbolRegistry.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "ast/NodeSerializer.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "TypeBuilder.h"
#include "ScopedNodes.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include "ast/utils/ASTHierachyDumper.h"
#include "common/ScopedValue.h"
#include "ScopeGuard.h"

USE_SWALLOW_NS
using namespace std;



void SemanticAnalyzer::visitWhileLoop(const WhileLoopPtr& node)
{
    NodeVisitor::visitWhileLoop(node);
}
void SemanticAnalyzer::visitForIn(const ForInLoopPtr& node)
{
    ScopedCodeBlockPtr codeBlock = static_pointer_cast<ScopedCodeBlock>(node->getCodeBlock());
    SCOPED_SET(t_hint, nullptr);
    if(node->getDeclaredType())
        t_hint = lookupType(node->getDeclaredType());
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
    NodeVisitor::visitForLoop(node);
}
void SemanticAnalyzer::visitDoLoop(const DoLoopPtr& node)
{
    NodeVisitor::visitDoLoop(node);
}

