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
    void setCodeBlock(const CodeBlockPtr& codeBlock);
    CodeBlockPtr getCodeBlock();
    
    void setCondition(const ExpressionPtr& expression);
    ExpressionPtr getCondition();
public:
    virtual void accept(NodeVisitor* visitor);
private:
    CodeBlockPtr codeBlock;
    ExpressionPtr condition;
};

SWIFT_NS_END

#endif//WHILE_LOOP_H
