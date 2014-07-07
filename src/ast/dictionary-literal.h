#ifndef DICTIONARY_LITERAL_H
#define DICTIONARY_LITERAL_H
#include "expression.h"

SWIFT_NS_BEGIN

class DictionaryLiteral : public Expression
{
public:
	DictionaryLiteral();
    ~DictionaryLiteral();
public:
	void insert(Expression* key, Expression* value);
    int numElements()const;
public:
    virtual void serialize(std::wostream& out);
private:
    std::vector<std::pair<Expression*, Expression*> > items;
};

SWIFT_NS_END

#endif//DICTIONARY_LITERAL_H