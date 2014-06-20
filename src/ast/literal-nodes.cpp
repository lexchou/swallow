#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral(bool val)
    :Node(0), value(val)
{
}
IntegerLiteral::IntegerLiteral(const std::wstring& val)
    :Node(0), value(val)
{
}
FloatLiteral::FloatLiteral(const std::wstring& val)
    :Node(0), value(val)
{
}
StringLiteral::StringLiteral(const std::wstring& val)
    :Node(0), value(val)
{
}
