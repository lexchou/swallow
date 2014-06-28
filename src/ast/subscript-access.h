#ifndef SUBSCRIPT_ACCESS_H
#define SUBSCRIPT_ACCESS_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class SubscriptAccess : public Expression
{
public:
    SubscriptAccess(Expression* self, Expression* index);
public:
    void setSelf(Expression* self);
    Expression* getSelf();
    
    void setIndex(Expression* index);
    Expression* getIndex();

    void setIndex(int idx, Expression* index);
    Expression* getIndex(int idx);
    
    void addIndex(Expression* index);
    int numIndices();
public:
    virtual void serialize(std::wostream& out);
    
};

SWIFT_NS_END



#endif//SUBSCRIPT_ACCESS_H