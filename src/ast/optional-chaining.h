#ifndef OPTIONAL_CHAINING_H
#define OPTIONAL_CHAINING_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class OptionalChaining : public ExpressionNode
{
public:
    OptionalChaining(ExpressionNode* expr);
public:
    void setExpression(ExpressionNode* expr);
    ExpressionNode* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//OPTIONAL_CHAINING_H