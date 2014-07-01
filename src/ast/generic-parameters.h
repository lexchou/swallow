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
    virtual void serialize(std::wostream& out);
public:
    void addGenericType(TypeIdentifier* type);
    int numGenericTypes()const;
    TypeIdentifier* getGenericType(int i);
    
    
    void addConstraint(GenericConstraint* constraint);
    int numConstraints()const;
    GenericConstraint* getConstraint(int i);
    
private:
    std::vector<GenericConstraint*> constraints;
};

SWIFT_NS_END

#endif//GENERIC_PARAMETERS_H
