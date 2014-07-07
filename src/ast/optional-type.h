#ifndef OPTIONAL_TYPE_H
#define OPTIONAL_TYPE_H
#include "type-node.h"
#include <string>

SWIFT_NS_BEGIN

class OptionalType : public TypeNode
{
public:
    OptionalType();
    ~OptionalType();
public:
    void setInnerType(TypeNode* innerType);
    TypeNode* getInnerType();
public:
    virtual void serialize(std::wostream& out);
private:
    TypeNode* innerType;
};

SWIFT_NS_END

#endif//OPTIONAL_TYPE_H
