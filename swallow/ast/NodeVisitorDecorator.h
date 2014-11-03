/* NodeVisitorDecorator.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
#ifndef NODE_VISITOR_DECORATOR_H
#define NODE_VISITOR_DECORATOR_H
#include "NodeVisitor.h"

SWALLOW_NS_BEGIN

class Statement;
class NodeVisitorDecorator : public NodeVisitor
{
public:
    NodeVisitorDecorator(NodeVisitor* visitor);
public:
    virtual void visitValueBindings(const ValueBindingsPtr& node) override;
    virtual void visitValueBinding(const ValueBindingPtr& node) override;
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitAssignment(const AssignmentPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitFunction(const FunctionDefPtr& node) override;
    virtual void visitDeinit(const DeinitializerDefPtr& node) override;
    virtual void visitInit(const InitializerDefPtr& node) override;
    virtual void visitImport(const ImportPtr& node) override;
    virtual void visitSubscript(const SubscriptDefPtr& node) override;
    virtual void visitTypeAlias(const TypeAliasPtr& node) override;
public://statement
    virtual void visitWhileLoop(const WhileLoopPtr& node) override;
    virtual void visitForIn(const ForInLoopPtr& node) override;
    virtual void visitForLoop(const ForLoopPtr& node) override;
    virtual void visitDoLoop(const DoLoopPtr& node) override;
    virtual void visitLabeledStatement(const LabeledStatementPtr& node) override;
    virtual void visitOperator(const OperatorDefPtr& node) override;
    virtual void visitArrayLiteral(const ArrayLiteralPtr& node) override;
    virtual void visitDictionaryLiteral(const DictionaryLiteralPtr& node) override;
    virtual void visitBreak(const BreakStatementPtr& node) override;
    virtual void visitReturn(const ReturnStatementPtr& node) override;
    virtual void visitContinue(const ContinueStatementPtr& node) override;
    virtual void visitFallthrough(const FallthroughStatementPtr& node) override;
    virtual void visitIf(const IfStatementPtr& node) override;
    virtual void visitSwitchCase(const SwitchCasePtr& node) override;
    virtual void visitCase(const CaseStatementPtr& node) override;
    virtual void visitCodeBlock(const CodeBlockPtr& node) override;
    virtual void visitParameter(const ParameterPtr& node) override;
    virtual void visitParameters(const ParametersPtr& node) override;
    virtual void visitProgram(const ProgramPtr& node) override;
    virtual void visitValueBindingPattern(const ValueBindingPatternPtr& node) override;
public:
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node) override;
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node) override;
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node) override;
    virtual void visitTuple(const TuplePtr& node) override;
    virtual void visitIdentifier(const IdentifierPtr& node) override;
    virtual void visitCompileConstant(const CompileConstantPtr& node) override;
    virtual void visitSubscriptAccess(const SubscriptAccessPtr& node) override;
    virtual void visitMemberAccess(const MemberAccessPtr& node) override;
    virtual void visitFunctionCall(const FunctionCallPtr& node) override;
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitSelf(const SelfExpressionPtr& node) override;
    virtual void visitInitializerReference(const InitializerReferencePtr& node) override;

    virtual void visitEnumCasePattern(const EnumCasePatternPtr& node) override;
    virtual void visitDynamicType(const DynamicTypePtr& node) override;
    virtual void visitForcedValue(const ForcedValuePtr& node) override;
    virtual void visitOptionalChaining(const OptionalChainingPtr& node) override;
    virtual void visitParenthesizedExpression(const ParenthesizedExpressionPtr& node) override;
public:
    virtual void visitArrayType(const ArrayTypePtr& node) override;
    virtual void visitFunctionType(const FunctionTypePtr& node) override;
    virtual void visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptional& node) override;
    virtual void visitOptionalType(const OptionalTypePtr& node) override;
    virtual void visitProtocolComposition(const ProtocolCompositionPtr& node) override;
    virtual void visitTupleType(const TupleTypePtr& node) override;
    virtual void visitTypeIdentifier(const TypeIdentifierPtr& node) override;
private:
    NodeVisitor* visitor;
};


SWALLOW_NS_END
#endif//NODE_VISITOR_DECORATOR_H
