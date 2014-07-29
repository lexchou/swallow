#ifndef TUPLE_H
#define TUPLE_H
#include "expression.h"
#include <string>
#include "ast-decl.h"

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
    void add(const PatternPtr& pattern);
    int numElements();
    PatternPtr getElement(int i);
    
    TypeNodePtr getType();
    void setType(const TypeNodePtr& type);

    std::vector<PatternPtr>::iterator begin(){return elements.begin();}
    std::vector<PatternPtr>::iterator end(){return elements.end();}

public:
    TypeNodePtr type;
    std::vector<PatternPtr> elements;
};

SWIFT_NS_END

#endif//TUPLE_H
