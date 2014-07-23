#ifndef INITIALIZER_REFERENCE_H
#define INITIALIZER_REFERENCE_H
#include "expression.h"

SWIFT_NS_BEGIN

class InitializerReference : public Expression
{
public:
    InitializerReference(Expression* expr);
    ~InitializerReference();
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

#endif//INITIALIZER_REFERENCE_H