#ifndef GENERIC_CONSTRAINT_H
#define GENERIC_CONSTRAINT_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN
class TypeIdentifier;

class GenericConstraint : public Node
{
public:
    enum ConstraintType
    {
        EqualsTo,
        DerivedFrom
    };
public:
    GenericConstraint();
public:
    void setIdentifier(TypeIdentifier* identifier);
    TypeIdentifier* getIdentifier();

    void setConstraintType(ConstraintType constraint);
    ConstraintType getConstraintType()const;
    
    void addExpectedType(TypeIdentifier* expectedIdentifier);
    int numExpectedTypes()const;
    TypeIdentifier* getExpectedType(int i);
    
public:
    virtual void serialize(std::wostream& out);
private:
    ConstraintType type;
};

SWIFT_NS_END

#endif//GENERIC_CONSTRAINT_H
