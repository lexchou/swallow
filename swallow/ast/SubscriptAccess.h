#ifndef SUBSCRIPT_ACCESS_H
#define SUBSCRIPT_ACCESS_H
#include "Expression.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class SubscriptAccess : public Expression
{
public:
    SubscriptAccess();
    ~SubscriptAccess();
public:
    void setSelf(const ExpressionPtr& self);
    const ExpressionPtr& getSelf();
    
    void setIndex(const ParenthesizedExpressionPtr& index);
    const ParenthesizedExpressionPtr& getIndex();

public:
    virtual void accept(NodeVisitor* visitor);
public:
    ExpressionPtr self;
    ParenthesizedExpressionPtr indices;
};

SWIFT_NS_END



#endif//SUBSCRIPT_ACCESS_H
