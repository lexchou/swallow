#include "TypedPattern.h"
#include "NodeVisitor.h"
USE_SWIFT_NS

TypedPattern::TypedPattern()
    :Expression(NodeType::TypedPattern)
{
}

TypedPattern::~TypedPattern()
{

}


void TypedPattern::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitTypedPattern);
}

void TypedPattern::setPattern(const PatternPtr& pattern)
{
    this->pattern = pattern;
}
PatternPtr TypedPattern::getPattern()const
{
    return pattern;
}
void TypedPattern::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}
TypeNodePtr TypedPattern::getDeclaredType()
{
    return declaredType;
}

GenericArgumentPtr TypedPattern::getGenericArgument()
{
    return genericArgument;
}
void TypedPattern::setGenericArgument(const GenericArgumentPtr& val)
{
    genericArgument = val;
}
