#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H
#include "operator-node.h"
#include <string>
SWIFT_NS_BEGIN

class UnaryOperator : public OperatorNode
{
public:
    UnaryOperator(const std::wstring& op, OperatorType::T type);
public:
    void setOperand(ExpressionNode* node){set(0, node);}
    ExpressionNode* getOperand(){return static_cast<ExpressionNode*>(get(0));}
    
    const std::wstring& getOperator() const { return op;}
    void setOperator(const std::wstring& op) { this->op = op;}
public:
    virtual void serialize(std::wostream& out);
private:
    std::wstring op;
};

SWIFT_NS_END

#endif//UNARY_OPERATOR_H
