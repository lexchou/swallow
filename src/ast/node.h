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
    Node* get(int index);
    int numChildren() const;
protected:
    void set(int index, Node* val);

protected:
    std::vector<Node*> children;
};


SWIFT_NS_END


#endif//NODE_H
