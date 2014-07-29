#include "dictionary-literal.h"
#include "node-visitor.h"
USE_SWIFT_NS


DictionaryLiteral::DictionaryLiteral()
    :Expression(NodeType::DictionaryLiteral)
{

}
DictionaryLiteral::~DictionaryLiteral()
{
}

void DictionaryLiteral::insert(const ExpressionPtr& key, const ExpressionPtr& value)
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
    std::vector<std::pair<ExpressionPtr, ExpressionPtr> >::iterator iter = items.begin();
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
void DictionaryLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitDictionaryLiteral);
}
