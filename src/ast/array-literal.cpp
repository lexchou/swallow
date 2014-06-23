#include "array-literal.h"
USE_SWIFT_NS

ArrayLiteral::ArrayLiteral()
	:ExpressionNode(0)
{

}
void ArrayLiteral::push(ExpressionNode* item)
{
	children.push_back(item);
}

void ArrayLiteral::serialize(std::wostream& out)
{
    out<<L"[";
    for(Children::iterator iter = children.begin(); iter != children.end(); iter++)
    {
        if(iter != children.begin())
            out<<L", ";
        (*iter)->serialize(out);
    }
    out<<L"]";
}

