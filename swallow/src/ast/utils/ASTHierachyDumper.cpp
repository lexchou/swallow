/* ASTHierachyDumper.cpp --
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
#include "ast/utils/ASTHierachyDumper.h"
#include "ast/ast.h"
#include "semantics/Type.h"
USE_SWALLOW_NS



ASTHierachyDumper::ASTHierachyDumper(std::wostream& out)
:out(out), indent(0)
{

}
void ASTHierachyDumper::write(const wchar_t *str)
{
    last = str;
    out<<str;
}
void ASTHierachyDumper::attr(const wchar_t* key, const TypePtr& value)
{
    if(value)
        attr(key, value->toString());
    else
        attr(key, L"null");
}
void ASTHierachyDumper::attr(const wchar_t* key, const std::wstring& value)
{
    if(last.back() != ' ')
        out<<L" ";
    if(key)
        write(key);
    if(key && !value.empty())
        write(L"=");
    if(!value.empty())
    {
        write(L"'");
        write(value.c_str());
        write(L"'");
    }
}
void ASTHierachyDumper::beginIndent(const wchar_t* name)
{
    if(last.back() != '\n')
        out<<std::endl;
    for(int i = 0; i < indent; i++)
    {
        write(L"  ");
    }
    indent++;
    write(L"(");
    write(name);
}

void ASTHierachyDumper::endIndent()
{
    write(L")");
    indent--;
    if(indent == 0)
        out<<std::endl;
}
void ASTHierachyDumper::visitDeinit(const DeinitializerDefPtr& node)
{

}

void ASTHierachyDumper::visitInit(const InitializerDefPtr& node)
{

}

void ASTHierachyDumper::visitImport(const ImportPtr& node)
{

}

void ASTHierachyDumper::visitSubscript(const SubscriptDefPtr& node)
{

}

void ASTHierachyDumper::visitTypeAlias(const TypeAliasPtr& node)
{
}

void ASTHierachyDumper::visitForIn(const ForInLoopPtr& node)
{
}

void ASTHierachyDumper::visitForLoop(const ForLoopPtr& node)
{
}

void ASTHierachyDumper::visitDoLoop(const DoLoopPtr& node)
{
}

void ASTHierachyDumper::visitLabeledStatement(const LabeledStatementPtr& node)
{

}

void ASTHierachyDumper::visitOperator(const OperatorDefPtr& node)
{

}

void ASTHierachyDumper::visitArrayLiteral(const ArrayLiteralPtr& node)
{

}

void ASTHierachyDumper::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
}

void ASTHierachyDumper::visitBreak(const BreakStatementPtr& node)
{
}

void ASTHierachyDumper::visitReturn(const ReturnStatementPtr& node)
{

}

void ASTHierachyDumper::visitContinue(const ContinueStatementPtr& node)
{
}

void ASTHierachyDumper::visitFallthrough(const FallthroughStatementPtr& node)
{
}

void ASTHierachyDumper::visitIf(const IfStatementPtr& node)
{
    beginIndent(L"if");
    node->getCondition()->accept(this);
    if(node->getThen())
        node->getThen()->accept(this);
    if(node->getElse())
        node->getElse()->accept(this);
    endIndent();
}

void ASTHierachyDumper::visitSwitchCase(const SwitchCasePtr& node)
{
}

void ASTHierachyDumper::visitCase(const CaseStatementPtr& node)
{
}

void ASTHierachyDumper::visitCodeBlock(const CodeBlockPtr& node)
{
    beginIndent(L"block");
    for(const StatementPtr& st : *node)
    {
        st->accept(this);
    }
    endIndent();
}

void ASTHierachyDumper::visitParameter(const ParameterPtr& node)
{

}

void ASTHierachyDumper::visitParameters(const ParametersPtr& node)
{

}

void ASTHierachyDumper::visitProgram(const ProgramPtr& node)
{
    beginIndent(L"program");
    for(auto st : *node)
    {
        st->accept(this);
    }
    endIndent();
}

void ASTHierachyDumper::visitValueBindingPattern(const ValueBindingPatternPtr& node)
{
    beginIndent(L"pattern_binding_decl");
    attr(L"type", node->getType());
    node->getBinding()->accept(this);
    endIndent();
}


void ASTHierachyDumper::visitValueBindings(const ValueBindingsPtr &node)
{
    beginIndent(L"value_bindings");
    NodeVisitor::visitValueBindings(node);
    endIndent();
}

void ASTHierachyDumper::visitComputedProperty(const ComputedPropertyPtr& node)
{

}

void ASTHierachyDumper::visitValueBinding(const ValueBindingPtr &node)
{
    beginIndent(L"value_binding");
    attr(L"type", node->getType());
    node->getName()->accept(this);
    if(node->getInitializer())
        node->getInitializer()->accept(this);
    endIndent();

}

void ASTHierachyDumper::visitAssignment(const AssignmentPtr& node)
{
    beginIndent(L"assign");
    attr(L"type", node->getType());
    node->getLHS()->accept(this);
    node->getRHS()->accept(this);
    endIndent();
}

void ASTHierachyDumper::visitClass(const ClassDefPtr& node)
{

}

void ASTHierachyDumper::visitStruct(const StructDefPtr& node)
{

}

void ASTHierachyDumper::visitEnum(const EnumDefPtr& node)
{

}

void ASTHierachyDumper::visitProtocol(const ProtocolDefPtr& node)
{

}

void ASTHierachyDumper::visitExtension(const ExtensionDefPtr& node)
{
    beginIndent(L"extension_decl");
    attr(L"name", node->getIdentifier()->getName());
    NodeVisitor::visitExtension(node);
    endIndent();
}

void ASTHierachyDumper::visitFunction(const FunctionDefPtr& node)
{
    beginIndent(L"func_decl");
    attr(L"type", node->getType());
    attr(L"name", node->getName());
    NodeVisitor::visitFunction(node);
    endIndent();
}

void ASTHierachyDumper::visitWhileLoop(const WhileLoopPtr& node)
{

}

void ASTHierachyDumper::visitConditionalOperator(const ConditionalOperatorPtr& node)
{

}

void ASTHierachyDumper::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    beginIndent(L"call_expr");
    attr(L"type", node->getType());
    attr(L"name", node->getOperator());
    node->getLHS()->accept(this);
    node->getRHS()->accept(this);
    endIndent();
}

void ASTHierachyDumper::visitUnaryOperator(const UnaryOperatorPtr& node)
{
}

void ASTHierachyDumper::visitTuple(const TuplePtr& node)
{

}

void ASTHierachyDumper::visitIdentifier(const IdentifierPtr& node)
{
    beginIndent(L"pattern_named");
    attr(nullptr, node->getIdentifier());
    attr(L"type", node->getType());
    endIndent();
}
void ASTHierachyDumper::visitTypedPattern(const TypedPatternPtr& node)
{

}
void ASTHierachyDumper::visitCompileConstant(const CompileConstantPtr& node)
{
}

void ASTHierachyDumper::visitSubscriptAccess(const SubscriptAccessPtr& node)
{

}

void ASTHierachyDumper::visitMemberAccess(const MemberAccessPtr& node)
{
    beginIndent(L"dot_expr");
    if(node->getField())
        attr(L"field", node->getField()->getIdentifier());
    else
    {
        std::wstringstream ss;
        ss<<node->getIndex();
        attr(L"field", ss.str());
    }
    attr(L"type", node->getType());
    if(node->getSelf())
        node->getSelf()->accept(this);
    endIndent();
}

void ASTHierachyDumper::visitFunctionCall(const FunctionCallPtr& node)
{
    beginIndent(L"call_expr");
    attr(L"type", node->getType());
    node->getFunction()->accept(this);
    if(node->getArguments())
        node->getArguments()->accept(this);
    if(node->getTrailingClosure())
        node->getTrailingClosure()->accept(this);

    endIndent();
}

void ASTHierachyDumper::visitClosure(const ClosurePtr& node)
{

}

void ASTHierachyDumper::visitSelf(const SelfExpressionPtr& node)
{
}

void ASTHierachyDumper::visitInitializerReference(const InitializerReferencePtr& node)
{

}


void ASTHierachyDumper::visitEnumCasePattern(const EnumCasePatternPtr& node)
{
}

void ASTHierachyDumper::visitDynamicType(const DynamicTypePtr& node)
{

}

void ASTHierachyDumper::visitForcedValue(const ForcedValuePtr& node)
{

}

void ASTHierachyDumper::visitOptionalChaining(const OptionalChainingPtr& node)
{
    beginIndent(L"bind_optional_expr");
    attr(L"type", node->getType());
    node->getExpression()->accept(this);
    endIndent();
}

void ASTHierachyDumper::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    beginIndent(L"paren_expr");
    attr(L"type", node->getType());
    for(auto term : *node)
    {
        term.expression->accept(this);
    }

    endIndent();
}

void ASTHierachyDumper::visitStringInterpolation(const StringInterpolationPtr &node)
{

}
void ASTHierachyDumper::visitString(const StringLiteralPtr& node)
{

}
void ASTHierachyDumper::visitInteger(const IntegerLiteralPtr& node)
{

}
void ASTHierachyDumper::visitFloat(const FloatLiteralPtr& node)
{

}

void ASTHierachyDumper::visitNilLiteral(const NilLiteralPtr& node)
{

}
void ASTHierachyDumper::visitBooleanLiteral(const BooleanLiteralPtr& node)
{

}

void ASTHierachyDumper::visitArrayType(const ArrayTypePtr& node)
{

}
void ASTHierachyDumper::visitFunctionType(const FunctionTypePtr& node)
{

}
void ASTHierachyDumper::visitImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptionalPtr& node)
{

}
void ASTHierachyDumper::visitOptionalType(const OptionalTypePtr& node)
{

}
void ASTHierachyDumper::visitProtocolComposition(const ProtocolCompositionPtr& node)
{

}
void ASTHierachyDumper::visitTupleType(const TupleTypePtr& node)
{

}
void ASTHierachyDumper::visitTypeIdentifier(const TypeIdentifierPtr& node)
{

}
