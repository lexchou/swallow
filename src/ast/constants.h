#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "declaration.h"
#include <string>

SWIFT_NS_BEGIN

class Pattern;
class Expression;
class Constant;
class Constants : public Declaration
{
public:
    Constants();
    ~Constants();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;

    void add(Constant* constant);
    int numConstants();
    Constant* getConstant(int i);

    std::vector<Constant*>::iterator begin(){return constants.begin();}
    std::vector<Constant*>::iterator end(){return constants.end();}
public:
    virtual void serialize(std::wostream& out);
public://Node
    virtual void accept(NodeVisitor* visitor);
public:
    std::vector<Constant*> constants;
};

SWIFT_NS_END

#endif//CONSTANTS_H
