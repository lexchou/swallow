#ifndef NODE_H
#define NODE_H
#include "swift_conf.h"
#include <vector>
#include <sstream>
#ifdef TRACE_NODE
#include <set>
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
    Node(int numChildren);
public:
    virtual ~Node();
public://children management
    Node* get(int index);
    void set(int index, Node* val);
    int numChildren() const;
public:
    SourceInfo* getSourceInfo();
    
public:
    virtual void serialize(std::wostream& out) {};
protected:

protected:
    typedef std::vector<Node*> Children;
    Children children;
    SourceInfo sourceInfo;
    
#ifdef TRACE_NODE
public:
    static std::set<Node*> UnreleasedNodes;
    static int NodeCount;
#endif
};


SWIFT_NS_END


#endif//NODE_H
