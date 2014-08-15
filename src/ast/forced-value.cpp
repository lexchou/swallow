#include <ast/node-visitor.h>
#include "forced-value.h"
USE_SWIFT_NS


ForcedValue::ForcedValue()
    :Expression(NodeType::ForcedValue)
{
}
ForcedValue::~ForcedValue()
{
}
void ForcedValue::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr ForcedValue::getExpression()
{
    return expression;
}


void ForcedValue::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitForcedValue);
}