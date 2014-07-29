#include "comment.h"
USE_SWIFT_NS

CommentNode::CommentNode()
    :Node(NodeType::Comment), comment(comment)
{

}

void CommentNode::serialize(std::wostream& out)
{
    out<<L"/*"<<comment<<L"*/";
}

