#include "identifier.h"
#include "node-visitor.h"
USE_SWIFT_NS

Identifier::Identifier()
    :Expression(NodeType::Identifier)
{
}

Identifier::~Identifier()
{

}


void Identifier::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitIdentifier);
}


void Identifier::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}
TypeNodePtr Identifier::getDeclaredType()
{
    return declaredType;
}

GenericArgumentPtr Identifier::getGenericArgument()
{
    return genericArgument;
}
void Identifier::setGenericArgument(const GenericArgumentPtr& val)
{
    genericArgument = val;
}