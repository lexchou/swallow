#include "comment.h"
USE_SWIFT_NS

CommentNode::CommentNode()
    :Node(NodeType::Comment)
{

}

void CommentNode::serialize(std::wostream& out)
{
    out<<L"/*"<<comment<<L"*/";
}

