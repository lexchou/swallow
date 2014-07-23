
#include "implicitly-unwrapped-optional.h"
USE_SWIFT_NS


ImplicitlyUnwrappedOptional::ImplicitlyUnwrappedOptional()
    :TypeNode(NodeType::ImplicitlyUnwrappedOptional), innerType(NULL)
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
void ImplicitlyUnwrappedOptional::accept(NodeVisitor* visitor)
{
//
}


void ImplicitlyUnwrappedOptional::setInnerType(TypeNode* innerType)
{
    this->innerType = innerType;
}
TypeNode* ImplicitlyUnwrappedOptional::getInnerType()
{
    return innerType;
}
