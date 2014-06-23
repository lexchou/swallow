#ifndef LITERAL_NODES_H
#define LITERAL_NODES_H
#include "expression-node.h"
#include <string>

SWIFT_NS_BEGIN

class BooleanLiteral : public ExpressionNode
{
public:
    BooleanLiteral(bool val);
public:
    virtual void serialize(std::wostream& out);
private:
    bool value;
};


SWIFT_NS_END



#endif//LITERAL_NODES_H
