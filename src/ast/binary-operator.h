#ifndef BINARY_OPERATOR_H
#define BINARY_OPERATOR_H
#include "expression-node.h"
#include <string>
SWIFT_NS_BEGIN

class BinaryOperator : public ExpressionNode
{
public:
    BinaryOperator(const std::wstring& op);
public:
    void setLHS(ExpressionNode* node){set(0, node);}
    ExpressionNode* getLHS(){return static_cast<ExpressionNode*>(get(0));}

    void setRHS(ExpressionNode* node){set(1, node);}
    ExpressionNode* getRHS(){return static_cast<ExpressionNode*>(get(1));}
private:
    std::wstring op;
};

SWIFT_NS_END

#endif//BINARY_OPERATOR_H
