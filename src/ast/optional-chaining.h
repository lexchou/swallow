#ifndef OPTIONAL_CHAINING_H
#define OPTIONAL_CHAINING_H
#include "expression.h"

SWIFT_NS_BEGIN

class OptionalChaining : public Expression
{
public:
    OptionalChaining();
    ~OptionalChaining();
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

#endif//OPTIONAL_CHAINING_H