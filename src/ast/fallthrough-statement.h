#ifndef FALLTHROUGH_STATEMENT_H
#define FALLTHROUGH_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class FallthroughStatement : public Statement
{
public:
    FallthroughStatement();
public:
    virtual void accept(NodeVisitor* visitor);
private:
};

SWIFT_NS_END

#endif//FALLTHROUGH_STATEMENT_H
