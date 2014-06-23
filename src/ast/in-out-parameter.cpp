#include "in-out-parameter.h"
#include "identifier.h"
USE_SWIFT_NS;

InOutParameter::InOutParameter(Identifier* identifier)
:UnaryOperator(L"&", OperatorType::PrefixUnary)
{
    setOperand(identifier);
}
