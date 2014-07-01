
#include "generic-constraint.h"
#include "type-identifier.h"
USE_SWIFT_NS


GenericConstraint::GenericConstraint()
    :Node(1)
{
}
void GenericConstraint::serialize(std::wostream& out)
{
    
}


void GenericConstraint::setIdentifier(TypeIdentifier* identifier)
{
    set(0, identifier);
}
TypeIdentifier* GenericConstraint::getIdentifier()
{
    return static_cast<TypeIdentifier*>(get(0));
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
    children.push_back(expectedIdentifier);
}
int GenericConstraint::numExpectedTypes()const
{
    return children.size() - 1;
}
TypeIdentifier* GenericConstraint::getExpectedType(int i)
{
    return static_cast<TypeIdentifier*>(get(i + 1));
}