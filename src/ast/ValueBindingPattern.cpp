#include "ValueBindingPattern.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ValueBindingPattern::ValueBindingPattern(NodeType::T nodeType)
    :Pattern(nodeType), binding(NULL)
{
}
ValueBindingPattern::~ValueBindingPattern()
{
}

void ValueBindingPattern::setBinding(const PatternPtr& st)
{
    this->binding = st;
}
PatternPtr ValueBindingPattern::getBinding()
{
    return binding;
}

LetPattern::LetPattern()
    :ValueBindingPattern(NodeType::LetPattern)
{
}

void LetPattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitLetPattern);
}

VarPattern::VarPattern()
    :ValueBindingPattern(NodeType::VarPattern)
{
}

void VarPattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitVarPattern);
}
