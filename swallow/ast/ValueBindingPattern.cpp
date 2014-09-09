#include "ValueBindingPattern.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ValueBindingPattern::ValueBindingPattern()
    :Pattern(NodeType::ValueBindingPattern), readonly(false), binding(NULL)
{
}
ValueBindingPattern::~ValueBindingPattern()
{
}
bool ValueBindingPattern::isReadOnly() const
{
    return readonly;
}
void ValueBindingPattern::setReadOnly(bool readonly)
{
    this->readonly = readonly;
}
void ValueBindingPattern::setBinding(const PatternPtr& st)
{
    this->binding = st;
}
PatternPtr ValueBindingPattern::getBinding() const
{
    return binding;
}

TypeNodePtr ValueBindingPattern::getDeclaredType()const
{
    return declaredType;
}
void ValueBindingPattern::setDeclaredType(const TypeNodePtr& type)
{
    declaredType = type;
}


void ValueBindingPattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitValueBindingPattern);
}