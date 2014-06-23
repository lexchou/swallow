#ifndef FORCED_VALUE_H
#define FORCED_VALUE_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class ForcedValue : public ExpressionNode
{
public:
    ForcedValue(ExpressionNode* expr);
public:
    void setExpression(ExpressionNode* expr);
    ExpressionNode* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//FORCED_VALUE_H