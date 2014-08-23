#ifndef NODE_SERIALIZER_H
#define NODE_SERIALIZER_H
#include "NodeVisitor.h"
#include <ostream>

SWIFT_NS_BEGIN



class NodeSerializer : public NodeVisitor
{
public:
    virtual void visitVariables(const VariablesPtr& node) override;
    virtual void visitVariable(const VariablePtr& node) override;
    virtual void visitConstant(const ConstantPtr& node) override;
    virtual void visitConstants(const ConstantsPtr& node) override;
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
    virtual void visitLetPattern(const LetPatternPtr& node) override;
    virtual void visitVarPattern(const VarPatternPtr& node) override;
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

    virtual void visitString(const StringLiteralPtr& node) override;
    virtual void visitInteger(const IntegerLiteralPtr& node) override;
    virtual void visitFloat(const FloatLiteralPtr& node) override;
public:
    virtual void visitArrayType(const ArrayTypePtr& node) override;
    virtual void visitFunctionType(const FunctionTypePtr& node) override;
    virtual void visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptional& node) override;
    virtual void visitOptionalType(const OptionalTypePtr& node) override;
    virtual void visitProtocolComposition(const ProtocolCompositionPtr& node) override;
    virtual void visitTupleType(const TupleTypePtr& node) override;
    virtual void visitTypeIdentifier(const TypeIdentifierPtr& node) override;
protected:
    virtual void append(const wchar_t* str) = 0;
    virtual void append(const std::wstring& str) = 0;
};


class NodeSerializerW : public NodeSerializer
{
public:
    NodeSerializerW(std::wostream& out);
protected:
    virtual void append(const wchar_t* str) override;
    virtual void append(const std::wstring& str) override;
private:
    std::wostream& out;
};
class NodeSerializerA: public NodeSerializer
{
public:
    NodeSerializerA(std::ostream& out);
protected:
    virtual void append(const wchar_t* str) override;
    virtual void append(const std::wstring& str) override;
private:
    std::ostream& out;

};


SWIFT_NS_END


#endif//NODE_SERIALIZER_H
