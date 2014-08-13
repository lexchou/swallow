
#include "value-binding-pattern.h"
#include "node-visitor.h"
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
void LetPattern::serialize(std::wostream& out)
{
    out<<L"let ";
    getBinding()->serialize(out);
}
void LetPattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitLetPattern);
}

VarPattern::VarPattern()
    :ValueBindingPattern(NodeType::VarPattern)
{
}
void VarPattern::serialize(std::wostream& out)
{
    out<<L"var ";
    getBinding()->serialize(out);
}
void VarPattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitVarPattern);
}
