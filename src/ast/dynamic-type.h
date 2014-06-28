#ifndef DYNAMIC_TYPE_H
#define DYNAMIC_TYPE_H
#include "expression.h"

SWIFT_NS_BEGIN

class DynamicType : public Expression
{
public:
    DynamicType(Expression* expr);
public:
    void setExpression(Expression* expr);
    Expression* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//DYNAMIC_TYPE_H