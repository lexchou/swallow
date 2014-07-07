#ifndef WHILE_LOOP_H
#define WHILE_LOOP_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class WhileLoop : public Statement
{
public:
    WhileLoop();
    ~WhileLoop();
public:
    void setCodeBlock(CodeBlock* codeBlock);
    CodeBlock* getCodeBlock();
    
    void setCondition(Expression* expression);
    Expression* getCondition();
public:
    virtual void serialize(std::wostream& out);
private:
    CodeBlock* codeBlock;
    Expression* condition;
};

SWIFT_NS_END

#endif//WHILE_LOOP_H
