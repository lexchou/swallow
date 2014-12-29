/* ast-decl.h --
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
#ifndef AST_DECL_H
#define AST_DECL_H
#include "swallow_conf.h"
#include <memory>
#include <vector>

SWALLOW_NS_BEGIN

typedef std::shared_ptr<class Operator> OperatorPtr;

typedef std::shared_ptr<class Declaration> DeclarationPtr;
typedef std::shared_ptr<class Statement> StatementPtr;

typedef std::shared_ptr<class IntegerLiteral> IntegerLiteralPtr;
typedef std::shared_ptr<class FloatLiteral> FloatLiteralPtr;
typedef std::shared_ptr<class StringLiteral> StringLiteralPtr;
typedef std::shared_ptr<class NilLiteral> NilLiteralPtr;
typedef std::shared_ptr<class BooleanLiteral> BooleanLiteralPtr;

typedef std::shared_ptr<class CommentNode> CommentNodePtr;
typedef std::shared_ptr<class UnaryOperator> UnaryOperatorPtr;
typedef std::shared_ptr<class BinaryOperator> BinaryOperatorPtr;
typedef std::shared_ptr<class Identifier> IdentifierPtr;
typedef std::shared_ptr<class TypedPattern> TypedPatternPtr;
typedef std::shared_ptr<class GenericArgumentDef> GenericArgumentDefPtr;
typedef std::shared_ptr<class Node> NodePtr;
typedef std::shared_ptr<class Expression> ExpressionPtr;
typedef std::shared_ptr<class InOutParameter> InOutParameterPtr;
typedef std::shared_ptr<class ArrayLiteral> ArrayLiteralPtr;
typedef std::shared_ptr<class DictionaryLiteral> DictionaryLiteralPtr;
typedef std::shared_ptr<class CompileConstant> CompileConstantPtr;
typedef std::shared_ptr<class MemberAccess> MemberAccessPtr;
typedef std::shared_ptr<class SubscriptAccess> SubscriptAccessPtr;
typedef std::shared_ptr<class TypeNode> TypeNodePtr;
typedef std::shared_ptr<class TypeCheck> TypeCheckPtr;
typedef std::shared_ptr<class TypeCast> TypeCastPtr;
typedef std::shared_ptr<class Assignment> AssignmentPtr;
typedef std::shared_ptr<class ConditionalOperator> ConditionalOperatorPtr;
typedef std::shared_ptr<class ParenthesizedExpression> ParenthesizedExpressionPtr;
typedef std::shared_ptr<class InitializerReference> InitializerReferencePtr;
typedef std::shared_ptr<class SelfExpression> SelfExpressionPtr;
typedef std::shared_ptr<class DynamicType> DynamicTypePtr;
typedef std::shared_ptr<class ForcedValue> ForcedValuePtr;
typedef std::shared_ptr<class OptionalChaining> OptionalChainingPtr;
typedef std::shared_ptr<class FunctionCall> FunctionCallPtr;
typedef std::shared_ptr<class ForLoop> ForLoopPtr;
typedef std::shared_ptr<class ForInLoop> ForInLoopPtr;
typedef std::shared_ptr<class WhileLoop> WhileLoopPtr;
typedef std::shared_ptr<class DoLoop> DoLoopPtr;
typedef std::shared_ptr<class IfStatement> IfStatementPtr;
typedef std::shared_ptr<class SwitchCase> SwitchCasePtr;
typedef std::shared_ptr<class CaseStatement> CaseStatementPtr;
typedef std::shared_ptr<class ReturnStatement> ReturnStatementPtr;
typedef std::shared_ptr<class BreakStatement> BreakStatementPtr;
typedef std::shared_ptr<class ContinueStatement> ContinueStatementPtr;
typedef std::shared_ptr<class FallthroughStatement> FallthroughStatementPtr;
typedef std::shared_ptr<class LabeledStatement> LabeledStatementPtr;
typedef std::shared_ptr<class CodeBlock> CodeBlockPtr;
typedef std::shared_ptr<class Tuple> TuplePtr;
typedef std::shared_ptr<class ValueBindingPattern> ValueBindingPatternPtr;
typedef std::shared_ptr<class FunctionType> FunctionTypePtr;
typedef std::shared_ptr<class ArrayType> ArrayTypePtr;
typedef std::shared_ptr<class DictionaryType> DictionaryTypePtr;
typedef std::shared_ptr<class OptionalType> OptionalTypePtr;
typedef std::shared_ptr<class ImplicitlyUnwrappedOptional> ImplicitlyUnwrappedOptionalPtr;
typedef std::shared_ptr<class TypeIdentifier> TypeIdentifierPtr;
typedef std::shared_ptr<class ProtocolComposition> ProtocolCompositionPtr;
typedef std::shared_ptr<class TupleType> TupleTypePtr;
typedef std::shared_ptr<class Attribute> AttributePtr;
typedef std::shared_ptr<class Closure> ClosurePtr;
typedef std::shared_ptr<class Program> ProgramPtr;



typedef std::shared_ptr<class Import> ImportPtr;
typedef std::shared_ptr<class ValueBinding> ValueBindingPtr;
typedef std::shared_ptr<class ValueBindings> ValueBindingsPtr;
typedef std::shared_ptr<class ComputedProperty> ComputedPropertyPtr;
typedef std::shared_ptr<class TypeAlias> TypeAliasPtr;
typedef std::shared_ptr<class FunctionDef> FunctionDefPtr;
typedef std::shared_ptr<class EnumDef> EnumDefPtr;
typedef std::shared_ptr<class StructDef> StructDefPtr;
typedef std::shared_ptr<class ClassDef> ClassDefPtr;
typedef std::shared_ptr<class ProtocolDef> ProtocolDefPtr;
typedef std::shared_ptr<class InitializerDef> InitializerDefPtr;
typedef std::shared_ptr<class DeinitializerDef> DeinitializerDefPtr;
typedef std::shared_ptr<class ExtensionDef> ExtensionDefPtr;
typedef std::shared_ptr<class SubscriptDef> SubscriptDefPtr;
typedef std::shared_ptr<class OperatorDef> OperatorDefPtr;
typedef std::shared_ptr<class Parameters> ParametersPtr;
typedef std::shared_ptr<class Parameter> ParameterPtr;
typedef std::shared_ptr<class GenericConstraintDef> GenericConstraintDefPtr;
typedef std::shared_ptr<class GenericParametersDef> GenericParametersDefPtr;
typedef std::shared_ptr<class Pattern> PatternPtr;
typedef std::shared_ptr<class EnumCasePattern> EnumCasePatternPtr;
typedef std::shared_ptr<class StringInterpolation> StringInterpolationPtr;
typedef std::vector<AttributePtr> Attributes;


SWALLOW_NS_END

#endif//
