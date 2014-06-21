#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral(bool val)
    :ExpressionNode(0), value(val)
{
}
IntegerLiteral::IntegerLiteral(const std::wstring& val)
    :ExpressionNode(0), value(val)
{
}
FloatLiteral::FloatLiteral(const std::wstring& val)
    :ExpressionNode(0), value(val)
{
}
StringLiteral::StringLiteral(const std::wstring& val)
    :ExpressionNode(0), value(val)
{
}
