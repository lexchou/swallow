#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include <vector>
#include <sstream>

SWIFT_NS_BEGIN

class Node
{
protected:
    Node(int numChildren);
public:
    virtual ~Node();
public:
    Node* get(int index);
    void set(int index, Node* val);
    int numChildren() const;
public:
    virtual void serialize(std::wostream& out) {};
protected:

protected:
    std::vector<Node*> children;
};


SWIFT_NS_END


#endif//NODE_H
