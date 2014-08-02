#ifndef OPERATOR_H
#define OPERATOR_H
#include "expression.h"
#include "tokenizer/tokens.h"
SWIFT_NS_BEGIN

class Operator : public Expression
{
protected:
    Operator(NodeType::T);
public:
    
    OperatorType::T getOperatorType() const { return operatorType;}
    void setOperatorType(OperatorType::T type){this->operatorType = type;}
    Associativity::T getAssociativity() const { return associativity;}
    void setAssociativity(Associativity::T a) { associativity = a;}
    int getPrecedence() const { return precedence;}
    void setPrecedence(int v) { precedence = v;}
    
public:
    virtual int numChildren() = 0;
    virtual NodePtr get(int i) = 0;
    virtual void set(int i, const NodePtr& val) = 0;
protected:
    OperatorType::T operatorType;
    Associativity::T associativity;
    int precedence;
    
};

SWIFT_NS_END


#endif//OPERATOR_H