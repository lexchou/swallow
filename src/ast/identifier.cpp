#include "identifier.h"
#include "type-node.h"
USE_SWIFT_NS

Identifier::Identifier(const std::wstring& identifier)
    :identifier(identifier), type(NULL)
{
}

Identifier::~Identifier()
{
    SafeDelete(type);
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