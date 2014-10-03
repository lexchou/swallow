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



GenericArgumentDefPtr Identifier::getGenericArgumentDef() const
{
    return genericArgumentDef;
}
void Identifier::setGenericArgumentDef(const GenericArgumentDefPtr& val)
{
    genericArgumentDef = val;
}
