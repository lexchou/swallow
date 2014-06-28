#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral(bool val)
    :Expression(0), value(val)
{
}

void BooleanLiteral::serialize(std::wostream& out)
{
    out<<value;
}