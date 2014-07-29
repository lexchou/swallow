#include "float-literal.h"
USE_SWIFT_NS

FloatLiteral::FloatLiteral()
    :Expression(NodeType::FloatLiteral), valueAsString(L"0.0"), value(0)
{
}
void FloatLiteral::serialize(std::wostream& out)
{
    out<<value;
}