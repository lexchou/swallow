#ifndef DO_LOOP_H
#define DO_LOOP_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class DoLoop : public Statement
{
public:
    DoLoop();
    ~DoLoop();
public:
    void setCodeBlock(CodeBlock* codeBlock);
    CodeBlock* getCodeBlock();
    
    void setCondition(Expression* expression);
    Expression* getCondition();
public:
    virtual void serialize(std::wostream& out);
    virtual void accept(NodeVisitor* visitor);
private:
    CodeBlock* codeBlock;
    Expression* condition;
};

SWIFT_NS_END

#endif//DO_LOOP_H
