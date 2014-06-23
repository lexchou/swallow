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
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//DICTIONARY_LITERAL_H