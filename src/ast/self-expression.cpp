#include "self-expression.h"
USE_SWIFT_NS




SelfExpression::SelfExpression(Expression* expr)
:expression(expr)
{
}
SelfExpression::~SelfExpression()
{
    SafeDelete(expression);
}

void SelfExpression::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* SelfExpression::getExpression()
{
    return expression;
}


void SelfExpression::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".self";
}