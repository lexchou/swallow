#ifndef CONSTANT_H
#define CONSTANT_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
class Constant : public Declaration
{
    typedef std::vector<std::pair<Pattern*, Expression*> > Pairs;
public:
    Constant();
    ~Constant();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
    
    void add(Pattern* pattern, Expression* initializer);
    int numPairs();
    std::pair<Pattern*, Expression*> getPair(int i);

    Pairs::iterator begin(){return pairs.begin();}
    Pairs::iterator end(){return pairs.end();}
public:
    virtual void serialize(std::wostream& out);
public://Node
    virtual void accept(NodeVisitor* visitor);
public:
    Pairs pairs;
};

SWIFT_NS_END

#endif//CONSTANT_H
