#ifndef SUBSCRIPT_ACCESS_H
#define SUBSCRIPT_ACCESS_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class SubscriptAccess : public Expression
{
public:
    SubscriptAccess();
    ~SubscriptAccess();
public:
    void setSelf(ExpressionPtr self);
    ExpressionPtr getSelf();
    
    void setIndex(ExpressionPtr index);
    ExpressionPtr getIndex();

    void setIndex(int idx, const ExpressionPtr& index);
    ExpressionPtr getIndex(int idx);
    
    void addIndex(ExpressionPtr index);
    int numIndices();

    std::vector<ExpressionPtr>::iterator begin(){return indices.begin();}
    std::vector<ExpressionPtr>::iterator end(){return indices.end();}

public:
    virtual void accept(NodeVisitor* visitor);
public:
    ExpressionPtr self;
    std::vector<ExpressionPtr> indices;
    
};

SWIFT_NS_END



#endif//SUBSCRIPT_ACCESS_H