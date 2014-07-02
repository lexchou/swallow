
#include "tuple.h"
#include "type-node.h"
USE_SWIFT_NS


Tuple::Tuple()
    :Expression(1)
{
}
void Tuple::serialize(std::wostream& out)
{
    out<<L"(";
    Children::iterator iter = children.begin();
    for(; iter != children.end(); iter++)
    {
        (*iter)->serialize(out);
    }
    out<<L")";
}


TypeNode* Tuple::getType()
{
    return static_cast<TypeNode*>(get(0));
}
void Tuple::setType(TypeNode* type)
{
    set(0, type);
}

void Tuple::add(Pattern* st)
{
    children.push_back(st);
}
int Tuple::numElements()
{
    return children.size() - 1;
}
Pattern* Tuple::getElement(int i)
{
    return static_cast<Pattern*>(get(i + 1));
}