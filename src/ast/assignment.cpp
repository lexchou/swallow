#include "assignment.h"
USE_SWIFT_NS

Assignment::Assignment(ExpressionNode* lhs, ExpressionNode* rhs)
    :BinaryOperator(L"=", Associativity::Right, 90)
{
    setLHS(lhs);
    setRHS(rhs);
}