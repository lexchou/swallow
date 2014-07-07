#ifndef SELF_EXPRESSION_H
#define SELF_EXPRESSION_H
#include "expression.h"

SWIFT_NS_BEGIN

class SelfExpression : public Expression
{
public:
    SelfExpression(Expression* expr);
    ~SelfExpression();
public:
    void setExpression(Expression* expr);
    Expression* getExpression();
public:
    virtual void serialize(std::wostream& out);
private:
    Expression* expression;
};

SWIFT_NS_END

#endif//SELF_EXPRESSION_H