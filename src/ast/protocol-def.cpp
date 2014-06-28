
#include "protocol-def.h"
USE_SWIFT_NS


ProtocolDef::ProtocolDef()
    :Declaration(0)
{
}
void ProtocolDef::serialize(std::wostream& out)
{
}


void ProtocolDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& ProtocolDef::getName()const
{
    return name;
}
