/* NodeSerializer.cpp --
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
#include "ast/utils/NodeSerializer.h"
#include "ast/ast.h"
//#include <codecvt>

USE_SWALLOW_NS


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
    append(node->isReadOnly() ? L"let " : L"var ");
    NodeVisitor::visitValueBindings(node);
}
void NodeSerializer::visitValueBinding(const ValueBindingPtr &node)
{
    node->getName()->accept(this);
    if(node->getDeclaredType())
    {
        append(L" : ");
        node->getDeclaredType()->accept(this);
    }
    if(node->getInitializer())
    {
        append(L" = ");
        node->getInitializer()->accept(this);
    }
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
    for(const ParametersNodePtr& params : node->getParametersList())
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
    append(L"[");
    for(auto el : *node)
    {
        el->accept(this);
    }
    append(L"]");
}
void NodeSerializer::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    bool first = true;
    append(L"[");
    for(auto item : *node)
    {
        if(!first)
            append(L", ");
        first = false;
        item.first->accept(this);
        append(L" : ");
        item.second->accept(this);
    }
    append(L"]");
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
void NodeSerializer::visitParameter(const ParameterNodePtr& node)
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
void NodeSerializer::visitParameters(const ParametersNodePtr& node)
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
    append(L"(");
    bool first = true;
    for(auto element : *node)
    {
        if(!first)
            append(L", ");
        first = false;
        //element.vaccept(this);
        element->accept(this);
    }
    append(L")");
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
    node->getFunction()->accept(this);
    if(node->getArguments())
        node->getArguments()->accept(this);
    if(node->getTrailingClosure())
        node->getTrailingClosure()->accept(this);
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
    append(L"(");
    bool first = true;
    for(auto element : *node)
    {
        if(!first)
            append(L", ");
        first = false;
        if(!element.name.empty())
        {
            append(element.name);
            append(L" : ");
        }
        element.expression->accept(this);
    }
    append(L")");
}
void NodeSerializer::visitString(const StringLiteralPtr& node)
{
    append(L"\"");
    append(node->value);
    append(L"\"");
}
void NodeSerializer::visitInteger(const IntegerLiteralPtr& node)
{
    append(node->valueAsString);

}
void NodeSerializer::visitFloat(const FloatLiteralPtr& node)
{
    append(node->valueAsString);
}
void NodeSerializer::visitNilLiteral(const NilLiteralPtr& node)
{
    append(L"nil");
}
void NodeSerializer::visitBooleanLiteral(const BooleanLiteralPtr& node)
{
    if(node->getValue())
        append(L"true");
    else
        append(L"false");
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
void NodeSerializer::visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptionalPtr& node)
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
