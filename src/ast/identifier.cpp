#include "identifier.h"
#include "type-node.h"
#include "generic-argument.h"
#include "node-visitor.h"
USE_SWIFT_NS

Identifier::Identifier()
    :Expression(NodeType::Identifier)
{
    flags = 0;
}

Identifier::~Identifier()
{

}

void Identifier::serialize(std::wostream& out)
{
    out<<identifier;
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