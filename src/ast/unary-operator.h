#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H
#include "operator.h"
#include <string>
SWIFT_NS_BEGIN

class UnaryOperator : public Operator
{
public:
    UnaryOperator(const std::wstring& op, OperatorType::T type);
    ~UnaryOperator();
public:
    void setOperand(Expression* node){operand = node;}
    Expression* getOperand(){return operand;}
    
    const std::wstring& getOperator() const { return op;}
    void setOperator(const std::wstring& op) { this->op = op;}
public:
    virtual int numChildren();
    virtual Node* get(int i);
    virtual void set(int i, Node* val);
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    std::wstring op;
    Expression* operand;
};

SWIFT_NS_END

#endif//UNARY_OPERATOR_H
