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

CompileConstant::CompileConstant(const std::wstring& name, const std::wstring& value)
    :ExpressionNode(0), name(name), value(value)
{
    
}
const std::wstring& CompileConstant::getName() const
{
    return name;
}
const std::wstring& CompileConstant::getValue() const
{
    return value;
}