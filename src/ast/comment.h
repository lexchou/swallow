#ifndef COMMENT_NODE_H
#define COMMENT_NODE_H
#include "node.h"
#include <string>
SWIFT_NS_BEGIN

class CommentNode : public Node
{
public:
	CommentNode();
public:
	std::wstring comment;
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//COMMENT_NODE_H