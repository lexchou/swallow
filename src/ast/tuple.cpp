
#include "tuple.h"
USE_SWIFT_NS


Tuple::Tuple()
    :ExpressionNode(0)
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


void Tuple::add(Statement* st)
{
    children.push_back(st);
}
int Tuple::numElements()
{
    return children.size();
}
Statement* Tuple::getElement(int i)
{
    return static_cast<Statement*>(get(i));
}