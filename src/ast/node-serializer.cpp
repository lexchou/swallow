#include "node-serializer.h"
#include "ast.h"
#include <codecvt>

USE_SWIFT_NS


NodeSerializerW::NodeSerializerW(std::wostream& out)
:out(out)
{

}
void NodeSerializerW::append(const wchar_t* str)
{
    out<<str;
}
void NodeSerializerW::append(const std::wstring& str)
{
    out<<str;
}
NodeSerializerA::NodeSerializerA(std::ostream& out)
:out(out)
{

}
void NodeSerializerA::append(const wchar_t* str)
{
    typedef std::codecvt_utf8<wchar_t> convert_typeX;
    std::wstring_convert<convert_typeX, wchar_t> converterX;

    out<<converterX.to_bytes(str);
}
void NodeSerializerA::append(const std::wstring& str)
{
    append(str.c_str());
}




void NodeSerializer::visitVariables(const VariablesPtr& node)
{

}
void NodeSerializer::visitVariable(const VariablePtr& node)
{

}
void NodeSerializer::visitConstant(const ConstantPtr& node)
{

}
void NodeSerializer::visitConstants(const ConstantsPtr& node)
{

}
void NodeSerializer::visitAssignment(const AssignmentPtr& node)
{

}
void NodeSerializer::visitClass(const ClassDefPtr& node)
{

}
void NodeSerializer::visitStruct(const StructDefPtr& node)
{

}
void NodeSerializer::visitEnum(const EnumDefPtr& node)
{

}
void NodeSerializer::visitProtocol(const ProtocolDefPtr& node)
{

}
void NodeSerializer::visitExtension(const ExtensionDefPtr& node)
{

}
void NodeSerializer::visitFunction(const FunctionDefPtr& node)
{

}
void NodeSerializer::visitDeinit(const DeinitializerDefPtr& node)
{

}
void NodeSerializer::visitInit(const InitializerDefPtr& node)
{

}
void NodeSerializer::visitImport(const ImportPtr& node)
{

}
void NodeSerializer::visitSubscript(const SubscriptDefPtr& node)
{

}
void NodeSerializer::visitTypeAlias(const TypeAliasPtr& node)
{

}
void NodeSerializer::visitWhileLoop(const WhileLoopPtr& node)
{

}
void NodeSerializer::visitForIn(const ForInLoopPtr& node)
{

}
void NodeSerializer::visitForLoop(const ForLoopPtr& node)
{

}
void NodeSerializer::visitDoLoop(const DoLoopPtr& node)
{

}
void NodeSerializer::visitLabeledStatement(const LabeledStatementPtr& node)
{

}
void NodeSerializer::visitOperator(const OperatorDefPtr& node)
{

}
void NodeSerializer::visitArrayLiteral(const ArrayLiteralPtr& node)
{

}
void NodeSerializer::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{

}
void NodeSerializer::visitBreak(const BreakStatementPtr& node)
{

}
void NodeSerializer::visitReturn(const ReturnStatementPtr& node)
{

}
void NodeSerializer::visitContinue(const ContinueStatementPtr& node)
{

}
void NodeSerializer::visitFallthrough(const FallthroughStatementPtr& node)
{

}
void NodeSerializer::visitIf(const IfStatementPtr& node)
{

}
void NodeSerializer::visitSwitchCase(const SwitchCasePtr& node)
{

}
void NodeSerializer::visitCase(const CaseStatementPtr& node)
{

}
void NodeSerializer::visitCodeBlock(const CodeBlockPtr& node)
{

}
void NodeSerializer::visitParameter(const ParameterPtr& node)
{

}
void NodeSerializer::visitParameters(const ParametersPtr& node)
{

}
void NodeSerializer::visitProgram(const ProgramPtr& node)
{

}
void NodeSerializer::visitLetPattern(const LetPatternPtr& node)
{

}
void NodeSerializer::visitVarPattern(const VarPatternPtr& node)
{

}
void NodeSerializer::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

}
void NodeSerializer::visitBinaryOperator(const BinaryOperatorPtr& node)
{

}
void NodeSerializer::visitUnaryOperator(const UnaryOperatorPtr& node)
{

}
void NodeSerializer::visitTuple(const TuplePtr& node)
{

}
void NodeSerializer::visitIdentifier(const IdentifierPtr& node)
{
    append(node->getIdentifier());
}
void NodeSerializer::visitCompileConstant(const CompileConstantPtr& node)
{

}
void NodeSerializer::visitSubscriptAccess(const SubscriptAccessPtr& node)
{

}
void NodeSerializer::visitMemberAccess(const MemberAccessPtr& node)
{

}
void NodeSerializer::visitFunctionCall(const FunctionCallPtr& node)
{

}
void NodeSerializer::visitClosure(const ClosurePtr& node)
{

}
void NodeSerializer::visitSelf(const SelfExpressionPtr& node)
{

}
void NodeSerializer::visitInitializerReference(const InitializerReferencePtr& node)
{

}
void NodeSerializer::visitEnumCasePattern(const EnumCasePatternPtr& node)
{

}
void NodeSerializer::visitDynamicType(const DynamicTypePtr& node)
{

}
void NodeSerializer::visitForcedValue(const ForcedValuePtr& node)
{

}
void NodeSerializer::visitOptionalChaining(const OptionalChainingPtr& node)
{

}
void NodeSerializer::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{

}
void NodeSerializer::visitString(const StringLiteralPtr& node)
{

}
void NodeSerializer::visitInteger(const IntegerLiteralPtr& node)
{

}
void NodeSerializer::visitFloat(const FloatLiteralPtr& node)
{

}