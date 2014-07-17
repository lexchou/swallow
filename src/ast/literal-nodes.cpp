#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral(bool val)
    :Expression(NodeType::BooleanLiteral), value(val)
{
}

void BooleanLiteral::serialize(std::wostream& out)
{
    out<<value;
}