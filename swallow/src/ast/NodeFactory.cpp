/* NodeFactory.cpp --
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
#include "ast/NodeFactory.h"
#include "ast/ast.h"
USE_SWALLOW_NS;


NodeFactory::NodeFactory()
{
}

void NodeFactory::bindNode(const SourceInfo&s, Node* n)
{
    *n->getSourceInfo() = s;
    n->nodeFactory = this;
}


ProgramPtr NodeFactory::createProgram()
{
    return create<Program>(SourceInfo());
}
CommentNodePtr NodeFactory::createComment(const SourceInfo&state)
{
    return create<CommentNode>(state);
}
IntegerLiteralPtr NodeFactory::createInteger(const SourceInfo&state)
{
    return create<IntegerLiteral>(state);
}
FloatLiteralPtr NodeFactory::createFloat(const SourceInfo&state)
{
    return create<FloatLiteral>(state);
}
NilLiteralPtr NodeFactory::createNilLiteral(const SourceInfo& state)
{
    return create<NilLiteral>(state);
}
BooleanLiteralPtr NodeFactory::createBooleanLiteral(const SourceInfo& state)
{
    return create<BooleanLiteral>(state);
}
DefaultValuePtr NodeFactory::createDefaultValue(const SourceInfo& state)
{
    return create<DefaultValue>(state);
}
StringInterpolationPtr NodeFactory::createStringInterpolation(const SourceInfo &state)
{
    return create<StringInterpolation>(state);
}
StringLiteralPtr NodeFactory::createString(const SourceInfo&state)
{
    return create<StringLiteral>(state);
}
UnaryOperatorPtr NodeFactory::createUnary(const SourceInfo&state)
{
    return create<UnaryOperator>(state);
}
TypedPatternPtr NodeFactory::createTypedPattern(const SourceInfo &state)
{
    return create<TypedPattern>(state);
}
IdentifierPtr NodeFactory::createIdentifier(const SourceInfo&state)
{
    return create<Identifier>(state);
}
GenericArgumentDefPtr NodeFactory::createGenericArgumentDef(const SourceInfo& state)
{
    return create<GenericArgumentDef>(state);
}
InOutParameterNode NodeFactory::createInOutParameter(const SourceInfo&state)
{
    return create<InOutParameter>(state);
}
BinaryOperatorPtr NodeFactory::createBinary(const SourceInfo&state)
{
    return create<BinaryOperator>(state);
}

ArrayLiteralPtr NodeFactory::createArrayLiteral(const SourceInfo& state)
{
    return create<ArrayLiteral>(state);
}
DictionaryLiteralPtr NodeFactory::createDictionaryLiteral(const SourceInfo& state)
{
    return create<DictionaryLiteral>(state);
}
CompileConstantPtr NodeFactory::createCompilecConstant(const SourceInfo&state)
{
    return create<CompileConstant>(state);
}

MemberAccessPtr NodeFactory::createMemberAccess(const SourceInfo&state)
{
    return create<MemberAccess>(state);
}
SubscriptAccessPtr NodeFactory::createSubscriptAccess(const SourceInfo&state)
{
    return create<SubscriptAccess>(state);
}

TypeCheckPtr NodeFactory::createTypeCheck(const SourceInfo&state)
{
    return create<TypeCheck>(state);
}
TypeCastPtr NodeFactory::createTypeCast(const SourceInfo&state)
{
    return create<TypeCast>(state);
}
AssignmentPtr NodeFactory::createAssignment(const SourceInfo&state)
{
    return create<Assignment>(state);
}
ConditionalOperatorPtr NodeFactory::createConditionalOperator(const SourceInfo&state)
{
    return create<ConditionalOperator>(state);
}
ParenthesizedExpressionPtr NodeFactory::createParenthesizedExpression(const SourceInfo& state)
{
    return create<ParenthesizedExpression>(state);
}

InitializerReferencePtr NodeFactory::createInitializerReference(const SourceInfo&state)
{
    return create<InitializerReference>(state);
}
SelfExpressionPtr NodeFactory::createSelfExpression(const SourceInfo&state)
{
    return create<SelfExpression>(state);
}
DynamicTypePtr NodeFactory::createDynamicType(const SourceInfo&state)
{
    return create<DynamicType>(state);
}
ForcedValuePtr NodeFactory::createForcedValue(const SourceInfo&state)
{
    return create<ForcedValue>(state);
}
OptionalChainingPtr NodeFactory::createOptionalChaining(const SourceInfo&state)
{
    return create<OptionalChaining>(state);
}
FunctionCallPtr NodeFactory::createFunctionCall(const SourceInfo& state)
{
    return create<FunctionCall>(state);
}


ForLoopPtr NodeFactory::createForLoop(const SourceInfo& state)
{
    return create<ForLoop>(state);
}
ForInLoopPtr NodeFactory::createForInLoop(const SourceInfo& state)
{
    return create<ForInLoop>(state);
}
WhileLoopPtr NodeFactory::createWhileLoop(const SourceInfo& state)
{
    return create<WhileLoop>(state);
}
IfStatementPtr NodeFactory::createIf(const SourceInfo& state)
{
    return create<IfStatement>(state);
}
DoLoopPtr NodeFactory::createDoLoop(const SourceInfo& state)
{
    return create<DoLoop>(state);
}
SwitchCasePtr NodeFactory::createSwitch(const SourceInfo& state)
{
    return create<SwitchCase>(state);
}
CaseStatementPtr NodeFactory::createCase(const SourceInfo& state)
{
    return create<CaseStatement>(state);
}
BreakStatementPtr NodeFactory::createBreak(const SourceInfo& state)
{
    return create<BreakStatement>(state);
}
ContinueStatementPtr NodeFactory::createContinue(const SourceInfo& state)
{
    return create<ContinueStatement>(state);
}
FallthroughStatementPtr NodeFactory::createFallthrough(const SourceInfo& state)
{
    return create<FallthroughStatement>(state);
}
ReturnStatementPtr NodeFactory::createReturn(const SourceInfo& state)
{
    return create<ReturnStatement>(state);
}
LabeledStatementPtr NodeFactory::createLabel(const SourceInfo& state)
{
    return create<LabeledStatement>(state);
}
CodeBlockPtr NodeFactory::createCodeBlock(const SourceInfo& state)
{
    return create<CodeBlock>(state);
}
ValueBindingPatternPtr NodeFactory::createValueBindingPattern(const SourceInfo& state)
{
    return create<ValueBindingPattern>(state);
}
TuplePtr NodeFactory::createTuple(const SourceInfo& state)
{
    return create<Tuple>(state);
}
ClosurePtr NodeFactory::createClosure(const SourceInfo& state)
{
    return create<Closure>(state);
}
EnumCasePatternPtr NodeFactory::createEnumCasePattern(const SourceInfo&state)
{
    return create<EnumCasePattern>(state);
}


FunctionTypePtr NodeFactory::createFunctionType(const SourceInfo&state)
{
    return create<FunctionType>(state);
}
ArrayTypePtr NodeFactory::createArrayType(const SourceInfo&state)
{
    return create<ArrayType>(state);
}
DictionaryTypePtr NodeFactory::createDictionaryType(const SourceInfo& state)
{
    return create<DictionaryType>(state);
}
OptionalTypePtr NodeFactory::createOptionalType(const SourceInfo&state)
{
    return create<OptionalType>(state);
}
ImplicitlyUnwrappedOptionalPtr NodeFactory::createImplicitlyUnwrappedOptional(const SourceInfo&state)
{
    return create<ImplicitlyUnwrappedOptional>(state);
}
TypeIdentifierPtr NodeFactory::createTypeIdentifier(const SourceInfo&state)
{
    return create<TypeIdentifier>(state);
}
ProtocolCompositionPtr NodeFactory::createProtocolComposition(const SourceInfo& state)
{
    return create<ProtocolComposition>(state);
}
TupleTypePtr NodeFactory::createTupleType(const SourceInfo& state)
{
    return create<TupleType>(state);
}
AttributePtr NodeFactory::createAttribute(const SourceInfo& state)
{
    return create<Attribute>(state);
}



ImportPtr NodeFactory::createImport(const SourceInfo&state)
{
    return create<Import>(state);
}
ComputedPropertyPtr NodeFactory::createComputedProperty(const SourceInfo &state)
{
    return create<ComputedProperty>(state);
}

ValueBindingPtr NodeFactory::createValueBinding(const SourceInfo &state)
{
    return create<ValueBinding>(state);
}

ValueBindingsPtr NodeFactory::createValueBindings(const SourceInfo& state)
{
    return  create<ValueBindings>(state);
}
TypeAliasPtr NodeFactory::createTypealias(const SourceInfo&state)
{
    return create<TypeAlias>(state);
}
FunctionDefPtr NodeFactory::createFunction(const SourceInfo&state)
{
    return create<FunctionDef>(state);
}
ParametersNodePtr NodeFactory::createParameters(const SourceInfo& state)
{
    return create<ParametersNode>(state);
}
ParameterNodePtr NodeFactory::createParameter(const SourceInfo& state)
{
    return create<ParameterNode>(state);
}
EnumDefPtr NodeFactory::createEnum(const SourceInfo&state)
{
    return create<EnumDef>(state);
}
StructDefPtr NodeFactory::createStruct(const SourceInfo&state)
{
    return create<StructDef>(state);
}
ClassDefPtr NodeFactory::createClass(const SourceInfo&state)
{
    return create<ClassDef>(state);
}
ProtocolDefPtr NodeFactory::createProtocol(const SourceInfo&state)
{
    return create<ProtocolDef>(state);
}
InitializerDefPtr NodeFactory::createInitializer(const SourceInfo&state)
{
    return create<InitializerDef>(state);
}
DeinitializerDefPtr NodeFactory::createDeinitializer(const SourceInfo&state)
{
    return create<DeinitializerDef>(state);
}
ExtensionDefPtr NodeFactory::createExtension(const SourceInfo&state)
{
    return create<ExtensionDef>(state);
}
SubscriptDefPtr NodeFactory::createSubscript(const SourceInfo&state)
{
    return create<SubscriptDef>(state);
}
OperatorDefPtr NodeFactory::createOperator(const SourceInfo&state)
{
    return create<OperatorDef>(state);
}
GenericConstraintDefPtr NodeFactory::createGenericConstraintDef(const SourceInfo& state)
{
    return create<GenericConstraintDef>(state);
}
GenericParametersDefPtr NodeFactory::createGenericParametersDef(const SourceInfo& state)
{
    return create<GenericParametersDef>(state);
}



