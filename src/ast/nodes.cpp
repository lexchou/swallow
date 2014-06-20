#include "node.h"
#include <cstdlib>
#include <cassert>
using namespace Swift;
Node::Node(int numChildren)
    :children(NULL), numChildren(numChildren)
{
    assert(numChildren >= 0);
    if(numChildren)
    {
        children = new Node*[numChildren];
        for(int i = 0; i < numChildren; i++)
        {
            children[i] = NULL;
        }
    }
}
Node::~Node()
{
    if(children)
        delete[] children;
    children = NULL;
}
void Node::set(int index, Node* val)
{
    if(index >=0 && index < numChildren)
        children[index] = val;
}
Node* Node::get(int index)
{
    if(index >=0 && index < numChildren)
        return children[index];
    return NULL;
}

