#include "integer-literal.h"
USE_SWIFT_NS

IntegerLiteral::IntegerLiteral(const std::wstring& val)
    :Expression(NodeType::IntegerLiteral), value(val)
{
}
void IntegerLiteral::serialize(std::wostream& out)
{
    out<<value;
}
