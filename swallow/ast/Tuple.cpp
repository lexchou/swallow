#include "Tuple.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


Tuple::Tuple()
    :Expression(NodeType::Tuple)
{
}
Tuple::~Tuple()
{
}

void Tuple::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitTuple);
}


TypeNodePtr Tuple::getDeclaredType()
{
    return declaredType;
}
void Tuple::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}

void Tuple::add(const PatternPtr& st)
{
    elements.push_back(st);
}
int Tuple::numElements()
{
    return elements.size();
}
PatternPtr Tuple::getElement(int i)
{
    return elements[i];
}
