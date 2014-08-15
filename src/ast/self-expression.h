#ifndef SELF_EXPRESSION_H
#define SELF_EXPRESSION_H
#include "expression.h"

SWIFT_NS_BEGIN

class SelfExpression : public Expression
{
public:
    SelfExpression();
    ~SelfExpression();
public:
    void setExpression(const ExpressionPtr& expr);
    ExpressionPtr getExpression();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr expression;
};

SWIFT_NS_END

#endif//SELF_EXPRESSION_H