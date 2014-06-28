
#include "function-def.h"
USE_SWIFT_NS


FunctionDef::FunctionDef()
    :Declaration(0)
{
}
void FunctionDef::serialize(std::wostream& out)
{
}

void FunctionDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& FunctionDef::getName()const
{
    return name;
}
