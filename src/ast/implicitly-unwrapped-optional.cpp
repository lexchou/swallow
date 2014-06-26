
#include "implicitly-unwrapped-optional.h"
USE_SWIFT_NS


ImplicitlyUnwrappedOptional::ImplicitlyUnwrappedOptional()
    :TypeNode(1)
{
}

void ImplicitlyUnwrappedOptional::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"!";
}



void ImplicitlyUnwrappedOptional::setInnerType(TypeNode* innerType)
{
    set(0, innerType);
}
TypeNode* ImplicitlyUnwrappedOptional::getInnerType()
{
    return static_cast<TypeNode*>(get(0));
}
