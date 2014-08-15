#include "initializer-reference.h"
#include <ast/node-visitor.h>
USE_SWIFT_NS


InitializerReference::InitializerReference()
:Expression(NodeType::InitRef)
{
}
InitializerReference::~InitializerReference()
{
}

void InitializerReference::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr InitializerReference::getExpression()
{
    return expression;
}


void InitializerReference::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitInitializerReference);
}
