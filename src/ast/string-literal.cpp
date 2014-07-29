#include "string-literal.h"
USE_SWIFT_NS


StringLiteral::StringLiteral()
    :Expression(NodeType::StringLiteral)
{
}
void StringLiteral::serialize(std::wostream& out)
{
    out<<value;
}