#include "assignment.h"
#include "node-visitor.h"
USE_SWIFT_NS

Assignment::Assignment(Expression* lhs, Expression* rhs)
    :BinaryOperator(L"=", Associativity::Right, 90)
{
    setLHS(lhs);
    setRHS(rhs);
}


void Assignment::accept(NodeVisitor* visitor)
{
    visitor->visitAssignment(this);
}