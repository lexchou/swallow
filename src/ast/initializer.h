#ifndef INITIALIZER_H
#define INITIALIZER_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class Initializer : public ExpressionNode
{
public:
    Initializer(ExpressionNode* expr);
public:
    void setExpression(ExpressionNode* expr);
    ExpressionNode* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//INITIALIZER_H