#ifndef SELF_EXPRESSION_H
#define SELF_EXPRESSION_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class SelfExpression : public ExpressionNode
{
public:
    SelfExpression(ExpressionNode* expr);
public:
    void setExpression(ExpressionNode* expr);
    ExpressionNode* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//SELF_EXPRESSION_H