
#include "class-def.h"
USE_SWIFT_NS


ClassDef::ClassDef()
    :Declaration(0)
{
}
void ClassDef::serialize(std::wostream& out)
{
}


void ClassDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& ClassDef::getName()const
{
    return name;
}
