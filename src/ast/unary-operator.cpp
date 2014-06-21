#include "unary-operator.h"
#include "identifier.h"
USE_SWIFT_NS;

UnaryOperator::UnaryOperator(const std::wstring& op)
    :ExpressionNode(1), op(op)
{
}
InOutParameter::InOutParameter(Identifier* identifier)
    :UnaryOperator(L"&")
{
    setOperand(identifier);
}
