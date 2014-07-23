#ifndef FORCED_VALUE_H
#define FORCED_VALUE_H
#include "expression.h"

SWIFT_NS_BEGIN

class ForcedValue : public Expression
{
public:
    ForcedValue(Expression* expr);
    ~ForcedValue();
public:
    void setExpression(Expression* expr);
    Expression* getExpression();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    Expression* expression;
};

SWIFT_NS_END

#endif//FORCED_VALUE_H