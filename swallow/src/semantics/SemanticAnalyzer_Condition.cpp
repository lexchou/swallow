/* SemanticAnalyzer_Condition.cpp --
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
#include "semantics/InitializationTracer.h"

USE_SWALLOW_NS
using namespace std;


static bool isOptionalChainingChecking(const ExpressionPtr& expr)
{
    if(expr->getNodeType() != NodeType::Assignment)
        return false;
    AssignmentPtr assignment = static_pointer_cast<Assignment>(expr);
    if(assignment->getLHS()->getNodeType() != NodeType::ValueBindingPattern)
        return false;
    return true;
}

void SemanticAnalyzer::visitIf(const IfStatementPtr& node)
{
    GlobalScope* global = symbolRegistry->getGlobalScope();
    ExpressionPtr condition = node->getCondition();
    if(isOptionalChainingChecking(condition))
    {
        AssignmentPtr assignment = static_pointer_cast<Assignment>(condition);
        PatternPtr expr = assignment->getRHS();
        ValueBindingPatternPtr binding = static_pointer_cast<ValueBindingPattern>(assignment->getLHS());
        expr->accept(this);
        if(!global->isOptional(expr->getType()))
        {
            error(expr, Errors::E_BOUND_VALUE_IN_A_CONDITIONAL_BINDING_MUST_BE_OF_OPTIONAL_TYPE);
            return;
        }

        //ASTHierachyDumper dumper(std::wcout);
        //node->accept(&dumper);
        //create value binding
        TypePtr unpackedType = expr->getType();
        while(global->isOptional(unpackedType))
        {
            unpackedType = unpackedType->getGenericArguments()->get(0);
        }
        if(binding->getDeclaredType())
        {
            TypePtr declaredType = lookupType(binding->getDeclaredType());
            assert(declaredType != nullptr);
            //check if type is identical to initializer type
            if(!Type::equals(unpackedType, declaredType))
            {
                error(expr, Errors::E_A_IS_NOT_IDENTICIAL_TO_B_2, unpackedType->toString(), declaredType->toString());
                return;
            }
        }
        ScopedCodeBlockPtr then = static_pointer_cast<ScopedCodeBlock>(node->getThen());
        SymbolScope* scope = then->getScope();
        int symbolFlags = SymbolFlagInitialized | SymbolFlagReadable;
        if(!binding->isReadOnly())
            symbolFlags |= SymbolFlagWritable;
        //TODO: refactor here and visitValueBindings, these code can be shared
        if(IdentifierPtr id = dynamic_pointer_cast<Identifier>(binding->getBinding()))
        {
            SymbolPtr sym(new SymbolPlaceHolder(id->getIdentifier(), unpackedType, SymbolPlaceHolder::R_LOCAL_VARIABLE, symbolFlags));
            scope->addSymbol(sym);
        }
        else if(TuplePtr tuple = dynamic_pointer_cast<Tuple>(binding->getBinding()))
        {
            vector<TupleExtractionResult> results;
            vector<int> indices;
            wstring tempName = generateTempName();
            this->expandTuple(results, indices, tuple, tempName, unpackedType, binding->isReadOnly() ? AccessibilityConstant : AccessibilityVariable);
            SymbolPtr temp(new SymbolPlaceHolder(tempName, unpackedType, SymbolPlaceHolder::R_LOCAL_VARIABLE, SymbolFlagInitialized | SymbolFlagReadable | SymbolFlagTemporary));
            scope->addSymbol(temp);
            for(const TupleExtractionResult& res : results)
            {
                SymbolPtr sym(new SymbolPlaceHolder(res.name->getIdentifier(), res.type, SymbolPlaceHolder::R_LOCAL_VARIABLE, symbolFlags));
                scope->addSymbol(sym);
            }
        }
        else
        {
            assert(0 && "Invalid instance");
        }
    }
    else
    {
        condition->accept(this);
        //the condition must be conform to type BooleanType
        TypePtr condType = condition->getType();
        assert(condType != nullptr);
        if(!condType->canAssignTo(global->BooleanType()))
        {
            error(node->getCondition(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, condType->toString(), L"BooleanType");
            return;
        }
    }

    InitializationTracer tracer(ctx.currentInitializationTracer, InitializationTracer::Sequence);

    SCOPED_SET(ctx.currentInitializationTracer, &tracer);

    {
        InitializationTracer ifTracer(&tracer, InitializationTracer::Branch);
        SCOPED_SET(ctx.currentInitializationTracer, &ifTracer);
        node->getThen()->accept(this);
    }
    {
        InitializationTracer elseTracer(&tracer, InitializationTracer::Branch);
        SCOPED_SET(ctx.currentInitializationTracer, &elseTracer);
        if (node->getElse())
            node->getElse()->accept(this);
    }
}

static void checkExhausiveSwitch(SemanticAnalyzer* sa, const SwitchCasePtr& node)
{
    TypePtr conditionType = node->getControlExpression()->getType();
    //only check for enum and bool type
    TypePtr Bool = sa->getSymbolRegistry()->getGlobalScope()->Bool();
    if(conditionType->getCategory() != Type::Enum && conditionType != Bool)
        return;
    //do not check if it already has a default case
    if(node->getDefaultCase())
        return;
    if(conditionType->getCategory() == Type::Enum)
    {
        if(node->numCases() != conditionType->numEnumCases())
        {
            sa->error(node, Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE);
        }
    }
    else if(conditionType == Bool)
    {
        if(node->numCases() != 2)
        {
            sa->error(node, Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE);
        }
    }
}

void SemanticAnalyzer::visitSwitchCase(const SwitchCasePtr& node)
{
    node->getControlExpression()->accept(this);
    TypePtr conditionType = node->getControlExpression()->getType();
    assert(conditionType != nullptr);
    SCOPED_SET(ctx.contextualType, conditionType);

    checkExhausiveSwitch(this, node);

    InitializationTracer tracer(ctx.currentInitializationTracer, InitializationTracer::Sequence);
    for(const CaseStatementPtr& c : *node)
    {
        CodeBlockPtr statements = c->getCodeBlock();

        InitializationTracer caseTracer(&tracer, InitializationTracer::Branch);
        SCOPED_SET(ctx.currentInitializationTracer, &caseTracer);
        if(statements->numStatements() == 0)
        {
            error(node, Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, L"case");
            return;
        }
        c->accept(this);
    }
    {
        InitializationTracer caseTracer(&tracer, InitializationTracer::Branch);
        SCOPED_SET(ctx.currentInitializationTracer, &caseTracer);
        if (node->getDefaultCase())
        {
            if (node->getDefaultCase()->getCodeBlock()->numStatements() == 0)
            {
                error(node, Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, L"default");
                return;
            }
            node->getDefaultCase()->accept(this);
        }
    }
}

void SemanticAnalyzer::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
    //enum-case is similar to member access but also provided associated-value-enum binding's unpacking
    if(!ctx.contextualType)
    {
        //invalid contextual type
        error(node, Errors::E_NO_CONTEXTUAL_TYPE_TO_ACCESS_MEMBER_A_1, node->getName());
        return;
    }
    const EnumCase* ec = ctx.contextualType->getEnumCase(node->getName());
    if (ec == nullptr)
    {
        error(node, Errors::E_DOES_NOT_HAVE_A_MEMBER_2, ctx.contextualType->toString(), node->getName());
        return;
    }
    node->setType(ctx.contextualType);
    //TODO check for associated values for unpacking

}


void SemanticAnalyzer::visitCase(const CaseStatementPtr& node)
{
    assert(ctx.contextualType != nullptr);
    ScopedCodeBlockPtr codeBlock = static_pointer_cast<ScopedCodeBlock>(node->getCodeBlock());
    for(const CaseStatement::Condition& cond : node->getConditions())
    {
        //the pattern should be evaluated to the contextual type where the condition must evaluate to BooleanLiteralConvertible
        if(ctx.currentType && ctx.currentType->getCategory() == Type::Enum && cond.condition->getNodeType() == NodeType::Identifier)
        {
            IdentifierPtr id = static_pointer_cast<Identifier>(cond.condition);
            if(ctx.currentType->getEnumCase(id->getIdentifier()))
            {
                cond.condition->setType(ctx.currentType);
            }
            else
            {
                cond.condition->accept(this);
            }
        }
        else
            cond.condition->accept(this);
        TypePtr patternType = cond.condition->getType();
        assert(patternType != nullptr);
        if(!patternType->canAssignTo(ctx.contextualType))
        {
            error(cond.condition, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, patternType->toString(), ctx.contextualType->toString());
            break;
        }
        //create symbols that used for unpacking associated values
        if(ctx.contextualType->getCategory() == Type::Enum)
        {
            PatternAccessibility accessibility = AccessibilityUndefined;
            TuplePtr tuple = nullptr;
            const EnumCase* ec = nullptr;
            if(cond.condition->getNodeType() == NodeType::Identifier)
            {
                IdentifierPtr id = static_pointer_cast<Identifier>(cond.condition);
                ec = ctx.contextualType->getEnumCase(id->getIdentifier());
            }
            else
            {
                EnumCasePatternPtr enumCase = dynamic_pointer_cast<EnumCasePattern>(cond.condition);
                if (!enumCase)
                {
                    if (ValueBindingPatternPtr binding = dynamic_pointer_cast<ValueBindingPattern>(cond.condition))
                    {
                        enumCase = dynamic_pointer_cast<EnumCasePattern>(binding->getBinding());
                        accessibility = binding->isReadOnly() ? AccessibilityConstant : AccessibilityVariable;
                    }
                }
                assert(enumCase != nullptr);
                ec = ctx.contextualType->getEnumCase(enumCase->getName());
            }
            if(ec && ec->type != symbolRegistry->getGlobalScope()->Void())
            {
                vector<TupleExtractionResult> results;
                vector<int> indices;
                wstring tempName = generateTempName();
                this->expandTuple(results, indices, cond.condition, L"#0", ec->type, accessibility);
                for (auto var : results)
                {
                    //register symbol
                    const wstring &name = var.name->getIdentifier();
                    SymbolPlaceHolderPtr sym(new SymbolPlaceHolder(name, var.type, SymbolPlaceHolder::R_LOCAL_VARIABLE, SymbolFlagInitialized | SymbolFlagReadable));
                    codeBlock->getScope()->addSymbol(sym);
                }
            }
        }
        else
        {
            if(ValueBindingPatternPtr binding = dynamic_pointer_cast<ValueBindingPattern>(cond.condition))
            {
                PatternAccessibility accessibility = binding->isReadOnly() ? AccessibilityConstant : AccessibilityVariable;
                int flags = SymbolFlagInitialized | SymbolFlagReadable | (binding->isReadOnly() ? 0 : SymbolFlagWritable);
                if(binding->getBinding()->getNodeType() == NodeType::Tuple)
                {
                    //register as tuple
                    vector<TupleExtractionResult> results;
                    vector<int> indices;
                    wstring tempName = this->generateTempName();
                    this->expandTuple(results, indices, binding->getBinding(), tempName, ctx.contextualType, accessibility);
                    for (auto var : results)
                    {
                        //register symbol
                        const wstring &name = var.name->getIdentifier();
                        SymbolPlaceHolderPtr sym(new SymbolPlaceHolder(name, var.type, SymbolPlaceHolder::R_LOCAL_VARIABLE, flags));
                        codeBlock->getScope()->addSymbol(sym);
                    }
                }
                else
                {
                    //register as one variable
                    IdentifierPtr id = dynamic_pointer_cast<Identifier>(binding->getBinding());
                    assert(id != nullptr);
                    const wstring &name = id->getIdentifier();
                    SymbolPlaceHolderPtr sym(new SymbolPlaceHolder(name, ctx.contextualType, SymbolPlaceHolder::R_LOCAL_VARIABLE, flags));
                    codeBlock->getScope()->addSymbol(sym);
                }
            }
        }

        if(cond.guard)
        {
            ScopeGuard scope(codeBlock.get(), this);
            cond.guard->accept(this);
            TypePtr whereType = cond.guard->getType();
            assert(whereType != nullptr);
            TypePtr p_BooleanType = symbolRegistry->getGlobalScope()->BooleanType();
            if(!whereType->canAssignTo(p_BooleanType))
            {
                error(cond.guard, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, whereType->toString(), p_BooleanType->toString());
                break;
            }
        }
    }
    node->getCodeBlock()->accept(this);
}
