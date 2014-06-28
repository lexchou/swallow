
#include "case-statement.h"
#include "expression.h"
USE_SWIFT_NS


CaseStatement::CaseStatement()
    :Statement(2)
{
}
void CaseStatement::serialize(std::wostream& out)
{
    out<<L"case ";
    getCondition()->serialize(out);
    if(getGuard())
    {
        out<<L" where ";
        getGuard()->serialize(out);
    }
    out<<L":"<<std::endl;
    for(int i = 0; i < numStatements(); i++)
    {
        Statement* st = getStatement(i);
        st->serialize(out);
        out<<std::endl;
    }
}

void CaseStatement::setCondition(Pattern* expr)
{
    set(0, expr);
}
Pattern* CaseStatement::getCondition()
{
    return static_cast<Pattern*>(get(0));
}

void CaseStatement::setGuard(Expression* expr)
{
    set(1, expr);
}
Expression* CaseStatement::getGuard()
{
    return static_cast<Expression*>(get(1));
}

void CaseStatement::addStatement(Statement* statement)
{
    children.push_back(statement);
}
Statement* CaseStatement::getStatement(int idx)
{
    return static_cast<Statement*>(get(idx + 2));
}
int CaseStatement::numStatements()
{
    return children.size() - 2;
}


