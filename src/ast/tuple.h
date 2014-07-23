#ifndef TUPLE_H
#define TUPLE_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class TypeNode;
class Tuple : public Expression
{
public:
    Tuple();
    ~Tuple();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
public:
    void add(Pattern* pattern);
    int numElements();
    Pattern* getElement(int i);
    
    TypeNode* getType();
    void setType(TypeNode* type);

    std::vector<Pattern*>::iterator begin(){return elements.begin();}
    std::vector<Pattern*>::iterator end(){return elements.end();}

public:
    TypeNode* type;
    std::vector<Pattern*> elements;
};

SWIFT_NS_END

#endif//TUPLE_H
