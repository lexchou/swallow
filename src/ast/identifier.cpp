#include "identifier.h"
#include "type-node.h"
#include "generic-argument.h"
USE_SWIFT_NS

Identifier::Identifier(const std::wstring& identifier)
    :identifier(identifier), type(NULL), genericArgument(NULL)
{
}

Identifier::~Identifier()
{
    SafeDelete(type);
    SafeDelete(genericArgument);
}

void Identifier::serialize(std::wostream& out)
{
    out<<identifier;
}


void Identifier::setType(TypeNode* type)
{
    this->type = type;
}
TypeNode* Identifier::getType()
{
    return type;
}

GenericArgument* Identifier::getGenericArgument()
{
    return genericArgument;
}
void Identifier::setGenericArgument(GenericArgument* val)
{
    genericArgument = val;
}