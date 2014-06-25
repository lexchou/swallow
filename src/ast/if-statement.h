#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class ExpressionNode;
class CodeBlock;
class IfStatement : public Statement
{
public:
    IfStatement();
public:
    void setCondition(ExpressionNode* expr);
    ExpressionNode* getCondition();
    
    void setThen(CodeBlock* thenPart);
    CodeBlock* getThen();
    
    void setElse(Statement* elsePart);
    Statement* getElse();
    
public:
    virtual void serialize(std::wostream& out);
private:
};

SWIFT_NS_END

#endif//IF_STATEMENT_H
