/* NodeVisitorDecorator.cpp --
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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
#include "NodeVisitorDecorator.h"
USE_SWIFT_NS


#define VISIT(fn) visitor->fn(node);
NodeVisitorDecorator::NodeVisitorDecorator(NodeVisitor* visitor)
:visitor(visitor)
{

}

void NodeVisitorDecorator::visitDeinit(const DeinitializerDefPtr& node)
{
    visitor->visitDeinit(node);
}

void NodeVisitorDecorator::visitInit(const InitializerDefPtr& node)
{
    visitor->visitInit(node);
}

void NodeVisitorDecorator::visitImport(const ImportPtr& node)
{
    visitor->visitImport(node);
}

void NodeVisitorDecorator::visitSubscript(const SubscriptDefPtr& node)
{
    visitor->visitSubscript(node);
}

void NodeVisitorDecorator::visitTypeAlias(const TypeAliasPtr& node)
{
    visitor->visitTypeAlias(node);
}

void NodeVisitorDecorator::visitForIn(const ForInLoopPtr& node)
{
    visitor->visitForIn(node);
}

void NodeVisitorDecorator::visitForLoop(const ForLoopPtr& node)
{
    visitor->visitForLoop(node);
}

void NodeVisitorDecorator::visitDoLoop(const DoLoopPtr& node)
{
    visitor->visitDoLoop(node);
}

void NodeVisitorDecorator::visitLabeledStatement(const LabeledStatementPtr& node)
{
    visitor->visitLabeledStatement(node);
}

void NodeVisitorDecorator::visitOperator(const OperatorDefPtr& node)
{
    visitor->visitOperator(node);
}

void NodeVisitorDecorator::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    visitor->visitArrayLiteral(node);
}

void NodeVisitorDecorator::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    visitor->visitDictionaryLiteral(node);
}

void NodeVisitorDecorator::visitBreak(const BreakStatementPtr& node)
{
    visitor->visitBreak(node);
}

void NodeVisitorDecorator::visitReturn(const ReturnStatementPtr& node)
{
    visitor->visitReturn(node);
}

void NodeVisitorDecorator::visitContinue(const ContinueStatementPtr& node)
{
    visitor->visitContinue(node);
}

void NodeVisitorDecorator::visitFallthrough(const FallthroughStatementPtr& node)
{
    visitor->visitFallthrough(node);
}

void NodeVisitorDecorator::visitIf(const IfStatementPtr& node)
{
    visitor->visitIf(node);
}

void NodeVisitorDecorator::visitSwitchCase(const SwitchCasePtr& node)
{
    visitor->visitSwitchCase(node);
}

void NodeVisitorDecorator::visitCase(const CaseStatementPtr& node)
{
    visitor->visitCase(node);
}

void NodeVisitorDecorator::visitCodeBlock(const CodeBlockPtr& node)
{
    visitor->visitCodeBlock(node);
}

void NodeVisitorDecorator::visitParameter(const ParameterPtr& node)
{
    VISIT(visitParameter);
}

void NodeVisitorDecorator::visitParameters(const ParametersPtr& node)
{
    VISIT(visitParameters);
}

void NodeVisitorDecorator::visitProgram(const ProgramPtr& node)
{
    VISIT(visitProgram);
}

void NodeVisitorDecorator::visitValueBindingPattern(const ValueBindingPatternPtr& node)
{
    VISIT(visitValueBindingPattern);
}

void NodeVisitorDecorator::visitValueBindings(const ValueBindingsPtr &node)
{
    VISIT(visitValueBindings);
}

void NodeVisitorDecorator::visitValueBinding(const ValueBindingPtr &node)
{
    VISIT(visitValueBinding);
}

void NodeVisitorDecorator::visitComputedProperty(const ComputedPropertyPtr &node)
{
    VISIT(visitComputedProperty);
}

void NodeVisitorDecorator::visitAssignment(const AssignmentPtr& node)
{
    VISIT(visitAssignment);
}

void NodeVisitorDecorator::visitClass(const ClassDefPtr& node)
{
    VISIT(visitClass);
}

void NodeVisitorDecorator::visitStruct(const StructDefPtr& node)
{
    VISIT(visitStruct);
}

void NodeVisitorDecorator::visitEnum(const EnumDefPtr& node)
{
    VISIT(visitEnum);
}

void NodeVisitorDecorator::visitProtocol(const ProtocolDefPtr& node)
{
    VISIT(visitProtocol);
}

void NodeVisitorDecorator::visitExtension(const ExtensionDefPtr& node)
{
    VISIT(visitExtension);
}

void NodeVisitorDecorator::visitFunction(const FunctionDefPtr& node)
{
    VISIT(visitFunction);
}

void NodeVisitorDecorator::visitWhileLoop(const WhileLoopPtr& node)
{
    VISIT(visitWhileLoop);
}

void NodeVisitorDecorator::visitConditionalOperator(const ConditionalOperatorPtr& node)
{
    VISIT(visitConditionalOperator);
}

void NodeVisitorDecorator::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    VISIT(visitBinaryOperator);
}

void NodeVisitorDecorator::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    VISIT(visitUnaryOperator);
}

void NodeVisitorDecorator::visitTuple(const TuplePtr& node)
{
    VISIT(visitTuple);
}

void NodeVisitorDecorator::visitIdentifier(const IdentifierPtr& node)
{
    VISIT(visitIdentifier);
}

void NodeVisitorDecorator::visitCompileConstant(const CompileConstantPtr& node)
{
    VISIT(visitCompileConstant);
}

void NodeVisitorDecorator::visitSubscriptAccess(const SubscriptAccessPtr& node)
{
    VISIT(visitSubscriptAccess);
}

void NodeVisitorDecorator::visitMemberAccess(const MemberAccessPtr& node)
{
    VISIT(visitMemberAccess);
}

void NodeVisitorDecorator::visitFunctionCall(const FunctionCallPtr& node)
{
    VISIT(visitFunctionCall);
}

void NodeVisitorDecorator::visitClosure(const ClosurePtr& node)
{
    VISIT(visitClosure);
}

void NodeVisitorDecorator::visitSelf(const SelfExpressionPtr& node)
{
    VISIT(visitSelf);
}

void NodeVisitorDecorator::visitInitializerReference(const InitializerReferencePtr& node)
{
    VISIT(visitInitializerReference);
}


void NodeVisitorDecorator::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
    VISIT(visitEnumCasePattern);
}

void NodeVisitorDecorator::visitDynamicType(const DynamicTypePtr& node)
{
    VISIT(visitDynamicType);
}

void NodeVisitorDecorator::visitForcedValue(const ForcedValuePtr& node)
{
    VISIT(visitForcedValue);
}

void NodeVisitorDecorator::visitOptionalChaining(const OptionalChainingPtr& node)
{
    VISIT(visitOptionalChaining);
}

void NodeVisitorDecorator::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    VISIT(visitParenthesizedExpression);
}

void NodeVisitorDecorator::visitArrayType(const ArrayTypePtr& node)
{
    VISIT(visitArrayType);
}
void NodeVisitorDecorator::visitFunctionType(const FunctionTypePtr& node)
{
    VISIT(visitFunctionType);
}
void NodeVisitorDecorator::visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptional& node)
{
    VISIT(visitImplicitlyUnwrappedOptional);
}
void NodeVisitorDecorator::visitOptionalType(const OptionalTypePtr& node)
{
    VISIT(visitOptionalType);
}
void NodeVisitorDecorator::visitProtocolComposition(const ProtocolCompositionPtr& node)
{
    VISIT(visitProtocolComposition);
}
void NodeVisitorDecorator::visitTupleType(const TupleTypePtr& node)
{
    VISIT(visitTupleType);
}
void NodeVisitorDecorator::visitTypeIdentifier(const TypeIdentifierPtr& node)
{
    VISIT(visitTypeIdentifier);
}
