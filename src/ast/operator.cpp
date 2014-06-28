#include "operator.h"
USE_SWIFT_NS

Operator::Operator(int numChildren, OperatorType::T type, Associativity::T associativity, int precedence)
    :Expression(numChildren), type(type), associativity(associativity), precedence(precedence)
{
    
}