#include "in-out-parameter.h"
#include "identifier.h"
USE_SWIFT_NS;

InOutParameter::InOutParameter()
{
    this->setOperator(L"&");
    type = OperatorType::PrefixUnary;
}
