#ifndef CONSTANT_H
#define CONSTANT_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
class Constant : public Declaration
{
public:
    Constant();
public:
    using Declaration::setSpecifiers;
    void add(Pattern* pattern, Expression* initializer);
    int numPairs();
    std::pair<Pattern*, Expression*> getPair(int i);
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//CONSTANT_H
