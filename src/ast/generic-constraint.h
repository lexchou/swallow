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
    ~GenericConstraint();
public:
    void setIdentifier(const TypeIdentifierPtr& identifier);
    TypeIdentifierPtr getIdentifier();

    void setConstraintType(ConstraintType constraint);
    ConstraintType getConstraintType()const;
    
    void addExpectedType(const TypeIdentifierPtr& expectedIdentifier);
    int numExpectedTypes()const;
    TypeIdentifierPtr getExpectedType(int i);
    
public:
    virtual void serialize(std::wostream& out);
private:
    ConstraintType type;
    TypeIdentifierPtr identifier;
    std::vector<TypeIdentifierPtr> expectedTypes;
};

SWIFT_NS_END

#endif//GENERIC_CONSTRAINT_H
