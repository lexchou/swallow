#include "node.h"
#include "common/auto-release-pool.h"
#include <cstdlib>
#include <cassert>
#include <set>
#include <algorithm>
USE_SWIFT_NS

#ifdef TRACE_NODE
int Node::NodeCount = 0;
std::list<Node*> Node::UnreleasedNodes;

#endif


Node::Node()
:autoReleasePool(NULL)
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
    this->setAutoReleasePool(NULL);
}



void Node::setAutoReleasePool(AutoReleasePool* autoReleasePool)
{
    if(this->autoReleasePool == autoReleasePool)
        return;
    if(this->autoReleasePool)
    {
        AutoReleasePool* old = this->autoReleasePool;
        this->autoReleasePool = NULL;
        old->remove(this);

    }
    this->autoReleasePool = autoReleasePool;
    if(autoReleasePool)
    {
        autoReleasePool->add(this);
    }
}
AutoReleasePool* Node::getAutoReleasePool()
{
    return autoReleasePool;
}

SourceInfo* Node::getSourceInfo()
{
    return &sourceInfo;
}
