/* OperatorResolver.cpp --
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
#include "semantics/OperatorResolver.h"
#include <cassert>
#include "ast/ast.h"
#include "common/CompilerResults.h"
#include "semantics/SymbolRegistry.h"
#include "common/Errors.h"
#include "semantics/ScopedNodes.h"

USE_SWALLOW_NS
using namespace std;


OperatorResolver::OperatorResolver(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
        :SemanticPass(symbolRegistry, compilerResults)
{
}

/*!
 * Rotate the AST tree if required
 */
bool OperatorResolver::rotateRequired(const OperatorPtr& lhs, const OperatorPtr& rhs)
{
    BinaryOperatorPtr left = dynamic_pointer_cast<BinaryOperator>(lhs);
    BinaryOperatorPtr right = dynamic_pointer_cast<BinaryOperator>(rhs);
    if(!left || !right)
        return false;
    OperatorInfo* leftOp = symbolRegistry->getOperator(left->getOperator(), OperatorType::InfixBinary);
    OperatorInfo* rightOp = symbolRegistry->getOperator(right->getOperator(), OperatorType::InfixBinary);
    if(!leftOp || !rightOp)
        return false;
    if(leftOp->precedence.infix == rightOp->precedence.infix)
    {
        //only right associativity is required because it's left associativity
        return leftOp->associativity == Associativity::Right;
    }
    return leftOp->precedence.infix < rightOp->precedence.infix;
}
/*!
 * Sort the AST tree by precedence, if the precedence is the same, sort by associativity
 */
OperatorPtr OperatorResolver::sortExpression(const OperatorPtr& op)
{
    OperatorPtr root = op;
    OperatorPtr c;
    if(op->numChildren() != 2)
        return op;
    NodePtr left = op->get(0);
    NodePtr right = op->get(1);
    if(left && (c = std::dynamic_pointer_cast<Operator>(left)) && c->numChildren() == 2)
    {
        left = c = sortExpression(c);
        if(rotateRequired(c, root))
        {
            //make c the root
            NodePtr r = c->get(1);
            c->set(1, root);
            root->set(0, r);
            root = c;
            //sort new right child
            NodePtr r2 = sortExpression(std::static_pointer_cast<Operator>(c->get(1)));
            c->set(1, r2);
            return root;
        }
    }
    //sort old right child
    c = std::dynamic_pointer_cast<Operator>(right);
    if(c != NULL)
        sortExpression(c);
    return root;
}


void OperatorResolver::visitOperator(const OperatorDefPtr& node)
{
    NodePtr parentNode = node->getParentNode();
    if(!parentNode || parentNode->getNodeType() != NodeType::Program)
    {
        error(node, Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, L"operator");
        abort();
        return;
    }
    //register operator
    if(node->getType() == OperatorType::InfixBinary)
    {
        //check precedence range
        if(node->getPrecedence() < 0 || node->getPrecedence() > 255)
        {
            error(node, Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE);
            abort();
        }
    }

    bool r = symbolRegistry->registerOperator(node->getName(), node->getType(), node->getAssociativity(), node->getPrecedence(), node->isAssignment());
    if(!r)
    {
        error(node, Errors::E_OPERATOR_REDECLARED);
        abort();
    }
}

void OperatorResolver::visitAssignment(const AssignmentPtr& node)
{
    visitBinaryOperator(node);
}
void OperatorResolver::visitComputedProperty(const ComputedPropertyPtr& node)
{
    node->setInitializer(transform<Expression>(node->getInitializer()));
    if(node->getGetter())
        node->getGetter()->accept(this);
    if(node->getSetter())
        node->getSetter()->accept(this);
    if(node->getWillSet())
        node->getWillSet()->accept(this);
    if(node->getDidSet())
        node->getDidSet()->accept(this);
}

void OperatorResolver::visitProtocol(const ProtocolDefPtr& node)
{
    //ignore everything under protocol
}

void OperatorResolver::visitClosure(const ClosurePtr& node)
{
    for(auto& st : *node)
    {
        st = transform<Statement>(st);
    }
}


void OperatorResolver::visitProgram(const ProgramPtr& node)
{
    ScopedProgramPtr program = static_pointer_cast<ScopedProgram>(node);
    symbolRegistry->setFileScope(program->getScope());
    for(auto& st : *node)
    {
        st = transform<Statement>(st);
    }
}
void OperatorResolver::visitCodeBlock(const CodeBlockPtr& node)
{
    for(auto& st : *node)
    {
        st = transform<Statement>(st);
    }
}

void OperatorResolver::visitValueBinding(const ValueBindingPtr& node)
{
    node->setInitializer(transform<Expression>(node->getInitializer()));
}

void OperatorResolver::visitStringInterpolation(const StringInterpolationPtr& node)
{
    for(auto& expr : *node)
    {
        expr = transform<Expression>(expr);
    }
}

void OperatorResolver::visitConditionalOperator(const ConditionalOperatorPtr& node)
{
    node->setCondition(transform<Pattern>(node->getCondition()));
    node->setTrueExpression(transform<Expression>(node->getTrueExpression()));
    node->setFalseExpression(transform<Expression>(node->getFalseExpression()));
}
void OperatorResolver::visitBinaryOperator(const BinaryOperatorPtr& node)
{
    node->setRHS(transform<Pattern>(node->getRHS()));
    node->setLHS(transform<Pattern>(node->getLHS()));
}
void OperatorResolver::visitUnaryOperator(const UnaryOperatorPtr& node)
{
    node->setOperand(transform<Expression>(node->getOperand()));
}
void OperatorResolver::visitTuple(const TuplePtr& node)
{
    for(PatternPtr& p : *node)
    {
        p = transform<Pattern>(p);
    }
}

void OperatorResolver::visitReturn(const ReturnStatementPtr& node)
{
    node->setExpression(transform<Expression>(node->getExpression()));
}
void OperatorResolver::visitParenthesizedExpression(const ParenthesizedExpressionPtr& node)
{
    for(auto& p : *node)
    {
        p.expression = transform<Expression>(p.expression);
    }
}
void OperatorResolver::visitArrayLiteral(const ArrayLiteralPtr& node)
{
    for(auto& p : *node)
    {
        p = transform<Expression>(p);
    }
}
void OperatorResolver::visitDictionaryLiteral(const DictionaryLiteralPtr& node)
{
    for(auto& el : *node)
    {
        el.first = transform<Expression>(el.first);
        el.second = transform<Expression>(el.second);
    }
}

void OperatorResolver::visitForcedValue(const ForcedValuePtr& node)
{
    node->setExpression(transform<Expression>(node->getExpression()));
}
void OperatorResolver::visitOptionalChaining(const OptionalChainingPtr& node)
{
    node->setExpression(transform<Expression>(node->getExpression()));
}
void OperatorResolver::visitIf(const IfStatementPtr& node)
{
    node->setCondition(transform<Expression>(node->getCondition()));
    node->getThen()->accept(this);
    if(node->getElse())
        node->getElse()->accept(this);
}
void OperatorResolver::visitSwitchCase(const SwitchCasePtr& node)
{
    node->setControlExpression(transform<Expression>(node->getControlExpression()));
    for(auto c : *node)
    {
        c->accept(this);
    }
    if(node->getDefaultCase())
        node->getDefaultCase()->accept(this);
}
void OperatorResolver::visitCase(const CaseStatementPtr& node)
{
    node->getCodeBlock()->accept(this);
}
