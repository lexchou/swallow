#include "DictionaryLiteral.h"
#include "NodeVisitor.h"
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


void DictionaryLiteral::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitDictionaryLiteral);
}
