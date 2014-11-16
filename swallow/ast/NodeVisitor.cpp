/* NodeVisitor.cpp --
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
#include "NodeVisitor.h"
#include "ast.h"
USE_SWALLOW_NS


#define ACCEPT(node) do { \
        Node* n = node.get(); \
        if(n) { \
            n->accept(this); \
        } \
    } while(0);


void NodeVisitor::visitDeinit(const DeinitializerDefPtr& node)
{
    ACCEPT(node->getBody());
}

void NodeVisitor::visitInit(const InitializerDefPtr& node)
{
    ACCEPT(node->getBody());
}

void NodeVisitor::visitImport(const ImportPtr& node)
{

}

void NodeVisitor::visitSubscript(const SubscriptDefPtr& node)
{
    ACCEPT(node->getGetter());
    ACCEPT(node->getSetter());
}

void NodeVisitor::visitTypeAlias(const TypeAliasPtr& node)
{
}

void NodeVisitor::visitForIn(const ForInLoopPtr& node)
{
    ACCEPT(node->getContainer());
    ACCEPT(node->getCodeBlock());
}

void NodeVisitor::visitForLoop(const ForLoopPtr& node)
{
    for(ExpressionPtr init : node->inits)
    {
        ACCEPT(init);
    }
    ACCEPT(node->condition);
    ACCEPT(node->step);
    ACCEPT(node->codeBlock);
}

void NodeVisitor::visitDoLoop(const DoLoopPtr& node)
{
    ACCEPT(node->getCodeBlock());
    ACCEPT(node->getCondition());
}

void NodeVisitor::visitLabeledStatement(const LabeledStatementPtr& node)
{
    ACCEPT(node->getStatement());
}

void NodeVisitor::visitOperator(const OperatorDefPtr& node)
{

}

void NodeVisitor::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    for(ExpressionPtr expr : node->elements)
    {
        ACCEPT(expr);
    }
}

void NodeVisitor::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    for(auto entry : *node)
    {
        ACCEPT(entry.first);
        ACCEPT(entry.second);
    }
}

void NodeVisitor::visitBreak(const BreakStatementPtr& node)
{
}

void NodeVisitor::visitReturn(const ReturnStatementPtr& node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitContinue(const ContinueStatementPtr& node)
{
}

void NodeVisitor::visitFallthrough(const FallthroughStatementPtr& node)
{
}

void NodeVisitor::visitIf(const IfStatementPtr& node)
{
    ACCEPT(node->getCondition());
    ACCEPT(node->getThen());
    ACCEPT(node->getElse());
}

void NodeVisitor::visitSwitchCase(const SwitchCasePtr& node)
{
    ACCEPT(node->getControlExpression());
    for(CaseStatementPtr c : *node)
    {
        ACCEPT(c);
    }
    ACCEPT(node->getDefaultCase());
}

void NodeVisitor::visitCase(const CaseStatementPtr& node)
{
    for(auto condition : node->getConditions())
    {
        ACCEPT(condition.condition);
        ACCEPT(condition.guard);
    }
    ACCEPT(node->getCodeBlock());
}

void NodeVisitor::visitCodeBlock(const CodeBlockPtr& node)
{
    for(auto st: *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitParameter(const ParameterPtr& node)
{

}

void NodeVisitor::visitParameters(const ParametersPtr& node)
{
    for(ParameterPtr p : *node)
    {
        ACCEPT(p);
    }
}

void NodeVisitor::visitProgram(const ProgramPtr& node)
{
    for(StatementPtr st : *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitValueBindingPattern(const ValueBindingPatternPtr& node)
{
}


void NodeVisitor::visitValueBindings(const ValueBindingsPtr &node)
{
    for(ValueBindingPtr var : *node)
    {
        ACCEPT(var);
    }
}

void NodeVisitor::visitComputedProperty(const ComputedPropertyPtr& node)
{
    ACCEPT(node->getInitializer());
    ACCEPT(node->getGetter());
    ACCEPT(node->getSetter());
    ACCEPT(node->getWillSet());
    ACCEPT(node->getDidSet());
}

void NodeVisitor::visitValueBinding(const ValueBindingPtr &node)
{
    ACCEPT(node->getInitializer());
}

void NodeVisitor::visitAssignment(const AssignmentPtr& node)
{
    ACCEPT(node->getLHS());
    ACCEPT(node->getRHS());
}

void NodeVisitor::visitClass(const ClassDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitStruct(const StructDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitEnum(const EnumDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitProtocol(const ProtocolDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitExtension(const ExtensionDefPtr& node)
{
    for(DeclarationPtr dec : *node)
    {
        dec->accept(this);
    }
}

void NodeVisitor::visitFunction(const FunctionDefPtr& node)
{
    for(ParametersPtr params : node->getParametersList())
    {
        ACCEPT(params);
    }
    ACCEPT(node->getBody());
}

void NodeVisitor::visitWhileLoop(const WhileLoopPtr& node)
{
    ACCEPT(node->getCondition());
    ACCEPT(node->getCodeBlock());
}

void NodeVisitor::visitConditionalOperator(const ConditionalOperatorPtr& node)
{
    ACCEPT(node->getCondition());
    ACCEPT(node->getTrueExpression());
    ACCEPT(node->getFalseExpression());
}

void NodeVisitor::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    ACCEPT(node->getLHS());
    ACCEPT(node->getRHS());
}

void NodeVisitor::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    ACCEPT(node->getOperand());
}

void NodeVisitor::visitTuple(const TuplePtr& node)
{
    for(const PatternPtr & p : *node)
    {
        ACCEPT(p);
    }
}

void NodeVisitor::visitIdentifier(const IdentifierPtr& node)
{

}
void NodeVisitor::visitTypedPattern(const TypedPatternPtr& node)
{
    ACCEPT(node->getPattern());
}
void NodeVisitor::visitCompileConstant(const CompileConstantPtr& node)
{
}

void NodeVisitor::visitSubscriptAccess(const SubscriptAccessPtr& node)
{
    ACCEPT(node->getSelf());
    ACCEPT(node->getIndex());
}

void NodeVisitor::visitMemberAccess(const MemberAccessPtr& node)
{
    ACCEPT(node->getSelf());
}

void NodeVisitor::visitFunctionCall(const FunctionCallPtr& node)
{
    ACCEPT(node->getFunction());
    ACCEPT(node->getArguments());
    ACCEPT(node->getTrailingClosure())
}

void NodeVisitor::visitClosure(const ClosurePtr& node)
{
    ACCEPT(node->getCapture());
    ACCEPT(node->getParameters());
    for(const StatementPtr& st : *node)
    {
        ACCEPT(st);
    }
}

void NodeVisitor::visitSelf(const SelfExpressionPtr& node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitInitializerReference(const InitializerReferencePtr& node)
{
    ACCEPT(node->getExpression());
}


void NodeVisitor::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
}

void NodeVisitor::visitDynamicType(const DynamicTypePtr& node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitForcedValue(const ForcedValuePtr& node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitOptionalChaining(const OptionalChainingPtr& node)
{
    ACCEPT(node->getExpression());
}

void NodeVisitor::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    for(auto term : *node)
    {
        ACCEPT(term.second);
    }
}

void NodeVisitor::visitStringInterpolation(const StringInterpolationPtr &node)
{
    for(auto expr : * node)
    {
        ACCEPT(expr);
    }
}
void NodeVisitor::visitString(const StringLiteralPtr& node)
{

}
void NodeVisitor::visitInteger(const IntegerLiteralPtr& node)
{

}
void NodeVisitor::visitFloat(const FloatLiteralPtr& node)
{

}


void NodeVisitor::visitArrayType(const ArrayTypePtr& node)
{

}
void NodeVisitor::visitFunctionType(const FunctionTypePtr& node)
{

}
void NodeVisitor::visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptional& node)
{

}
void NodeVisitor::visitOptionalType(const OptionalTypePtr& node)
{

}
void NodeVisitor::visitProtocolComposition(const ProtocolCompositionPtr& node)
{

}
void NodeVisitor::visitTupleType(const TupleTypePtr& node)
{

}
void NodeVisitor::visitTypeIdentifier(const TypeIdentifierPtr& node)
{

}
