#include "comment.h"
USE_SWIFT_NS

CommentNode::CommentNode(const std::wstring& comment)
    :Node(NodeType::Comment), comment(comment)
{

}

void CommentNode::serialize(std::wostream& out)
{
    out<<L"/*"<<comment<<L"*/";
}

