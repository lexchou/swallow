#ifndef TYPE_NODE_H
#define TYPE_NODE_H
#include "node.h"

SWIFT_NS_BEGIN

class TypeNode : public Node
{
protected:
    TypeNode(int numChildren);
    
};

SWIFT_NS_END


#endif//TYPE_NODE_H