
#include "generic-parameters.h"
#include "type-identifier.h"
#include "generic-constraint.h"
USE_SWIFT_NS


GenericParameters::GenericParameters()
    :Node(0)
{
}
GenericParameters::~GenericParameters()
{
    std::vector<GenericConstraint*>::iterator iter = constraints.begin();
    for(; iter != constraints.end(); iter++)
    {
        delete *iter;
    }
    constraints.clear();
}
void GenericParameters::serialize(std::wostream& out)
{

}


void GenericParameters::addGenericType(TypeIdentifier* type)
{
    children.push_back(type);
}
int GenericParameters::numGenericTypes()const
{
    return children.size();
}
TypeIdentifier* GenericParameters::getGenericType(int i)
{
    return static_cast<TypeIdentifier*>(get(i));
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
