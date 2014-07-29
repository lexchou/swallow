#ifndef DICTIONARY_LITERAL_H
#define DICTIONARY_LITERAL_H
#include "expression.h"

SWIFT_NS_BEGIN

class DictionaryLiteral : public Expression
{
    typedef std::vector<std::pair<ExpressionPtr, ExpressionPtr> > Map;
public:
	DictionaryLiteral();
    ~DictionaryLiteral();
public:
	void insert(const ExpressionPtr& key, const ExpressionPtr& value);
    int numElements()const;
public:
    Map::iterator begin(){return items.begin();}
    Map::iterator end(){return items.end();}
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    Map items;
};

SWIFT_NS_END

#endif//DICTIONARY_LITERAL_H