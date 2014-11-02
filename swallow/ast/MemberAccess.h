#ifndef MEMBER_ACCESS_H
#define MEMBER_ACCESS_H
#include "Expression.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class MemberAccess : public Expression
{
public:
    MemberAccess();
    ~MemberAccess();
public:
    void setSelf(const ExpressionPtr& self);
    void setField(const IdentifierPtr& field);
    
    ExpressionPtr getSelf();
    IdentifierPtr getField();

    int getIndex() const;
    void setIndex(int index);
public:
    virtual void accept(NodeVisitor* visitor);
public:
    ExpressionPtr self;
    IdentifierPtr field;
    int index;
};

SWIFT_NS_END



#endif//MEMBER_ACCESS_H
