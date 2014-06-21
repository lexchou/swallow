#ifndef DICTIONARY_LITERAL_H
#define DICTIONARY_LITERAL_H
#include "expression-node.h"

SWIFT_NS_BEGIN

class DictionaryLiteral : public ExpressionNode
{
public:
	DictionaryLiteral();
public:
	void insert(ExpressionNode* key, ExpressionNode* value);
};

SWIFT_NS_END

#endif//DICTIONARY_LITERAL_H