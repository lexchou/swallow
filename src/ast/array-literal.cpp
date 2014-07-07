#include "array-literal.h"
USE_SWIFT_NS

ArrayLiteral::ArrayLiteral()
{

}

ArrayLiteral::~ArrayLiteral()
{
    SafeDeleteAll(elements);
}
void ArrayLiteral::push(Expression* item)
{
	elements.push_back(item);
}
Expression* ArrayLiteral::getElement(int i)
{
    return elements[i];
}
int ArrayLiteral::numElements()const
{
    return elements.size();
}

void ArrayLiteral::serialize(std::wostream& out)
{
    out<<L"[";
    for(std::vector<Expression*>::iterator iter = elements.begin(); iter != elements.end(); iter++)
    {
        if(iter != elements.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
    out<<L"]";
}

