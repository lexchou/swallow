
#include "generic-parameters.h"
#include "type-identifier.h"
#include "generic-constraint.h"
USE_SWIFT_NS


GenericParameters::GenericParameters()
:Node(NodeType::GenericParameter)
{
}
GenericParameters::~GenericParameters()
{

}
void GenericParameters::serialize(std::wostream& out)
{

}


void GenericParameters::addGenericType(const TypeIdentifierPtr& type)
{
    genericTypes.push_back(type);
}
int GenericParameters::numGenericTypes()const
{
    return genericTypes.size();
}
TypeIdentifierPtr GenericParameters::getGenericType(int i)
{
    return genericTypes[i];
}


void GenericParameters::addConstraint(const GenericConstraintPtr& constraint)
{
    constraints.push_back(constraint);
}
int GenericParameters::numConstraints()const
{
    return constraints.size();
}
GenericConstraintPtr GenericParameters::getConstraint(int i)
{
    return constraints[i];
}
