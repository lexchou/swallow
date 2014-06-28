#ifndef DICTIONARY_LITERAL_H
#define DICTIONARY_LITERAL_H
#include "expression.h"

SWIFT_NS_BEGIN

class DictionaryLiteral : public Expression
{
public:
	DictionaryLiteral();
public:
	void insert(Expression* key, Expression* value);
public:
    virtual void serialize(std::wostream& out);
};

SWIFT_NS_END

#endif//DICTIONARY_LITERAL_H