#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H
#include "statement.h"
#include <string>

SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class IfStatement : public Statement
{
public:
    IfStatement();
    ~IfStatement();
public:
    void setCondition(Expression* expr);
    Expression* getCondition();
    
    void setThen(CodeBlock* thenPart);
    CodeBlock* getThen();
    
    void setElse(Statement* elsePart);
    Statement* getElse();
    
public:
    virtual void serialize(std::wostream& out);
private:
    Expression* condition;
    CodeBlock* thenPart;
    Statement* elsePart;
};

SWIFT_NS_END

#endif//IF_STATEMENT_H
