
#include "generic-constraint.h"
#include "type-identifier.h"
USE_SWIFT_NS


GenericConstraint::GenericConstraint()
:Node(NodeType::GenericConstraint), identifier(NULL)
{
}
GenericConstraint::~GenericConstraint()
{

}


void GenericConstraint::setIdentifier(const TypeIdentifierPtr& identifier)
{
    this->identifier = identifier;
}
TypeIdentifierPtr GenericConstraint::getIdentifier()
{
    return identifier;
}

void GenericConstraint::setConstraintType(ConstraintType constraint)
{
    this->type = constraint;
}
GenericConstraint::ConstraintType GenericConstraint::getConstraintType()const
{
    return type;
}

void GenericConstraint::addExpectedType(const TypeIdentifierPtr& expectedIdentifier)
{
    expectedTypes.push_back(expectedIdentifier);
}
int GenericConstraint::numExpectedTypes()const
{
    return expectedTypes.size();
}
TypeIdentifierPtr GenericConstraint::getExpectedType(int i)
{
    return expectedTypes[i];
}