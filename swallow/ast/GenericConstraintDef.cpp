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

void GenericConstraintDef::addExpectedType(const TypeIdentifierPtr& expectedIdentifier)
{
    expectedTypes.push_back(expectedIdentifier);
}
int GenericConstraintDef::numExpectedTypes()const
{
    return expectedTypes.size();
}
TypeIdentifierPtr GenericConstraintDef::getExpectedType(int i)
{
    return expectedTypes[i];
}
