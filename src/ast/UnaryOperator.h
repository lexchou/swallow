#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H
#include "Operator.h"
#include <string>
SWIFT_NS_BEGIN

class UnaryOperator : public Operator
{
public:
    UnaryOperator();
    ~UnaryOperator();
public:
    void setOperand(ExpressionPtr node){operand = node;}
    ExpressionPtr getOperand(){return operand;}
    
    const std::wstring& getOperator() const { return op;}
    void setOperator(const std::wstring& op) { this->op = op;}
public:
    virtual int numChildren();
    virtual NodePtr get(int i);
    virtual void set(int i, const NodePtr& val);
public:
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring op;
    ExpressionPtr operand;
};

SWIFT_NS_END

#endif//UNARY_OPERATOR_H
