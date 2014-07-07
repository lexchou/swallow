#ifndef OPERATOR_H
#define OPERATOR_H
#include "expression.h"
#include "tokenizer/tokens.h"
SWIFT_NS_BEGIN

class Operator : public Expression
{
protected:
    Operator(OperatorType::T type, Associativity::T associativity, int precedence);
public:
    
    OperatorType::T getType() const { return type;}
    Associativity::T getAssociativity() const { return associativity;}
    int getPrecedence() const { return precedence;}
    
public:
    virtual int numChildren() = 0;
    virtual Node* get(int i) = 0;
    virtual void set(int i, Node* val) = 0;
protected:
    OperatorType::T type;
    Associativity::T associativity;
    int precedence;
    
};

SWIFT_NS_END


#endif//OPERATOR_H