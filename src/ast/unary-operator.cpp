#include "unary-operator.h"
USE_SWIFT_NS;

UnaryOperator::UnaryOperator(const std::wstring& op)
    :Node(1), op(op)
{
}
