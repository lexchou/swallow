#include "enum-case-pattern.h"
#include <ast/node-visitor.h>
USE_SWIFT_NS


EnumCasePattern::EnumCasePattern()
    :Pattern(NodeType::EnumCasePattern), associatedBinding(NULL)
{
}
EnumCasePattern::~EnumCasePattern()
{

}

void EnumCasePattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitEnumCasePattern);
}

void EnumCasePattern::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& EnumCasePattern::getName()const
{
    return name;
}

void EnumCasePattern::setAssociatedBinding(const TuplePtr& tuple)
{
    associatedBinding = tuple;
}
TuplePtr EnumCasePattern::getAssociatedBinding()
{
    return associatedBinding;
}