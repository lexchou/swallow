#include "binary-operator.h"
USE_SWIFT_NS;

BinaryOperator::BinaryOperator(const std::wstring& op)
    :ExpressionNode(2), op(op)
{
}
