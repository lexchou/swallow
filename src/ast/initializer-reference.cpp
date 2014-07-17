#include "initializer-reference.h"
USE_SWIFT_NS


InitializerReference::InitializerReference(Expression* expr)
:Expression(NodeType::InitRef)
{
    setExpression(expr);
}
InitializerReference::~InitializerReference()
{
    SafeDelete(expression);
}

void InitializerReference::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* InitializerReference::getExpression()
{
    return expression;
}


void InitializerReference::serialize(std::wostream& out)
{
    getExpression()->serialize(out);
    out<<L".init";
}