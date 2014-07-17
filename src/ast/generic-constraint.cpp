
#include "generic-constraint.h"
#include "type-identifier.h"
USE_SWIFT_NS


GenericConstraint::GenericConstraint()
:Node(NodeType::GenericConstraint), identifier(NULL)
{
}
GenericConstraint::~GenericConstraint()
{
    SafeDelete(identifier);
    SafeDeleteAll(expectedTypes);
}

void GenericConstraint::serialize(std::wostream& out)
{
    
}


void GenericConstraint::setIdentifier(TypeIdentifier* identifier)
{
    this->identifier = identifier;
}
TypeIdentifier* GenericConstraint::getIdentifier()
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

void GenericConstraint::addExpectedType(TypeIdentifier* expectedIdentifier)
{
    expectedTypes.push_back(expectedIdentifier);
}
int GenericConstraint::numExpectedTypes()const
{
    return expectedTypes.size();
}
TypeIdentifier* GenericConstraint::getExpectedType(int i)
{
    return expectedTypes[i];
}