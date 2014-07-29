
#include "implicitly-unwrapped-optional.h"
USE_SWIFT_NS


ImplicitlyUnwrappedOptional::ImplicitlyUnwrappedOptional()
    :TypeNode(NodeType::ImplicitlyUnwrappedOptional), innerType(NULL)
{
}
ImplicitlyUnwrappedOptional::~ImplicitlyUnwrappedOptional()
{
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


void ImplicitlyUnwrappedOptional::setInnerType(const TypeNodePtr& innerType)
{
    this->innerType = innerType;
}
TypeNodePtr ImplicitlyUnwrappedOptional::getInnerType()
{
    return innerType;
}
