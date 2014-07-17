#include "forced-value.h"
USE_SWIFT_NS


ForcedValue::ForcedValue(Expression* expr)
    :Expression(NodeType::ForcedValue)
{
    setExpression(expr);
}
ForcedValue::~ForcedValue()
{
    SafeDelete(expression);
}
void ForcedValue::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* ForcedValue::getExpression()
{
    return expression;
}

void ForcedValue::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L"!";
}