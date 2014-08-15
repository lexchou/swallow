#include <ast/node-visitor.h>
#include "conditional-operator.h"
USE_SWIFT_NS

ConditionalOperator::ConditionalOperator()
    :Operator(NodeType::ConditionalOperator)
{
    operatorType = OperatorType::Ternary;
    associativity = Associativity::Right;
    precedence = 100;
}

ConditionalOperator::~ConditionalOperator()
{

}

void ConditionalOperator::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitConditionalOperator);
}


int ConditionalOperator::numChildren()
{
    return 3;
}
NodePtr ConditionalOperator::get(int i)
{
    switch(i)
    {
        case 0:
            return condition;
        case 1:
            return trueExpression;
        case 2:
            return falseExpression;
        default:
            return NULL;
    }
}
void ConditionalOperator::set(int i, const NodePtr& val)
{
    switch(i)
    {
        case 0:
            condition = std::dynamic_pointer_cast<Pattern>(val);
            break;
        case 1:
            trueExpression = std::dynamic_pointer_cast<Expression>(val);
            break;
        case 2:
            falseExpression = std::dynamic_pointer_cast<Expression>(val);
            break;
        default:
            break;
    }
}
