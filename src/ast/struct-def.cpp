
#include "struct-def.h"
USE_SWIFT_NS


StructDef::StructDef()
    :Declaration(0)
{
}
void StructDef::serialize(std::wostream& out)
{
}


void StructDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& StructDef::getName()const
{
    return name;
}
