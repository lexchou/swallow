#ifndef MEMBER_ACCESS_H
#define MEMBER_ACCESS_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class MemberAccess : public ExpressionNode
{
public:
    MemberAccess(Identifier* self, Identifier* field);
protected:
    Identifier* self;
    Identifier* field;
};

SWIFT_NS_END



#endif//MEMBER_ACCESS_H