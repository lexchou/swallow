#include "OptionalChaining.h"
#include "NodeVisitor.h"
USE_SWIFT_NS



OptionalChaining::OptionalChaining()
    :Expression(NodeType::OptionalChaining)
{
}
OptionalChaining::~OptionalChaining()
{
}

void OptionalChaining::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr OptionalChaining::getExpression()
{
    return expression;
}


void OptionalChaining::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitOptionalChaining);
}
