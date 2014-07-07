#ifndef FOR_LOOP_H
#define FOR_LOOP_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class ForLoop : public Statement
{
public:
    ForLoop();
    ~ForLoop();
public:
    void addInit(Expression* init);
    int numInit();
    Expression* getInit(int idx);
    
    void setCondition(Expression* cond);
    Expression* getCondition();
    
    void setStep(Expression* step);
    Expression* getStep();
    
    void setCodeBlock(CodeBlock* codeBlock);
    CodeBlock* getCodeBlock();
    
public:
    virtual void serialize(std::wostream& out);
private:
    std::vector<Expression*> inits;
    Expression* condition;
    Expression* step;
    CodeBlock* codeBlock;
};

SWIFT_NS_END

#endif//FOR_LOOP_H
