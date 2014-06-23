#include "initializer.h"
USE_SWIFT_NS


Initializer::Initializer(ExpressionNode* expr)
:ExpressionNode(1)
{
    setExpression(expr);
}

void Initializer::setExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* Initializer::getExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}


void Initializer::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".init";
}