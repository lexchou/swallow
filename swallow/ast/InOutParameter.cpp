#include "InOutParameter.h"
#include "Identifier.h"
USE_SWIFT_NS;

InOutParameter::InOutParameter()
{
    this->setOperator(L"&");
    operatorType = OperatorType::PrefixUnary;
}
