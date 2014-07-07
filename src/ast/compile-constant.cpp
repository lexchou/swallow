#include "compile-constant.h"
USE_SWIFT_NS

CompileConstant::CompileConstant(const std::wstring& name, const std::wstring& value)
:name(name), value(value)
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