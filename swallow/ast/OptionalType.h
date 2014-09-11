#ifndef OPTIONAL_TYPE_H
#define OPTIONAL_TYPE_H
#include "TypeNode.h"
#include <string>

SWIFT_NS_BEGIN

class OptionalType : public TypeNode
{
public:
    OptionalType();
    ~OptionalType();
public:
    void setInnerType(TypeNodePtr innerType);
    TypeNodePtr getInnerType();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    TypeNodePtr innerType;
};

SWIFT_NS_END

#endif//OPTIONAL_TYPE_H