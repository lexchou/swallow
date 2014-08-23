#include "UnaryOperator.h"
#include "NodeVisitor.h"
USE_SWIFT_NS;

UnaryOperator::UnaryOperator()
    :Operator(NodeType::UnaryOperator)
{
}
UnaryOperator::~UnaryOperator()
{
}

int UnaryOperator::numChildren()
{
    return 1;
}
NodePtr UnaryOperator::get(int i)
{
    switch(i)
    {
        case 0:
            return operand;
        default:
            return NULL;
    }
}
void UnaryOperator::set(int i, const NodePtr& val)
{
    if(i == 0)
        operand = std::dynamic_pointer_cast<Expression>(val);
}

void UnaryOperator::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitUnaryOperator);
}


