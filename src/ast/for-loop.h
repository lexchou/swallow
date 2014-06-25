#ifndef FOR_LOOP_H
#define FOR_LOOP_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ExpressionNode;
class CodeBlock;
class ForLoop : public Statement
{
public:
    ForLoop();
public:
    void addInit(ExpressionNode* init);
    int numInit();
    ExpressionNode* getInit(int idx);
    
    void setCondition(ExpressionNode* cond);
    ExpressionNode* getCondition();
    
    void setStep(ExpressionNode* step);
    ExpressionNode* getStep();
    
    void setCodeBlock(CodeBlock* codeBlock);
    CodeBlock* getCodeBlock();
    
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//FOR_LOOP_H
