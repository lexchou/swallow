#ifndef UNARY_OPERATOR_H
#define UNARY_OPERATOR_H
#include "expression-node.h"
#include <string>
SWIFT_NS_BEGIN

class UnaryOperator : public ExpressionNode
{
public:
    UnaryOperator(const std::wstring& op);
public:
    void setOperand(ExpressionNode* node){set(0, node);}
    ExpressionNode* getOperand(){return static_cast<ExpressionNode*>(get(0));}
private:
    std::wstring op;
};


class Identifier;
class InOutParameter : public UnaryOperator
{
public:
    InOutParameter(Identifier* identifier);
};

SWIFT_NS_END

#endif//UNARY_OPERATOR_H
