#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H
#include "Statement.h"
#include <string>
#include "ast-decl.h"


SWIFT_NS_BEGIN

class Expression;
class CodeBlock;
class IfStatement : public Statement
{
public:
    IfStatement();
    ~IfStatement();
public:
    void setCondition(const ExpressionPtr& expr);
    ExpressionPtr getCondition();
    
    void setThen(const CodeBlockPtr& thenPart);
    CodeBlockPtr getThen();
    
    void setElse(const StatementPtr& elsePart);
    StatementPtr getElse();
    
public:
    virtual void accept(NodeVisitor* visitor);
private:
    ExpressionPtr condition;
    CodeBlockPtr thenPart;
    StatementPtr elsePart;
};

SWIFT_NS_END

#endif//IF_STATEMENT_H
