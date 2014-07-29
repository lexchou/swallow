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
    ~Constant();
public:
    using Declaration::setSpecifiers;
    using Declaration::getSpecifiers;
public:
    virtual void serialize(std::wostream& out);
public://Node
    virtual void accept(NodeVisitor* visitor);
public:
    PatternPtr name;
    ExpressionPtr initializer;
};

SWIFT_NS_END

#endif//CONSTANT_H
