#ifndef MEMBER_ACCESS_H
#define MEMBER_ACCESS_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class MemberAccess : public Expression
{
public:
    MemberAccess(Expression* self, Identifier* field);
public:
    void setSelf(Expression* self);
    void setField(Identifier* field);
    
    Expression* getSelf();
    Identifier* getField();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END



#endif//MEMBER_ACCESS_H