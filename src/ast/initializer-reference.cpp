#include "initializer-reference.h"
USE_SWIFT_NS


InitializerReference::InitializerReference(Expression* expr)
:Expression(1)
{
    setExpression(expr);
}

void InitializerReference::setExpression(Expression* expr)
{
    set(0, expr);
}
Expression* InitializerReference::getExpression()
{
    return static_cast<Expression*>(get(0));
}


void InitializerReference::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".init";
}