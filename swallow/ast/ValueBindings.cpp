#include "ValueBindings.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ValueBindings::ValueBindings()
    :Declaration(NodeType::ValueBindings)
{
}
ValueBindings::~ValueBindings()
{
}

void ValueBindings::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitValueBindings);
}


void ValueBindings::add(const ValueBindingPtr& var)
{
    valueBindings.push_back(var);
}
ValueBindingPtr ValueBindings::get(int i)
{
    return valueBindings[i];
}
int ValueBindings::numBindings()
{
    return valueBindings.size();
}
