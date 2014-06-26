#include "node.h"
#include <cstdlib>
#include <cassert>
USE_SWIFT_NS

#ifdef TRACE_NODE
int Node::NodeCount = 0;
#endif


Node::Node(int numChildren)
{
    assert(numChildren >= 0);
    for(int i = 0; i < numChildren; i++)
    {
        children.push_back(NULL);
    }
#ifdef TRACE_NODE
    NodeCount++;
#endif
}
Node::~Node()
{
    Children::iterator iter = children.begin();
    for(; iter != children.end(); iter++)
    {
        Node* n = *iter;
        if(n)
            delete n;
    }
    children.clear();
#ifdef TRACE_NODE
    NodeCount--;
#endif
}

SourceInfo* Node::getSourceInfo()
{
    return &sourceInfo;
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
