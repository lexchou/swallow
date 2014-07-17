#ifndef STATEMENT_H
#define STATEMENT_H
#include "node.h"
#include <string>

SWIFT_NS_BEGIN

class Statement : public Node
{
public:
    Statement(NodeType::T nodeType);
private:
};

SWIFT_NS_END

#endif//STATEMENT_H
