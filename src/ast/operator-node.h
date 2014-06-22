#ifndef OPERATOR_NODE_H
#define OPERATOR_NODE_H
#include "expression-node.h"
#include "tokenizer/tokens.h"
SWIFT_NS_BEGIN

class OperatorNode : public ExpressionNode
{
protected:
    OperatorNode(int numChildren, OperatorType::T type, Associativity::T associativity, int precedence);
public:
    
    OperatorType::T getType() const { return type;}
    Associativity::T getAssociativity() const { return associativity;}
    int getPrecedence() const { return precedence;}
    
protected:
    OperatorType::T type;
    Associativity::T associativity;
    int precedence;
    
};

SWIFT_NS_END


#endif//OPERATOR_NODE_H