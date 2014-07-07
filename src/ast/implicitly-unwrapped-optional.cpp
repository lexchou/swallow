
#include "implicitly-unwrapped-optional.h"
USE_SWIFT_NS


ImplicitlyUnwrappedOptional::ImplicitlyUnwrappedOptional()
    :innerType(NULL)
{
}
ImplicitlyUnwrappedOptional::~ImplicitlyUnwrappedOptional()
{
    SafeDelete(innerType);
}

void ImplicitlyUnwrappedOptional::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"!";
}



void ImplicitlyUnwrappedOptional::setInnerType(TypeNode* innerType)
{
    this->innerType = innerType;
}
TypeNode* ImplicitlyUnwrappedOptional::getInnerType()
{
    return innerType;
}
