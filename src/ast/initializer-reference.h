#ifndef INITIALIZER_REFERENCE_H
#define INITIALIZER_REFERENCE_H
#include "expression.h"

SWIFT_NS_BEGIN

class InitializerReference : public Expression
{
public:
    InitializerReference();
    ~InitializerReference();
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

#endif//INITIALIZER_REFERENCE_H