#ifndef MEMBER_ACCESS_H
#define MEMBER_ACCESS_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class MemberAccess : public ExpressionNode
{
public:
    MemberAccess(ExpressionNode* self, Identifier* field);
public:
    void setSelf(ExpressionNode* self);
    void setField(Identifier* field);
    
    ExpressionNode* getSelf();
    Identifier* getField();
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END



#endif//MEMBER_ACCESS_H