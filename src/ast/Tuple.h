#ifndef TUPLE_H
#define TUPLE_H
#include "Expression.h"
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
    virtual void accept(NodeVisitor* visitor);
public:
    void add(const PatternPtr& pattern);
    int numElements();
    PatternPtr getElement(int i);
    
    TypeNodePtr getDeclaredType();
    void setDeclaredType(const TypeNodePtr& type);

    std::vector<PatternPtr>::iterator begin(){return elements.begin();}
    std::vector<PatternPtr>::iterator end(){return elements.end();}

public:
    TypeNodePtr declaredType;
    std::vector<PatternPtr> elements;
};

SWIFT_NS_END

#endif//TUPLE_H
