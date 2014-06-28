
#include "enum-def.h"
USE_SWIFT_NS


EnumDef::EnumDef()
    :Declaration(0)
{
}
void EnumDef::serialize(std::wostream& out)
{
}


void EnumDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& EnumDef::getName()const
{
    return name;
}
