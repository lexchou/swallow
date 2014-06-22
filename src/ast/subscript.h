#ifndef SUBSCRIPT_H
#define SUBSCRIPT_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN
class Identifier;
class Subscript : public ExpressionNode
{
public:
    Subscript(Identifier* self, ExpressionNode* index);
protected:
    Identifier* self;
    ExpressionNode* index;
};

SWIFT_NS_END



#endif//SUBSCRIPT_H