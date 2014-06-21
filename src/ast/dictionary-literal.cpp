#include "dictionary-literal.h"
USE_SWIFT_NS


DictionaryLiteral::DictionaryLiteral()
	:ExpressionNode(0)
{

}

void DictionaryLiteral::insert(ExpressionNode* key, ExpressionNode* value)
{
	children.push_back(key);
	children.push_back(value);
}
