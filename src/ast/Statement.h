#ifndef STATEMENT_H
#define STATEMENT_H
#include "Node.h"
#include <string>

SWIFT_NS_BEGIN

class Statement : public Node
{
public:
    Statement(NodeType::T nodeType);
private:
};
typedef std::shared_ptr<Statement> StatementPtr;

SWIFT_NS_END

#endif//STATEMENT_H
