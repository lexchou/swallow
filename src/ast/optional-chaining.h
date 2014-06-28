#ifndef OPTIONAL_CHAINING_H
#define OPTIONAL_CHAINING_H
#include "expression.h"

SWIFT_NS_BEGIN

class OptionalChaining : public Expression
{
public:
    OptionalChaining(Expression* expr);
public:
    void setExpression(Expression* expr);
    Expression* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//OPTIONAL_CHAINING_H