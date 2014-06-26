
#include "optional-type.h"
USE_SWIFT_NS


OptionalType::OptionalType()
    :TypeNode(1)
{
}
void OptionalType::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"?";
}



void OptionalType::setInnerType(TypeNode* innerType)
{
    set(0, innerType);
}
TypeNode* OptionalType::getInnerType()
{
    return static_cast<TypeNode*>(get(0));
}
