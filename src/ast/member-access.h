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
    ~MemberAccess();
public:
    void setSelf(Expression* self);
    void setField(Identifier* field);
    
    Expression* getSelf();
    Identifier* getField();
public:
    virtual void serialize(std::wostream& out);
public:
    Expression* self;
    Identifier* field;
    
};

SWIFT_NS_END



#endif//MEMBER_ACCESS_H