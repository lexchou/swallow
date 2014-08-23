#ifndef COMMENT_NODE_H
#define COMMENT_NODE_H
#include "Node.h"
#include <string>
SWIFT_NS_BEGIN

class CommentNode : public Node
{
public:
	CommentNode();
public:
	std::wstring comment;
public:
};

SWIFT_NS_END

#endif//COMMENT_NODE_H
