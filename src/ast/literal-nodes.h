#ifndef LITERAL_NODES_H
#define LITERAL_NODES_H
#include "expression.h"
#include <string>

SWIFT_NS_BEGIN

class BooleanLiteral : public Expression
{
public:
    BooleanLiteral();
public:
    virtual void serialize(std::wostream& out);
public:
    bool value;
};


SWIFT_NS_END



#endif//LITERAL_NODES_H
