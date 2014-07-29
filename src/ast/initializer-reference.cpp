#include <ast/node-visitor.h>
#include "initializer-reference.h"
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


void InitializerReference::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".init";
}

void InitializerReference::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitInitializerReference);
}
