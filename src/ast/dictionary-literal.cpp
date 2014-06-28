#include "dictionary-literal.h"
USE_SWIFT_NS


DictionaryLiteral::DictionaryLiteral()
	:Expression(0)
{

}

void DictionaryLiteral::insert(Expression* key, Expression* value)
{
	children.push_back(key);
	children.push_back(value);
}


void DictionaryLiteral::serialize(std::wostream& out)
{
    out<<L"[";
    if(children.empty())
        out<<L":";
    for(Children::iterator iter = children.begin(); iter != children.end();)
    {
        if(iter != children.begin())
            out<<L", ";
        (*iter++)->serialize(out);
        out<<L" : ";
        (*iter++)->serialize(out);
    }
    out<<L"]";
}

