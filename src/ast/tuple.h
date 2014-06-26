#ifndef TUPLE_H
#define TUPLE_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class Tuple : public ExpressionNode
{
public:
    Tuple();
public:
    virtual void serialize(std::wostream& out);
    void add(Statement* st);
    int numElements();
    Statement* getElement(int i);
    
private:
};

SWIFT_NS_END

#endif//TUPLE_H
