#include "string-literal.h"
#include "node-visitor.h"
USE_SWIFT_NS


StringLiteral::StringLiteral()
    :Expression(NodeType::StringLiteral)
{
}
void StringLiteral::serialize(std::wostream& out)
{
    out<<value;
}
void StringLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitString);
}
