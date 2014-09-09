#include "NodeSerializer.h"
#include "ast.h"
//#include <codecvt>

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

inline std::string narrow(std::wstring const& text)
{
    std::locale const loc("");
    wchar_t const* from = text.c_str();
    std::size_t const len = text.size();
    std::vector<char> buffer(len + 1);
    std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
    return std::string(&buffer[0], &buffer[len]);
}

void NodeSerializerA::append(const wchar_t* str)
{
    //typedef std::codecvt_utf8<wchar_t> convert_typeX;
    //std::wstring_convert<convert_typeX, wchar_t> converterX;

    //out<<converterX.to_bytes(str);
    std::string s = narrow(str);
    out<<s;
}
void NodeSerializerA::append(const std::wstring& str)
{
    append(str.c_str());
}




void NodeSerializer::visitValueBindings(const ValueBindingsPtr &node)
{

}
void NodeSerializer::visitValueBinding(const ValueBindingPtr &node)
{

}
void NodeSerializer::visitComputedProperty(const ComputedPropertyPtr &node)
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
    append(L"func ");
    append(node->getName());
    for(const ParametersPtr& params : node->getParametersList())
    {
        params->accept(this);
    }
    node->getBody()->accept(this);
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
    append(L"{");
    for(auto st : *node)
    {
        st->accept(this);
    }
    append(L"}");
}
void NodeSerializer::visitParameter(const ParameterPtr& node)
{
    if(node->isInout())
        append(L"inout ");
    if(node->isShorthandExternalName())
        append(L"#");
    else if(!node->getExternalName().empty())
    {
        append(node->getExternalName());
        append(L" ");
    }
    append(node->getLocalName());

    append(L" : ");
    node->getDeclaredType()->accept(this);
}
void NodeSerializer::visitParameters(const ParametersPtr& node)
{
    append(L"(");
    bool first = true;
    for(auto param : *node)
    {
        if(!first)
            append(L", ");
        param->accept(this);
        first = false;
    }
    append(L")");
}
void NodeSerializer::visitProgram(const ProgramPtr& node)
{

}

void NodeSerializer::visitValueBindingPattern(const ValueBindingPatternPtr& node)
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

void NodeSerializer::visitArrayType(const ArrayTypePtr& node)
{
    append(L"[");
    node->getInnerType()->accept(this);
    append(L"]");
}
void NodeSerializer::visitFunctionType(const FunctionTypePtr& node)
{

}
void NodeSerializer::visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptional& node)
{

}
void NodeSerializer::visitOptionalType(const OptionalTypePtr& node)
{

}
void NodeSerializer::visitProtocolComposition(const ProtocolCompositionPtr& node)
{

}
void NodeSerializer::visitTupleType(const TupleTypePtr& node)
{

}
void NodeSerializer::visitTypeIdentifier(const TypeIdentifierPtr& node)
{
    append(node->getName());
}
