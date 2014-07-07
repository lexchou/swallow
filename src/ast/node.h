#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include <vector>
#include <sstream>
#ifdef TRACE_NODE
#include <list>
#endif//TRACE_NODE

SWIFT_NS_BEGIN


struct SourceInfo
{
    int fileHash;
    int line;
    int column;
    SourceInfo()
    :fileHash(0), line(0), column(0)
    {}
};

class Node
{
protected:
    Node();
public:
    virtual ~Node();
public:
    SourceInfo* getSourceInfo();
    
public:
    virtual void serialize(std::wostream& out) {};
protected:

protected:
    SourceInfo sourceInfo;
    
#ifdef TRACE_NODE
public:
    static std::list<Node*> UnreleasedNodes;
    static int NodeCount;
#endif
};


SWIFT_NS_END


#endif//NODE_H
