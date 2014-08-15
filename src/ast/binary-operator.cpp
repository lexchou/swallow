#include "binary-operator.h"
#include "node-visitor.h"
USE_SWIFT_NS;

BinaryOperator::BinaryOperator()
    :Operator(NodeType::BinaryOperator)
{
    operatorType = OperatorType::InfixBinary;
}
BinaryOperator::~BinaryOperator()
{
}

void BinaryOperator::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitBinaryOperator);
}


int BinaryOperator::numChildren()
{
    return 2;
}
NodePtr BinaryOperator::get(int i)
{
    switch(i)
    {
        case 0:
            return lhs;
        case 1:
            return rhs;
        default:
            return NULL;
    }
}
void BinaryOperator::set(int i, const NodePtr& val)
{
    switch(i)
    {
        case 0:
            lhs = std::dynamic_pointer_cast<Pattern>(val);
            break;
        case 1:
            rhs = std::dynamic_pointer_cast<Pattern>(val);
            break;
        default:
            break;
    }
}