#include "optional-chaining.h"
USE_SWIFT_NS



OptionalChaining::OptionalChaining(Expression* expr)
:Expression(1)
{
    setExpression(expr);
}

void OptionalChaining::setExpression(Expression* expr)
{
    set(0, expr);
}
Expression* OptionalChaining::getExpression()
{
    return static_cast<Expression*>(get(0));
}


void OptionalChaining::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L"?";
}