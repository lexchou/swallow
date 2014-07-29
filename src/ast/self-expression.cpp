#include "self-expression.h"
#include "node-visitor.h"
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


void SelfExpression::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".self";
}
void SelfExpression::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSelf);
}
