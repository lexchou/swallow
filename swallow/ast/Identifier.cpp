#include "Identifier.h"
#include "NodeVisitor.h"
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



GenericArgumentPtr Identifier::getGenericArgument()
{
    return genericArgument;
}
void Identifier::setGenericArgument(const GenericArgumentPtr& val)
{
    genericArgument = val;
}
