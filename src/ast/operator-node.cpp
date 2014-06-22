#include "operator-node.h"
USE_SWIFT_NS

OperatorNode::OperatorNode(int numChildren, OperatorType::T type, Associativity::T associativity, int precedence)
    :ExpressionNode(numChildren), type(type), associativity(associativity), precedence(precedence)
{
    
}