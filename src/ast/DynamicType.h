#ifndef DYNAMIC_TYPE_H
#define DYNAMIC_TYPE_H
#include "Expression.h"

SWIFT_NS_BEGIN

class DynamicType : public Expression
{
public:
    DynamicType();
    ~DynamicType();
public:
    void setExpression(const ExpressionPtr& expr);
    ExpressionPtr getExpression();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr expression;
};

SWIFT_NS_END

#endif//DYNAMIC_TYPE_H
