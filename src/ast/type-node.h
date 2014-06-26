#ifndef TYPE_NODE_H
#define TYPE_NODE_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class TypeNode : public Statement
{
public:
    TypeNode(int numChildren);
private:
};

SWIFT_NS_END

#endif//TYPE_NODE_H
