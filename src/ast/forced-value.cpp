#include "forced-value.h"
USE_SWIFT_NS


ForcedValue::ForcedValue(Expression* expr)
    :Expression(1)
{
    setExpression(expr);
}

void ForcedValue::setExpression(Expression* expr)
{
    set(0, expr);
}
Expression* ForcedValue::getExpression()
{
    return static_cast<Expression*>(get(0));
}

void ForcedValue::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L"!";
}