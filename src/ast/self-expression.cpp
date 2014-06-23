#include "self-expression.h"
USE_SWIFT_NS




SelfExpression::SelfExpression(ExpressionNode* expr)
:ExpressionNode(1)
{
    setExpression(expr);
}

void SelfExpression::setExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* SelfExpression::getExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}


void SelfExpression::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".self";
}