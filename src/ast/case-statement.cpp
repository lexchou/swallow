
#include "case-statement.h"
#include "expression-node.h"
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

void CaseStatement::setCondition(Statement* expr)
{
    set(0, expr);
}
Statement* CaseStatement::getCondition()
{
    return static_cast<Statement*>(get(0));
}

void CaseStatement::setGuard(ExpressionNode* expr)
{
    set(1, expr);
}
ExpressionNode* CaseStatement::getGuard()
{
    return static_cast<ExpressionNode*>(get(1));
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


