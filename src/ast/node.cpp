#include "node.h"
#include <cstdlib>
#include <cassert>
#include <set>
USE_SWIFT_NS

#ifdef TRACE_NODE
int Node::NodeCount = 0;
std::list<Node*> Node::UnreleasedNodes;

#endif


Node::Node()
{
#ifdef TRACE_NODE
    NodeCount++;
    UnreleasedNodes.push_back(this);
#endif
}
Node::~Node()
{
#ifdef TRACE_NODE
    NodeCount--;
    std::list<Node*>::iterator iter = std::find(UnreleasedNodes.begin(), UnreleasedNodes.end(), this);
    if(iter != UnreleasedNodes.end())
        UnreleasedNodes.erase(iter);
#endif
}

SourceInfo* Node::getSourceInfo()
{
    return &sourceInfo;
}
