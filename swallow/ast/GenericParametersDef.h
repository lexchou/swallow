#ifndef GENERIC_PARAMETERS_H
#define GENERIC_PARAMETERS_H
#include "Node.h"
#include <string>

SWIFT_NS_BEGIN
class GenericConstraintDef;
class TypeIdentifier;

class GenericParametersDef : public Node
{
public:
    GenericParametersDef();
    ~GenericParametersDef();
    
public:
    void addGenericType(const TypeIdentifierPtr& type);
    int numGenericTypes()const;
    TypeIdentifierPtr getGenericType(int i);
    
    
    void addConstraint(const GenericConstraintDefPtr& constraint);
    int numConstraints()const;
    GenericConstraintDefPtr getConstraint(int i);

    std::vector<TypeIdentifierPtr>::iterator begin() { return genericTypes.begin();}
    std::vector<TypeIdentifierPtr>::iterator end() { return genericTypes.end();}

private:
    std::vector<GenericConstraintDefPtr> constraints;
    std::vector<TypeIdentifierPtr> genericTypes;

};

SWIFT_NS_END

#endif//GENERIC_PARAMETERS_H
