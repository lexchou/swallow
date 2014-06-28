#include "assignment.h"
USE_SWIFT_NS

Assignment::Assignment(Expression* lhs, Expression* rhs)
    :BinaryOperator(L"=", Associativity::Right, 90)
{
    setLHS(lhs);
    setRHS(rhs);
}