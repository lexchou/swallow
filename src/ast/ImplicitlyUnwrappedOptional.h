#ifndef IMPLICITLY_UNWRAPPED_OPTIONAL_H
#define IMPLICITLY_UNWRAPPED_OPTIONAL_H
#include "TypeNode.h"
#include <string>

SWIFT_NS_BEGIN

class ImplicitlyUnwrappedOptional : public TypeNode
{
public:
    ImplicitlyUnwrappedOptional();
    ~ImplicitlyUnwrappedOptional();
public:
    void setInnerType(const TypeNodePtr& innerType);
    TypeNodePtr getInnerType();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    TypeNodePtr innerType;
};

SWIFT_NS_END

#endif//IMPLICITLY_UNWRAPPED_OPTIONAL_H
