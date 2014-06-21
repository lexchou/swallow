#include "node.h"
#include <cstdlib>
#include <cassert>
USE_SWIFT_NS

Node::Node(int numChildren)
    :children(numChildren)
{
    assert(numChildren >= 0);
    if(numChildren)
    {
        for(int i = 0; i < numChildren; i++)
        {
            children.push_back(NULL);
        }
    }
}
Node::~Node()
{
}
void Node::set(int index, Node* val)
{
    if(index >=0 && index < children.size())
        children[index] = val;
}
Node* Node::get(int index)
{
    if(index >=0 && index < children.size())
        return children[index];
    return NULL;
}
int Node::numChildren() const
{
    return children.size();
}
