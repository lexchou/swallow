#include "SelfExpression.h"
#include "NodeVisitor.h"
USE_SWIFT_NS




SelfExpression::SelfExpression()
    :Expression(NodeType::Self)
{
}
SelfExpression::~SelfExpression()
{

}

void SelfExpression::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr SelfExpression::getExpression()
{
    return expression;
}

void SelfExpression::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSelf);
}
