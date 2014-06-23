#include "dynamic-type.h"
USE_SWIFT_NS

DynamicType::DynamicType(ExpressionNode* expr)
:ExpressionNode(1)
{
    setExpression(expr);
}

void DynamicType::setExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* DynamicType::getExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}

void DynamicType::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".dynamicType";
}
