#include "dictionary-literal.h"
USE_SWIFT_NS


DictionaryLiteral::DictionaryLiteral()
    :Expression(NodeType::DictionaryLiteral)
{

}
DictionaryLiteral::~DictionaryLiteral()
{
    std::vector<std::pair<Expression*, Expression*> >::iterator iter = items.begin();
    for(; iter != items.end(); iter++)
    {
        delete iter->first;
        delete iter->second;
    }
    items.clear();
}

void DictionaryLiteral::insert(Expression* key, Expression* value)
{
	items.push_back(std::make_pair(key, value));
}

int DictionaryLiteral::numElements()const
{
    return items.size();
}

void DictionaryLiteral::serialize(std::wostream& out)
{
    out<<L"[";
    if(items.empty())
        out<<L":";
    std::vector<std::pair<Expression*, Expression*> >::iterator iter = items.begin();
    for(; iter != items.end(); iter++)
    {
        if(iter != items.begin())
            out<<L", ";
        iter->first->serialize(out);
        out<<L" : ";
        iter->second->serialize(out);
    }
    out<<L"]";
}

