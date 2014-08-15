#ifndef FOR_LOOP_H
#define FOR_LOOP_H
#include "statement.h"
#include <string>
#include "ast-decl.h"

SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class ForLoop : public Statement
{
public:
    ForLoop();
    ~ForLoop();
public:
    void addInit(const ExpressionPtr& init);
    int numInit();
    ExpressionPtr getInit(int idx);
    
    void setCondition(const ExpressionPtr& cond);
    ExpressionPtr getCondition();
    
    void setStep(const ExpressionPtr& step);
    ExpressionPtr getStep();
    
    void setCodeBlock(const CodeBlockPtr& codeBlock);
    CodeBlockPtr getCodeBlock();
    
public:
    virtual void accept(NodeVisitor* visitor);
public:
    std::vector<ExpressionPtr> inits;
    ExpressionPtr condition;
    ExpressionPtr step;
    CodeBlockPtr codeBlock;
};

SWIFT_NS_END

#endif//FOR_LOOP_H
