#include "string-literal.h"
#include "node-visitor.h"
USE_SWIFT_NS


StringLiteral::StringLiteral()
    :Expression(NodeType::StringLiteral)
{
}

void StringLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitString);
}
