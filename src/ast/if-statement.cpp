#include "if-statement.h"
#include "expression.h"
#include "code-block.h"
USE_SWIFT_NS


IfStatement::IfStatement()
    :condition(NULL), thenPart(NULL), elsePart(NULL)
{
}

IfStatement::~IfStatement()
{
    SafeDelete(condition);
    SafeDelete(thenPart);
    SafeDelete(elsePart);
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
    this->condition = expr;
}
Expression* IfStatement::getCondition()
{
    return condition;
}

void IfStatement::setThen(CodeBlock* thenPart)
{
    this->thenPart = thenPart;
}
CodeBlock* IfStatement::getThen()
{
    return thenPart;
}

void IfStatement::setElse(Statement* elsePart)
{
    this->elsePart = elsePart;
}
Statement* IfStatement::getElse()
{
    return elsePart;
}