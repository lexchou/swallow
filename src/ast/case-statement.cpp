
#include "case-statement.h"
#include "expression.h"
USE_SWIFT_NS


CaseStatement::CaseStatement()
    :Statement(NodeType::Case)
{
}
CaseStatement::~CaseStatement()
{
    SafeDeleteAll(statements);
    std::vector<Condition>::iterator iter = conditions.begin();
    for(; iter != conditions.end(); iter++)
    {
        if(iter->condition)
            delete iter->condition;
        if(iter->guard)
            delete iter->guard;
    }
    conditions.clear();
}
void CaseStatement::serialize(std::wostream& out)
{
    std::vector<Condition>::iterator iter = conditions.begin();
    if(iter != conditions.end())
        out<<L"case ";
    else
        out<<L"default";
    for(; iter != conditions.end(); iter++)
    {
        if(iter != conditions.begin())
            out<<L", ";
        iter->condition->serialize(out);
        
        if(iter->guard)
        {
            out<<L" where ";
            iter->guard->serialize(out);
        }
    }
    out<<L":"<<std::endl;
    for(int i = 0; i < numStatements(); i++)
    {
        Statement* st = getStatement(i);
        st->serialize(out);
        out<<std::endl;
    }
}


void CaseStatement::addCondition(Pattern* condition, Expression* guard)
{
    conditions.push_back(Condition(condition, guard));
}
int CaseStatement::numConditions()const
{
    return conditions.size();
}
const CaseStatement::Condition& CaseStatement::getCondition(int i)
{
    return conditions[i];
}

void CaseStatement::addStatement(Statement* statement)
{
    statements.push_back(statement);
}
Statement* CaseStatement::getStatement(int idx)
{
    return statements[idx];
}
int CaseStatement::numStatements()
{
    return statements.size();
}


