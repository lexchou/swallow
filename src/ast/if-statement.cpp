#include "if-statement.h"
#include "expression.h"
#include "code-block.h"
USE_SWIFT_NS


IfStatement::IfStatement()
    :Statement(3)
{
}
void IfStatement::serialize(std::wostream& out)
{
    out<<L"if ";
    getCondition()->serialize(out);
    out<<std::endl;
    getThen()->serialize(out);
    if(getElse() != NULL)
    {
        out<<L"else";
        getElse()->serialize(out);
    }
}



void IfStatement::setCondition(Expression* expr)
{
    set(0, expr);
}
Expression* IfStatement::getCondition()
{
    return static_cast<Expression*>(get(0));
}

void IfStatement::setThen(CodeBlock* thenPart)
{
    set(1, thenPart);
}
CodeBlock* IfStatement::getThen()
{
    return static_cast<CodeBlock*>(get(1));
}

void IfStatement::setElse(Statement* elsePart)
{
    set(2, elsePart);
}
Statement* IfStatement::getElse()
{
    return static_cast<Statement*>(get(2));
}