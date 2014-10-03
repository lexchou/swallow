#include "GenericParametersDef.h"
USE_SWIFT_NS


GenericParametersDef::GenericParametersDef()
:Node(NodeType::GenericParameterDef)
{
}
GenericParametersDef::~GenericParametersDef()
{

}

void GenericParametersDef::addGenericType(const TypeIdentifierPtr& type)
{
    genericTypes.push_back(type);
}
int GenericParametersDef::numGenericTypes()const
{
    return genericTypes.size();
}
TypeIdentifierPtr GenericParametersDef::getGenericType(int i)
{
    return genericTypes[i];
}


void GenericParametersDef::addConstraint(const GenericConstraintDefPtr& constraint)
{
    constraints.push_back(constraint);
}
int GenericParametersDef::numConstraints()const
{
    return constraints.size();
}
GenericConstraintDefPtr GenericParametersDef::getConstraint(int i)
{
    return constraints[i];
}
const std::vector<GenericConstraintDefPtr> GenericParametersDef::getConstraints() const
{
    return constraints;
}
