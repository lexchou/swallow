#include "operator.h"
USE_SWIFT_NS

Operator::Operator(NodeType::T nodeType)
    :Expression(nodeType), operatorType(OperatorType::_), associativity(Associativity::None), precedence(100)
{
    
}