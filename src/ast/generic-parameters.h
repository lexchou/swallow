#ifndef GENERIC_PARAMETERS_H
#define GENERIC_PARAMETERS_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN
class GenericConstraint;
class TypeIdentifier;

class GenericParameters : public Node
{
public:
    GenericParameters();
    ~GenericParameters();
    
public:
    void addGenericType(const TypeIdentifierPtr& type);
    int numGenericTypes()const;
    TypeIdentifierPtr getGenericType(int i);
    
    
    void addConstraint(const GenericConstraintPtr& constraint);
    int numConstraints()const;
    GenericConstraintPtr getConstraint(int i);
    
private:
    std::vector<GenericConstraintPtr> constraints;
    std::vector<TypeIdentifierPtr> genericTypes;
    
};

SWIFT_NS_END

#endif//GENERIC_PARAMETERS_H
