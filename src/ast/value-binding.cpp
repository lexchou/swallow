
#include "value-binding.h"
#include "node-visitor.h"
USE_SWIFT_NS


ValueBinding::ValueBinding(NodeType::T nodeType)
    :Pattern(nodeType), binding(NULL)
{
}
ValueBinding::~ValueBinding()
{
}

void ValueBinding::setBinding(const PatternPtr& st)
{
    this->binding = st;
}
PatternPtr ValueBinding::getBinding()
{
    return binding;
}

LetBinding::LetBinding()
    :ValueBinding(NodeType::LetBinding)
{
}
void LetBinding::serialize(std::wostream& out)
{
    out<<L"let ";
    getBinding()->serialize(out);
}
void LetBinding::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitLetBinding);
}

VarBinding::VarBinding()
    :ValueBinding(NodeType::VarBinding)
{
}
void VarBinding::serialize(std::wostream& out)
{
    out<<L"var ";
    getBinding()->serialize(out);
}
void VarBinding::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitVarBinding);
}
