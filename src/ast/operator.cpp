#include "operator.h"
USE_SWIFT_NS

Operator::Operator(OperatorType::T type, Associativity::T associativity, int precedence)
    :type(type), associativity(associativity), precedence(precedence)
{
    
}