
#include "tuple.h"
#include "type-node.h"
#include "node-visitor.h"
USE_SWIFT_NS


Tuple::Tuple()
    :Expression(NodeType::Tuple), type(NULL)
{
}
Tuple::~Tuple()
{
}

void Tuple::serialize(std::wostream& out)
{
    out<<L"(";
    for(const PatternPtr& p : elements)
    {
        p->serialize(out);
    }
    out<<L")";
}

void Tuple::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitTuple);
}


TypeNodePtr Tuple::getType()
{
    return type;
}
void Tuple::setType(const TypeNodePtr& type)
{
    this->type = type;
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