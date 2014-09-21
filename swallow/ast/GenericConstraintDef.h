#ifndef GENERIC_CONSTRAINT_H
#define GENERIC_CONSTRAINT_H
#include "Node.h"
#include <string>

SWIFT_NS_BEGIN
class TypeIdentifier;

class GenericConstraintDef : public Node
{
public:
    enum ConstraintType
    {
        EqualsTo,
        DerivedFrom
    };
public:
    GenericConstraintDef();
    ~GenericConstraintDef();
public:
    void setIdentifier(const TypeIdentifierPtr& identifier);
    TypeIdentifierPtr getIdentifier();

    void setConstraintType(ConstraintType constraint);
    ConstraintType getConstraintType()const;
    
    void addExpectedType(const TypeIdentifierPtr& expectedIdentifier);
    int numExpectedTypes()const;
    TypeIdentifierPtr getExpectedType(int i);
    
private:
    ConstraintType type;
    TypeIdentifierPtr identifier;
    std::vector<TypeIdentifierPtr> expectedTypes;
};

SWIFT_NS_END

#endif//GENERIC_CONSTRAINT_H