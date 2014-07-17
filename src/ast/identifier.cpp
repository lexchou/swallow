#include "identifier.h"
#include "type-node.h"
#include "generic-argument.h"
USE_SWIFT_NS

Identifier::Identifier(const std::wstring& identifier)
    :Expression(NodeType::Identifier), identifier(identifier), declaredType(NULL), genericArgument(NULL)
{
}

Identifier::~Identifier()
{
    SafeDelete(declaredType);
    SafeDelete(genericArgument);
}

void Identifier::serialize(std::wostream& out)
{
    out<<identifier;
}


void Identifier::setDeclaredType(TypeNode* type)
{
    this->declaredType = type;
}
TypeNode* Identifier::getDeclaredType()
{
    return declaredType;
}

GenericArgument* Identifier::getGenericArgument()
{
    return genericArgument;
}
void Identifier::setGenericArgument(GenericArgument* val)
{
    genericArgument = val;
}