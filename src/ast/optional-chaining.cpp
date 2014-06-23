#include "optional-chaining.h"
USE_SWIFT_NS



OptionalChaining::OptionalChaining(ExpressionNode* expr)
:ExpressionNode(1)
{
    setExpression(expr);
}

void OptionalChaining::setExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* OptionalChaining::getExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}


void OptionalChaining::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L"?";
}