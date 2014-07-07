#include "dynamic-type.h"
USE_SWIFT_NS

DynamicType::DynamicType(Expression* expr)
{
    setExpression(expr);
}
DynamicType::~DynamicType()
{
    SafeDelete(expression);
}

void DynamicType::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* DynamicType::getExpression()
{
    return expression;
}

void DynamicType::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".dynamicType";
}
