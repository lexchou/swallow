#include "integer-literal.h"
USE_SWIFT_NS

IntegerLiteral::IntegerLiteral()
    :Expression(NodeType::IntegerLiteral), valueAsString(L"0"), value(0)
{
}
void IntegerLiteral::serialize(std::wostream& out)
{
    out<<value;
}
