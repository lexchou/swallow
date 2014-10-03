#include "GenericConstraintDef.h"
#include "TypeIdentifier.h"
USE_SWIFT_NS


GenericConstraintDef::GenericConstraintDef()
:Node(NodeType::GenericConstraintDef), identifier(NULL)
{
}
GenericConstraintDef::~GenericConstraintDef()
{

}


void GenericConstraintDef::setIdentifier(const TypeIdentifierPtr& identifier)
{
    this->identifier = identifier;
}
TypeIdentifierPtr GenericConstraintDef::getIdentifier()
{
    return identifier;
}

void GenericConstraintDef::setConstraintType(ConstraintType constraint)
{
    this->type = constraint;
}
GenericConstraintDef::ConstraintType GenericConstraintDef::getConstraintType()const
{
    return type;
}

void GenericConstraintDef::setExpectedType(const TypeIdentifierPtr& expectedIdentifier)
{
    this->expectedType = expectedIdentifier;
}

TypeIdentifierPtr GenericConstraintDef::getExpectedType()const
{
    return expectedType;
}
