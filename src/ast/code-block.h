#ifndef CODE_BLOCK_H
#define CODE_BLOCK_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class CodeBlock : public Statement
{
public:
    CodeBlock();
public:
    void addStatement(Statement* st);
    int numStatements();
    Statement* getStatement(int idx);
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//CODE_BLOCK_H
