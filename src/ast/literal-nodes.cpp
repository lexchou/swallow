#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral()
    :Expression(NodeType::BooleanLiteral)
{
}

void BooleanLiteral::serialize(std::wostream& out)
{
    out<<value;
}