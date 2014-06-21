#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include <vector>

SWIFT_NS_BEGIN

class Node
{
protected:
    Node(int numChildren);
public:
    virtual ~Node();
public:
protected:
    void set(int index, Node* val);
    Node* get(int index);

protected:
    std::vector<Node*> children;
    int numChildren;
};


SWIFT_NS_END


#endif//NODE_H
