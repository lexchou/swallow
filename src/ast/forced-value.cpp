#include "forced-value.h"
USE_SWIFT_NS


ForcedValue::ForcedValue(ExpressionNode* expr)
    :ExpressionNode(1)
{
    setExpression(expr);
}

void ForcedValue::setExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* ForcedValue::getExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}

void ForcedValue::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L"!";
}