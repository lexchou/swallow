#include <ast/node-visitor.h>
#include "dynamic-type.h"
USE_SWIFT_NS

DynamicType::DynamicType(Expression* expr)
    :Expression(NodeType::DynamicType)
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
void DynamicType::accept(NodeVisitor* visitor)
{
    visitor->visitDynamicType(this);
}