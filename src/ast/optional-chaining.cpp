#include <ast/node-visitor.h>
#include "optional-chaining.h"
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


void OptionalChaining::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L"?";
}
void OptionalChaining::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitOptionalChaining);
}