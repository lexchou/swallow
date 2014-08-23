#ifndef DO_LOOP_H
#define DO_LOOP_H
#include "Statement.h"
#include <string>
#include "ast-decl.h"

SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class DoLoop : public Statement
{
public:
    DoLoop();
    ~DoLoop();
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

#endif//DO_LOOP_H
