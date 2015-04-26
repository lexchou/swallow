/* NodeVisitor.h --
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
#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H
#include "swallow_conf.h"
#include "ast-decl.h"

SWALLOW_NS_BEGIN

class Statement;
class Node;
/*!
 * The NodeVisitor and its derived classes are not thread-safe.
 */
class SWALLOW_EXPORT NodeVisitor
{
    friend class Node;
public:
    virtual ~NodeVisitor(){}
public:
    virtual void visitValueBindings(const ValueBindingsPtr& node);
    virtual void visitComputedProperty(const ComputedPropertyPtr& node);
    virtual void visitValueBinding(const ValueBindingPtr& node);
    virtual void visitAssignment(const AssignmentPtr& node);
    virtual void visitClass(const ClassDefPtr& node);
    virtual void visitStruct(const StructDefPtr& node);
    virtual void visitEnum(const EnumDefPtr& node);
    virtual void visitProtocol(const ProtocolDefPtr& node);
    virtual void visitExtension(const ExtensionDefPtr& node);
    virtual void visitFunction(const FunctionDefPtr& node);
    virtual void visitDeinit(const DeinitializerDefPtr& node);
    virtual void visitInit(const InitializerDefPtr& node);
    virtual void visitImport(const ImportPtr& node);
    virtual void visitSubscript(const SubscriptDefPtr& node);
    virtual void visitTypeAlias(const TypeAliasPtr& node);
public://statement
    virtual void visitWhileLoop(const WhileLoopPtr& node);
    virtual void visitForIn(const ForInLoopPtr& node);
    virtual void visitForLoop(const ForLoopPtr& node);
    virtual void visitDoLoop(const DoLoopPtr& node);
    virtual void visitLabeledStatement(const LabeledStatementPtr& node);
    virtual void visitOperator(const OperatorDefPtr& node);
    virtual void visitArrayLiteral(const ArrayLiteralPtr& node);
    virtual void visitDictionaryLiteral(const DictionaryLiteralPtr& node);
    virtual void visitBreak(const BreakStatementPtr& node);
    virtual void visitReturn(const ReturnStatementPtr& node);
    virtual void visitContinue(const ContinueStatementPtr& node);
    virtual void visitFallthrough(const FallthroughStatementPtr& node);
    virtual void visitIf(const IfStatementPtr& node);
    virtual void visitSwitchCase(const SwitchCasePtr& node);
    virtual void visitCase(const CaseStatementPtr& node);
    virtual void visitCodeBlock(const CodeBlockPtr& node);
    virtual void visitParameter(const ParameterNodePtr& node);
    virtual void visitParameters(const ParametersNodePtr& node);
    virtual void visitProgram(const ProgramPtr& node);
    virtual void visitValueBindingPattern(const ValueBindingPatternPtr& node);
public:
    virtual void visitConditionalOperator(const ConditionalOperatorPtr& node);
    virtual void visitBinaryOperator(const BinaryOperatorPtr& node);
    virtual void visitUnaryOperator(const UnaryOperatorPtr& node);
    virtual void visitTuple(const TuplePtr& node);
    virtual void visitIdentifier(const IdentifierPtr& node);
    virtual void visitCompileConstant(const CompileConstantPtr& node);
    virtual void visitSubscriptAccess(const SubscriptAccessPtr& node);
    virtual void visitMemberAccess(const MemberAccessPtr& node);
    virtual void visitFunctionCall(const FunctionCallPtr& node);
    virtual void visitClosure(const ClosurePtr& node);
    virtual void visitSelf(const SelfExpressionPtr& node);
    virtual void visitInitializerReference(const InitializerReferencePtr& node);
    virtual void visitTypedPattern(const TypedPatternPtr& node);
    virtual void visitEnumCasePattern(const EnumCasePatternPtr& node);
    virtual void visitDynamicType(const DynamicTypePtr& node);
    virtual void visitForcedValue(const ForcedValuePtr& node);
    virtual void visitOptionalChaining(const OptionalChainingPtr& node);
    virtual void visitParenthesizedExpression(const ParenthesizedExpressionPtr& node);

    virtual void visitString(const StringLiteralPtr& node);
    virtual void visitStringInterpolation(const StringInterpolationPtr& node);
    virtual void visitInteger(const IntegerLiteralPtr& node);
    virtual void visitFloat(const FloatLiteralPtr& node);
    virtual void visitNilLiteral(const NilLiteralPtr& node);
    virtual void visitBooleanLiteral(const BooleanLiteralPtr& node);
    virtual void visitDefaultValue(const DefaultValuePtr& node);
public:
    virtual void visitArrayType(const ArrayTypePtr& node);
    virtual void visitFunctionType(const FunctionTypePtr& node);
    virtual void visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptionalPtr& node);
    virtual void visitOptionalType(const OptionalTypePtr& node);
    virtual void visitProtocolComposition(const ProtocolCompositionPtr& node);
    virtual void visitTupleType(const TupleTypePtr& node);
    virtual void visitTypeIdentifier(const TypeIdentifierPtr& node);
protected:
    NodePtr currentNode;
};


SWALLOW_NS_END
#endif//NODE_VISITOR_H
