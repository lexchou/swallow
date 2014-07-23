#include <ast/node-visitor.h>
#include "optional-chaining.h"
USE_SWIFT_NS



OptionalChaining::OptionalChaining(Expression* expr)
    :Expression(NodeType::OptionalChaining), expression(expr)
{
}
OptionalChaining::~OptionalChaining()
{
    SafeDelete(expression);
}

void OptionalChaining::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* OptionalChaining::getExpression()
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
    visitor->visitOptionalChaining(this);
}