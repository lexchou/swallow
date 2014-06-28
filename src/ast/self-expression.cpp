#include "self-expression.h"
USE_SWIFT_NS




SelfExpression::SelfExpression(Expression* expr)
:Expression(1)
{
    setExpression(expr);
}

void SelfExpression::setExpression(Expression* expr)
{
    set(0, expr);
}
Expression* SelfExpression::getExpression()
{
    return static_cast<Expression*>(get(0));
}


void SelfExpression::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".self";
}