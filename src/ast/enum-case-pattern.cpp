
#include "enum-case-pattern.h"
#include "tuple.h"
USE_SWIFT_NS


EnumCasePattern::EnumCasePattern()
    :Pattern(NodeType::EnumCasePattern), associatedBinding(NULL)
{
}
EnumCasePattern::~EnumCasePattern()
{
    SafeDelete(associatedBinding);
}
void EnumCasePattern::serialize(std::wostream& out)
{
}


void EnumCasePattern::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& EnumCasePattern::getName()const
{
    return name;
}

void EnumCasePattern::setAssociatedBinding(Tuple* tuple)
{
    associatedBinding = tuple;
}
Tuple* EnumCasePattern::getAssociatedBinding()
{
    return associatedBinding;
}