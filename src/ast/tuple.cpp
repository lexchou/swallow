
#include "tuple.h"
USE_SWIFT_NS


Tuple::Tuple()
    :Expression(0)
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


void Tuple::add(Pattern* st)
{
    children.push_back(st);
}
int Tuple::numElements()
{
    return children.size();
}
Pattern* Tuple::getElement(int i)
{
    return static_cast<Pattern*>(get(i));
}