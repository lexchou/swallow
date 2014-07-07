#include "float-literal.h"
USE_SWIFT_NS

FloatLiteral::FloatLiteral(const std::wstring& val)
:value(val)
{
}
void FloatLiteral::serialize(std::wostream& out)
{
    out<<value;
}