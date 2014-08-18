#ifndef ARRAY_TYPE_H
#define ARRAY_TYPE_H
#include "TypeNode.h"
#include <string>

SWIFT_NS_BEGIN

class ArrayType : public TypeNode
{
public:
    ArrayType();
    ~ArrayType();
public:
    void setInnerType(TypeNodePtr innerType);
    TypeNodePtr getInnerType();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    TypeNodePtr innerType;
};

SWIFT_NS_END

#endif//ARRAY_TYPE_H
