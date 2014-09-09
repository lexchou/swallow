#ifndef NODE_VISITOR_H
#define NODE_VISITOR_H
#include "swift_conf.h"
#include "ast-decl.h"

SWIFT_NS_BEGIN

class Statement;
class NodeVisitor
{
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
    virtual void visitParameter(const ParameterPtr& node);
    virtual void visitParameters(const ParametersPtr& node);
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
    virtual void visitInteger(const IntegerLiteralPtr& node);
    virtual void visitFloat(const FloatLiteralPtr& node);
public:
    virtual void visitArrayType(const ArrayTypePtr& node);
    virtual void visitFunctionType(const FunctionTypePtr& node);
    virtual void visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptional& node);
    virtual void visitOptionalType(const OptionalTypePtr& node);
    virtual void visitProtocolComposition(const ProtocolCompositionPtr& node);
    virtual void visitTupleType(const TupleTypePtr& node);
    virtual void visitTypeIdentifier(const TypeIdentifierPtr& node);
};


SWIFT_NS_END
#endif//NODE_VISITOR_H
