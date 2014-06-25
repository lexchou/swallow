#include "integer-literal.h"
USE_SWIFT_NS

IntegerLiteral::IntegerLiteral(const std::wstring& val)
:ExpressionNode(0), value(val)
{
}
void IntegerLiteral::serialize(std::wostream& out)
{
    out<<value;
}