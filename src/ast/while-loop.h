#ifndef WHILE_LOOP_H
#define WHILE_LOOP_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ExpressionNode;
class CodeBlock;
class WhileLoop : public Statement
{
public:
    WhileLoop();
public:
    void setCodeBlock(CodeBlock* codeBlock);
    CodeBlock* getCodeBlock();
    
    void setCondition(ExpressionNode* expression);
    ExpressionNode* getCondition();
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//WHILE_LOOP_H
