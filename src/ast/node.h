#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include "swift_types.h"
#include <vector>
#include <sstream>
#ifdef TRACE_NODE
#include <list>
#endif//TRACE_NODE

SWIFT_NS_BEGIN

class AutoReleasePool;

class Node
{
protected:
    Node();
public:
    virtual ~Node();
public:
    SourceInfo* getSourceInfo();
    void setAutoReleasePool(AutoReleasePool* autoReleasePool);
    AutoReleasePool* getAutoReleasePool();
public:
    virtual void serialize(std::wostream& out) {};
protected:

protected:
    SourceInfo sourceInfo;
    AutoReleasePool* autoReleasePool;
    
#ifdef TRACE_NODE
public:
    static std::list<Node*> UnreleasedNodes;
    static int NodeCount;
#endif
};


SWIFT_NS_END


#endif//NODE_H
