#include "identifier.h"
#include "type-node.h"
USE_SWIFT_NS

Identifier::Identifier(const std::wstring& identifier)
    :Expression(1), identifier(identifier)
{
}

void Identifier::serialize(std::wostream& out)
{
    out<<identifier;
}


void Identifier::setType(TypeNode* type)
{
    set(0, type);
}
TypeNode* Identifier::getType()
{
    return static_cast<TypeNode*>(get(0));
}