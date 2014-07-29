#include "if-statement.h"
#include "expression.h"
#include "code-block.h"
#include "node-visitor.h"
USE_SWIFT_NS


IfStatement::IfStatement()
    :Statement(NodeType::If), condition(NULL), thenPart(NULL), elsePart(NULL)
{
}

IfStatement::~IfStatement()
{

}

void IfStatement::serialize(std::wostream& out)
{
    out<<L"if ";
    getCondition()->serialize(out);
    out<<std::endl;
    getThen()->serialize(out);
    if(getElse() != NULL)
    {
        out<<L"else";
        getElse()->serialize(out);
    }
}
void IfStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitIf);
}


void IfStatement::setCondition(const ExpressionPtr& expr)
{
    this->condition = expr;
}
ExpressionPtr IfStatement::getCondition()
{
    return condition;
}

void IfStatement::setThen(const CodeBlockPtr& thenPart)
{
    this->thenPart = thenPart;
}
CodeBlockPtr IfStatement::getThen()
{
    return thenPart;
}

void IfStatement::setElse(const StatementPtr& elsePart)
{
    this->elsePart = elsePart;
}
StatementPtr IfStatement::getElse()
{
    return elsePart;
}