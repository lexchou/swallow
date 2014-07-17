
#include "optional-type.h"
USE_SWIFT_NS


OptionalType::OptionalType()
    :TypeNode(NodeType::OptionalType), innerType(NULL)
{
}
OptionalType::~OptionalType()
{
    SafeDelete(innerType);
}

void OptionalType::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"?";
}



void OptionalType::setInnerType(TypeNode* innerType)
{
    this->innerType = innerType;
}
TypeNode* OptionalType::getInnerType()
{
    return innerType;
}
