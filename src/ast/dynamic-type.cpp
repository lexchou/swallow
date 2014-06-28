#include "dynamic-type.h"
USE_SWIFT_NS

DynamicType::DynamicType(Expression* expr)
:Expression(1)
{
    setExpression(expr);
}

void DynamicType::setExpression(Expression* expr)
{
    set(0, expr);
}
Expression* DynamicType::getExpression()
{
    return static_cast<Expression*>(get(0));
}

void DynamicType::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".dynamicType";
}
