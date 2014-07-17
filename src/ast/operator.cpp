#include "operator.h"
USE_SWIFT_NS

Operator::Operator(NodeType::T nodeType, OperatorType::T type, Associativity::T associativity, int precedence)
    :Expression(nodeType), type(type), associativity(associativity), precedence(precedence)
{
    
}