#include "assignment.h"
#include "node-visitor.h"
USE_SWIFT_NS

Assignment::Assignment()
    :BinaryOperator()
{
    this->op = L"=";
    this->associativity = Associativity::Right;
    this->precedence = 90;
}


void Assignment::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitAssignment);
}