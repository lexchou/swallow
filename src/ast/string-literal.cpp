#include "string-literal.h"
USE_SWIFT_NS


StringLiteral::StringLiteral(const std::wstring& val)
:Expression(0), value(val)
{
}
void StringLiteral::serialize(std::wostream& out)
{
    out<<value;
}