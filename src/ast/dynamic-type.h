#ifndef DYNAMIC_TYPE_H
#define DYNAMIC_TYPE_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class DynamicType : public ExpressionNode
{
public:
    DynamicType(ExpressionNode* expr);
public:
    void setExpression(ExpressionNode* expr);
    ExpressionNode* getExpression();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//DYNAMIC_TYPE_H