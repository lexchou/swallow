#include "node-factory.h"
#include "ast/ast.h"
USE_SWIFT_NS;


NodeFactory::NodeFactory()
    :autoReleasePool(NULL)
{
}
void NodeFactory::setAutoReleasePool(AutoReleasePool* autoReleasePool)
{
    this->autoReleasePool = autoReleasePool;
}

void NodeFactory::bindNode(const SourceInfo&s, Node* n)
{
    *n->getSourceInfo() = s;
    n->setAutoReleasePool(autoReleasePool);
}


ProgramPtr NodeFactory::createProgram()
{
    return _(SourceInfo(), new Program());
}
CommentNodePtr NodeFactory::createComment(const SourceInfo&state)
{
    return _(state, new CommentNode());
}
BooleanLiteralPtr NodeFactory::createBoolean(const SourceInfo&state)
{
    return _(state, new BooleanLiteral());
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
IdentifierPtr NodeFactory::createIdentifier(const SourceInfo&state)
{
    return _(state, new Identifier());
}
GenericArgumentPtr NodeFactory::createGenericArgument(const SourceInfo& state)
{
    return _(state, new GenericArgument());
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
LetPatternPtr NodeFactory::createLetPattern(const SourceInfo& state)
{
    return _(state, new LetPattern());
}
VarPatternPtr NodeFactory::createVarPattern(const SourceInfo& state)
{
    return _(state, new VarPattern());
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
ConstantPtr NodeFactory::createConstant(const SourceInfo&state)
{
    return _(state, new Constant());
}
ConstantsPtr NodeFactory::createConstants(const SourceInfo&state)
{
    return _(state, new Constants());
}

VariablesPtr NodeFactory::createVariables(const SourceInfo&state)
{
    return _(state, new Variables());
}

VariablePtr NodeFactory::createVariable(const SourceInfo& state)
{
    return  _(state, new Variable());
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
GenericConstraintPtr NodeFactory::createGenericConstraint(const SourceInfo& state)
{
    return _(state, new GenericConstraint());
}
GenericParametersPtr NodeFactory::createGenericParameters(const SourceInfo& state)
{
    return _(state, new GenericParameters());
}



