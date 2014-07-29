#include <ast/node-visitor.h>
#include "dynamic-type.h"
USE_SWIFT_NS

DynamicType::DynamicType()
    :Expression(NodeType::DynamicType)
{
}
DynamicType::~DynamicType()
{
}

void DynamicType::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr DynamicType::getExpression()
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
    accept2(visitor, &NodeVisitor::visitDynamicType);
}