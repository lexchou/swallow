#ifndef TUPLE_H
#define TUPLE_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class Tuple : public Expression
{
public:
    Tuple();
public:
    virtual void serialize(std::wostream& out);
    void add(Pattern* pattern);
    int numElements();
    Pattern* getElement(int i);
    
private:
};

SWIFT_NS_END

#endif//TUPLE_H
