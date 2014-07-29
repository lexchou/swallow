#ifndef FORCED_VALUE_H
#define FORCED_VALUE_H
#include "expression.h"

SWIFT_NS_BEGIN

class ForcedValue : public Expression
{
public:
    ForcedValue();
    ~ForcedValue();
public:
    void setExpression(const ExpressionPtr& expr);
    ExpressionPtr getExpression();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr expression;
};

SWIFT_NS_END

#endif//FORCED_VALUE_H