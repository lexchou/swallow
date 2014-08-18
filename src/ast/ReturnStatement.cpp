#include "ReturnStatement.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ReturnStatement::ReturnStatement()
    :Statement(NodeType::Return), expression(NULL)
{
}
ReturnStatement::~ReturnStatement()
{
}

void ReturnStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitReturn);
}

void ReturnStatement::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr ReturnStatement::getExpression()
{
    return expression;
}
