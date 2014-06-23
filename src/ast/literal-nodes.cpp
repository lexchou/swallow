#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral(bool val)
    :ExpressionNode(0), value(val)
{
}

void BooleanLiteral::serialize(std::wostream& out)
{
    out<<value;
}