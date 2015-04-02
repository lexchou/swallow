/* JSONSerializer.cpp --
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
#include "JSONSerializer.h"
#include "common/ScopedValue.h"
#include "ast/ast.h"
#include "semantics/Type.h"
using namespace std;
using namespace Swallow;


struct JSONHelper
{
    std::wostream& out;
    JSONHelper* parent;
    int attrs;
    int children;
    JSONHelper(JSONHelper* parent, std::wostream& out)
            :out(out), parent(parent), attrs(0), children(0)
    {
        if(parent && parent->children)
            out<<L", ";
        out<<L"{";



    }
    void type(const TypePtr& type)
    {
        if(!type)
            return;
        attr(L"type", type->toString());
    }
    void attr(const wchar_t* key, const wstring& value)
    {
        attr(key, value.c_str());
    }
    void attr(const wchar_t* key, const wchar_t* value)
    {
        if(attrs)
            out<<L", ";
        attrs++;
        out<<L"\"" << key << "\" : \"" << value << L"\"";
    }

    ~JSONHelper()
    {
        out<<L"}";
        if(parent)
            parent->children++;
    }
};

#define JSON JSONHelper json(parent, out);SCOPED_SET(parent, &json);
#define CHILD_BEGIN(N) json.attr(L"node", COMBINE(L, #N)); json.out<<L", \"children\" : [";
#define CHILD_END json.out<<L"]"
#define NODE(N) CHILD_BEGIN(N); NodeVisitor::visit##N(node); CHILD_END

JSONSerializer::JSONSerializer(std::wstringstream& out)
:parent(nullptr), out(out)
{

}
void JSONSerializer::visitValueBindings(const ValueBindingsPtr& node)
{
    JSON;
    NODE(ValueBindings);
}
void JSONSerializer::visitComputedProperty(const ComputedPropertyPtr& node)
{
    JSON;
    json.attr(L"text", node->getName());
    json.type(node->getType());
    NODE(ComputedProperty);
}
void JSONSerializer::visitValueBinding(const ValueBindingPtr& node)
{
    JSON;
    json.type(node->getType());
    CHILD_BEGIN(ValueBinding);
    node->getName()->accept(this);
    NodeVisitor::visitValueBinding(node);
    CHILD_END;
}
void JSONSerializer::visitAssignment(const AssignmentPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(Assignment);
}
void JSONSerializer::visitClass(const ClassDefPtr& node)
{
    JSON;
    json.attr(L"text", node->getIdentifier()->getName());
    NODE(Class);
}
void JSONSerializer::visitStruct(const StructDefPtr& node)
{
    JSON;
    json.attr(L"text", node->getIdentifier()->getName());
    NODE(Struct);
}
void JSONSerializer::visitEnum(const EnumDefPtr& node)
{
    JSON;
    json.attr(L"text", node->getIdentifier()->getName());
    NODE(Enum);
}
void JSONSerializer::visitProtocol(const ProtocolDefPtr& node)
{

    JSON;
    json.attr(L"text", node->getIdentifier()->getName());
    NODE(Protocol);
}
void JSONSerializer::visitExtension(const ExtensionDefPtr& node)
{
    JSON;
    json.attr(L"text", node->getIdentifier()->getName());
    NODE(Extension);
}
void JSONSerializer::visitFunction(const FunctionDefPtr& node)
{
    JSON;
    json.attr(L"text", node->getName());
    NODE(Function);
}
void JSONSerializer::visitDeinit(const DeinitializerDefPtr& node)
{
    JSON;
    NODE(Deinit);

}
void JSONSerializer::visitInit(const InitializerDefPtr& node)
{

    JSON;
    NODE(Init);
}
void JSONSerializer::visitImport(const ImportPtr& node)
{

    JSON;
    NODE(Import);
}
void JSONSerializer::visitSubscript(const SubscriptDefPtr& node)
{

    JSON;
    NODE(Subscript);
}
void JSONSerializer::visitTypeAlias(const TypeAliasPtr& node)
{

    JSON;
    NODE(TypeAlias);
}
void JSONSerializer::visitWhileLoop(const WhileLoopPtr& node)
{

    JSON;
    NODE(WhileLoop);
}
void JSONSerializer::visitForIn(const ForInLoopPtr& node)
{

    JSON;
    NODE(ForIn);
}
void JSONSerializer::visitForLoop(const ForLoopPtr& node)
{
    JSON;
    NODE(ForLoop);

}
void JSONSerializer::visitDoLoop(const DoLoopPtr& node)
{

    JSON;
    NODE(DoLoop);
}
void JSONSerializer::visitLabeledStatement(const LabeledStatementPtr& node)
{

    JSON;
    NODE(LabeledStatement);
}
void JSONSerializer::visitOperator(const OperatorDefPtr& node)
{

    JSON;
    NODE(Operator);
}
void JSONSerializer::visitArrayLiteral(const ArrayLiteralPtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(ArrayLiteral);
}
void JSONSerializer::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(DictionaryLiteral);

}
void JSONSerializer::visitBreak(const BreakStatementPtr& node)
{
    JSON;
    NODE(Break);

}
void JSONSerializer::visitReturn(const ReturnStatementPtr& node)
{

    JSON;
    NODE(Return);
}
void JSONSerializer::visitContinue(const ContinueStatementPtr& node)
{

    JSON;
    NODE(Continue);
}
void JSONSerializer::visitFallthrough(const FallthroughStatementPtr& node)
{
    JSON;
    NODE(Fallthrough);

}
void JSONSerializer::visitIf(const IfStatementPtr& node)
{
    JSON;
    NODE(If);

}
void JSONSerializer::visitSwitchCase(const SwitchCasePtr& node)
{
    JSON;
    NODE(SwitchCase);

}
void JSONSerializer::visitCase(const CaseStatementPtr& node)
{

    JSON;
    NODE(Case);
}
void JSONSerializer::visitCodeBlock(const CodeBlockPtr& node)
{

    JSON;
    NODE(CodeBlock);
}
void JSONSerializer::visitParameter(const ParameterNodePtr& node)
{
    JSON;
    json.attr(L"text", node->getLocalName());
    json.type(node->getType());
    NODE(Parameter);

}
void JSONSerializer::visitParameters(const ParametersNodePtr& node)
{
    JSON;
    NODE(Parameters);

}
void JSONSerializer::visitProgram(const ProgramPtr& node)
{
    JSON;
    NODE(Program);
}
void JSONSerializer::visitValueBindingPattern(const ValueBindingPatternPtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(ValueBindingPattern);
}
void JSONSerializer::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(ConditionalOperator);
}
void JSONSerializer::visitBinaryOperator(const BinaryOperatorPtr& node)
{

    JSON;
    json.attr(L"text", node->getOperator());
    json.type(node->getType());
    NODE(BinaryOperator);
}
void JSONSerializer::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    JSON;
    json.attr(L"text", node->getOperator());
    json.type(node->getType());
    NODE(UnaryOperator);

}
void JSONSerializer::visitTuple(const TuplePtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(Tuple);

}
void JSONSerializer::visitIdentifier(const IdentifierPtr& node)
{
    JSON;
    json.attr(L"text", node->getIdentifier());
    json.type(node->getType());
    NODE(Identifier);

}
void JSONSerializer::visitCompileConstant(const CompileConstantPtr& node)
{
    JSON;
    json.attr(L"text", node->getName());
    json.type(node->getType());
    NODE(CompileConstant);

}
void JSONSerializer::visitSubscriptAccess(const SubscriptAccessPtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(SubscriptAccess);
}
void JSONSerializer::visitMemberAccess(const MemberAccessPtr& node)
{
    JSON;
    if(node->getField() != nullptr)
    {
        json.attr(L"field", node->getField()->getIdentifier());
    }
    else
    {
        wstringstream ss;
        ss<<node->getIndex();
        json.attr(L"field", ss.str());
    }
    json.type(node->getType());
    NODE(MemberAccess);

}
void JSONSerializer::visitFunctionCall(const FunctionCallPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(FunctionCall);

}
void JSONSerializer::visitClosure(const ClosurePtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(Closure);
}
void JSONSerializer::visitSelf(const SelfExpressionPtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(Self);
}
void JSONSerializer::visitInitializerReference(const InitializerReferencePtr& node)
{
    JSON;
    NODE(InitializerReference);

}
void JSONSerializer::visitTypedPattern(const TypedPatternPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(TypedPattern);

}
void JSONSerializer::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(EnumCasePattern);

}
void JSONSerializer::visitDynamicType(const DynamicTypePtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(DynamicType);

}
void JSONSerializer::visitForcedValue(const ForcedValuePtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(ForcedValue);

}
void JSONSerializer::visitOptionalChaining(const OptionalChainingPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(OptionalChaining);

}
void JSONSerializer::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(ParenthesizedExpression);
}
void JSONSerializer::visitString(const StringLiteralPtr& node)
{
    JSON;
    json.attr(L"text", node->value);
    json.type(node->getType());
    NODE(String);

}
void JSONSerializer::visitStringInterpolation(const StringInterpolationPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(StringInterpolation);

}
void JSONSerializer::visitInteger(const IntegerLiteralPtr& node)
{
    JSON;
    json.attr(L"text", node->valueAsString);
    json.type(node->getType());
    NODE(Integer);

}
void JSONSerializer::visitFloat(const FloatLiteralPtr& node)
{
    JSON;
    json.attr(L"text", node->valueAsString);
    json.type(node->getType());
    NODE(Float);

}
void JSONSerializer::visitNilLiteral(const NilLiteralPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(NilLiteral);

}
void JSONSerializer::visitBooleanLiteral(const BooleanLiteralPtr& node)
{
    JSON;
    json.type(node->getType());
    json.attr(L"text", node->getValue() ? L"true" : L"false");
    NODE(BooleanLiteral);

}
void JSONSerializer::visitArrayType(const ArrayTypePtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(ArrayType);

}
void JSONSerializer::visitFunctionType(const FunctionTypePtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(FunctionType);
}
void JSONSerializer::visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptionalPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(ImplicitlyUnwrappedOptional);

}
void JSONSerializer::visitOptionalType(const OptionalTypePtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(OptionalType);

}
void JSONSerializer::visitProtocolComposition(const ProtocolCompositionPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(ProtocolComposition);

}
void JSONSerializer::visitTupleType(const TupleTypePtr& node)
{

    JSON;
    json.type(node->getType());
    NODE(TupleType);
}
void JSONSerializer::visitTypeIdentifier(const TypeIdentifierPtr& node)
{
    JSON;
    json.type(node->getType());
    NODE(TypeIdentifier);

}
