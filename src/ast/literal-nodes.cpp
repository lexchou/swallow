#include "literal-nodes.h"
USE_SWIFT_NS


BooleanLiteral::BooleanLiteral(bool val)
    :ExpressionNode(0), value(val)
{
}

void BooleanLiteral::serialize(std::wostream& out)
{
    out<<value;
}
IntegerLiteral::IntegerLiteral(const std::wstring& val)
    :ExpressionNode(0), value(val)
{
}
void IntegerLiteral::serialize(std::wostream& out)
{
    out<<value;
}
FloatLiteral::FloatLiteral(const std::wstring& val)
    :ExpressionNode(0), value(val)
{
}
void FloatLiteral::serialize(std::wostream& out)
{
    out<<value;
}

StringLiteral::StringLiteral(const std::wstring& val)
    :ExpressionNode(0), value(val)
{
}
void StringLiteral::serialize(std::wostream& out)
{
    out<<value;
}

CompileConstant::CompileConstant(const std::wstring& name, const std::wstring& value)
    :ExpressionNode(0), name(name), value(value)
{
    
}
void CompileConstant::serialize(std::wostream& out)
{
    out<<name;
}

const std::wstring& CompileConstant::getName() const
{
    return name;
}
const std::wstring& CompileConstant::getValue() const
{
    return value;
}