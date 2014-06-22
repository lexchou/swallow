#include "unary-operator.h"
#include "identifier.h"
USE_SWIFT_NS;

UnaryOperator::UnaryOperator(const std::wstring& op, OperatorType::T type)
    :OperatorNode(1, type, Associativity::None ,precedence), op(op)
{
}
InOutParameter::InOutParameter(Identifier* identifier)
    :UnaryOperator(L"&", OperatorType::PrefixUnary)
{
    setOperand(identifier);
}
