
#include "enum-case-pattern.h"
#include "tuple.h"
USE_SWIFT_NS


EnumCasePattern::EnumCasePattern()
    :Pattern(1)
{
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
    set(0, tuple);
}
Tuple* EnumCasePattern::getAssociatedBinding()
{
    return static_cast<Tuple*>(get(0));
}