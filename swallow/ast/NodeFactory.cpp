/* NodeFactory.cpp --
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
#include "NodeFactory.h"
#include "ast.h"
USE_SWIFT_NS;


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
    return _(SourceInfo(), new Program());
}
CommentNodePtr NodeFactory::createComment(const SourceInfo&state)
{
    return _(state, new CommentNode());
}
IntegerLiteralPtr NodeFactory::createInteger(const SourceInfo&state)
{
    return _(state, new IntegerLiteral());
}
FloatLiteralPtr NodeFactory::createFloat(const SourceInfo&state)
{
    return _(state, new FloatLiteral());
}
StringLiteralPtr NodeFactory::createString(const SourceInfo&state)
{
    return _(state, new StringLiteral());
}
UnaryOperatorPtr NodeFactory::createUnary(const SourceInfo&state)
{
    return _(state, new UnaryOperator());
}
TypedPatternPtr NodeFactory::createTypedPattern(const SourceInfo &state)
{
    return _(state, new TypedPattern());
}
IdentifierPtr NodeFactory::createIdentifier(const SourceInfo&state)
{
    return _(state, new Identifier());
}
GenericArgumentDefPtr NodeFactory::createGenericArgumentDef(const SourceInfo& state)
{
    return _(state, new GenericArgumentDef());
}
InOutParameterPtr NodeFactory::createInOutParameter(const SourceInfo&state)
{
    return _(state, new InOutParameter());
}
BinaryOperatorPtr NodeFactory::createBinary(const SourceInfo&state)
{
    return _(state, new BinaryOperator());
}

ArrayLiteralPtr NodeFactory::createArrayLiteral(const SourceInfo& state)
{
    return _(state, new ArrayLiteral());
}
DictionaryLiteralPtr NodeFactory::createDictionaryLiteral(const SourceInfo& state)
{
    return _(state, new DictionaryLiteral());
}
CompileConstantPtr NodeFactory::createCompilecConstant(const SourceInfo&state)
{
    return _(state, new CompileConstant());
}

MemberAccessPtr NodeFactory::createMemberAccess(const SourceInfo&state)
{
    return _(state, new MemberAccess());
}
SubscriptAccessPtr NodeFactory::createSubscriptAccess(const SourceInfo&state)
{
    return _(state, new SubscriptAccess());
}

TypeCheckPtr NodeFactory::createTypeCheck(const SourceInfo&state)
{
    return _(state, new TypeCheck());
}
TypeCastPtr NodeFactory::createTypeCast(const SourceInfo&state)
{
    return _(state, new TypeCast());
}
AssignmentPtr NodeFactory::createAssignment(const SourceInfo&state)
{
    return _(state, new Assignment());
}
ConditionalOperatorPtr NodeFactory::createConditionalOperator(const SourceInfo&state)
{
    return _(state, new ConditionalOperator());
}
ParenthesizedExpressionPtr NodeFactory::createParenthesizedExpression(const SourceInfo& state)
{
    return _(state, new ParenthesizedExpression());
}

InitializerReferencePtr NodeFactory::createInitializerReference(const SourceInfo&state)
{
    return _(state, new InitializerReference());
}
SelfExpressionPtr NodeFactory::createSelfExpression(const SourceInfo&state)
{
    return _(state, new SelfExpression());
}
DynamicTypePtr NodeFactory::createDynamicType(const SourceInfo&state)
{
    return _(state, new DynamicType());
}
ForcedValuePtr NodeFactory::createForcedValue(const SourceInfo&state)
{
    return _(state, new ForcedValue());
}
OptionalChainingPtr NodeFactory::createOptionalChaining(const SourceInfo&state)
{
    return _(state, new OptionalChaining());
}
FunctionCallPtr NodeFactory::createFunctionCall(const SourceInfo& state)
{
    return _(state, new FunctionCall());
}


ForLoopPtr NodeFactory::createForLoop(const SourceInfo& state)
{
    return _(state, new ForLoop());
}
ForInLoopPtr NodeFactory::createForInLoop(const SourceInfo& state)
{
    return _(state, new ForInLoop());
}
WhileLoopPtr NodeFactory::createWhileLoop(const SourceInfo& state)
{
    return _(state, new WhileLoop());
}
IfStatementPtr NodeFactory::createIf(const SourceInfo& state)
{
    return _(state, new IfStatement());
}
DoLoopPtr NodeFactory::createDoLoop(const SourceInfo& state)
{
    return _(state, new DoLoop());
}
SwitchCasePtr NodeFactory::createSwitch(const SourceInfo& state)
{
    return _(state, new SwitchCase());
}
CaseStatementPtr NodeFactory::createCase(const SourceInfo& state)
{
    return _(state, new CaseStatement());
}
BreakStatementPtr NodeFactory::createBreak(const SourceInfo& state)
{
    return _(state, new BreakStatement());
}
ContinueStatementPtr NodeFactory::createContinue(const SourceInfo& state)
{
    return _(state, new ContinueStatement());
}
FallthroughStatementPtr NodeFactory::createFallthrough(const SourceInfo& state)
{
    return _(state, new FallthroughStatement());
}
ReturnStatementPtr NodeFactory::createReturn(const SourceInfo& state)
{
    return _(state, new ReturnStatement());
}
LabeledStatementPtr NodeFactory::createLabel(const SourceInfo& state)
{
    return _(state, new LabeledStatement());
}
CodeBlockPtr NodeFactory::createCodeBlock(const SourceInfo& state)
{
    return _(state, new CodeBlock());
}
ValueBindingPatternPtr NodeFactory::createValueBindingPattern(const SourceInfo& state)
{
    return _(state, new ValueBindingPattern());
}
TuplePtr NodeFactory::createTuple(const SourceInfo& state)
{
    return _(state, new Tuple());
}
ClosurePtr NodeFactory::createClosure(const SourceInfo& state)
{
    return _(state, new Closure());
}
EnumCasePatternPtr NodeFactory::createEnumCasePattern(const SourceInfo&state)
{
    return _(state, new EnumCasePattern());
}


FunctionTypePtr NodeFactory::createFunctionType(const SourceInfo&state)
{
    return _(state, new FunctionType());
}
ArrayTypePtr NodeFactory::createArrayType(const SourceInfo&state)
{
    return _(state, new ArrayType());
}
OptionalTypePtr NodeFactory::createOptionalType(const SourceInfo&state)
{
    return _(state, new OptionalType());
}
ImplicitlyUnwrappedOptionalPtr NodeFactory::createImplicitlyUnwrappedOptional(const SourceInfo&state)
{
    return _(state, new ImplicitlyUnwrappedOptional());
}
TypeIdentifierPtr NodeFactory::createTypeIdentifier(const SourceInfo&state)
{
    return _(state, new TypeIdentifier());
}
ProtocolCompositionPtr NodeFactory::createProtocolComposition(const SourceInfo& state)
{
    return _(state, new ProtocolComposition());
}
TupleTypePtr NodeFactory::createTupleType(const SourceInfo& state)
{
    return _(state, new TupleType());
}
AttributePtr NodeFactory::createAttribute(const SourceInfo& state)
{
    return _(state, new Attribute());
}



ImportPtr NodeFactory::createImport(const SourceInfo&state)
{
    return _(state, new Import());
}
ComputedPropertyPtr NodeFactory::createComputedProperty(const SourceInfo &state)
{
    return _(state, new ComputedProperty());
}

ValueBindingPtr NodeFactory::createValueBinding(const SourceInfo &state)
{
    return _(state, new ValueBinding());
}

ValueBindingsPtr NodeFactory::createValueBindings(const SourceInfo& state)
{
    return  _(state, new ValueBindings());
}
TypeAliasPtr NodeFactory::createTypealias(const SourceInfo&state)
{
    return _(state, new TypeAlias());
}
FunctionDefPtr NodeFactory::createFunction(const SourceInfo&state)
{
    return _(state, new FunctionDef());
}
ParametersPtr NodeFactory::createParameters(const SourceInfo& state)
{
    return _(state, new Parameters());
}
ParameterPtr NodeFactory::createParameter(const SourceInfo& state)
{
    return _(state, new Parameter());
}
EnumDefPtr NodeFactory::createEnum(const SourceInfo&state)
{
    return _(state, new EnumDef());
}
StructDefPtr NodeFactory::createStruct(const SourceInfo&state)
{
    return _(state, new StructDef());
}
ClassDefPtr NodeFactory::createClass(const SourceInfo&state)
{
    return _(state, new ClassDef());
}
ProtocolDefPtr NodeFactory::createProtocol(const SourceInfo&state)
{
    return _(state, new ProtocolDef());
}
InitializerDefPtr NodeFactory::createInitializer(const SourceInfo&state)
{
    return _(state, new InitializerDef());
}
DeinitializerDefPtr NodeFactory::createDeinitializer(const SourceInfo&state)
{
    return _(state, new DeinitializerDef());
}
ExtensionDefPtr NodeFactory::createExtension(const SourceInfo&state)
{
    return _(state, new ExtensionDef());
}
SubscriptDefPtr NodeFactory::createSubscript(const SourceInfo&state)
{
    return _(state, new SubscriptDef());
}
OperatorDefPtr NodeFactory::createOperator(const SourceInfo&state)
{
    return _(state, new OperatorDef());
}
GenericConstraintDefPtr NodeFactory::createGenericConstraintDef(const SourceInfo& state)
{
    return _(state, new GenericConstraintDef());
}
GenericParametersDefPtr NodeFactory::createGenericParametersDef(const SourceInfo& state)
{
    return _(state, new GenericParametersDef());
}



