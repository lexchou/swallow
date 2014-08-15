
#include "optional-type.h"
USE_SWIFT_NS


OptionalType::OptionalType()
    :TypeNode(NodeType::OptionalType), innerType(NULL)
{
}
OptionalType::~OptionalType()
{
}

void OptionalType::accept(NodeVisitor* visitor)
{
//
}



void OptionalType::setInnerType(TypeNodePtr innerType)
{
    this->innerType = innerType;
}
TypeNodePtr OptionalType::getInnerType()
{
    return innerType;
}
