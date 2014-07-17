
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
    SafeDeleteAll(genericTypes);
    SafeDeleteAll(constraints);
}
void GenericParameters::serialize(std::wostream& out)
{

}


void GenericParameters::addGenericType(TypeIdentifier* type)
{
    genericTypes.push_back(type);
}
int GenericParameters::numGenericTypes()const
{
    return genericTypes.size();
}
TypeIdentifier* GenericParameters::getGenericType(int i)
{
    return static_cast<TypeIdentifier*>(genericTypes[i]);
}


void GenericParameters::addConstraint(GenericConstraint* constraint)
{
    constraints.push_back(constraint);
}
int GenericParameters::numConstraints()const
{
    return constraints.size();
}
GenericConstraint* GenericParameters::getConstraint(int i)
{
    return constraints[i];
}
