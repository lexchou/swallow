
#include "case-statement.h"
#include "expression.h"
#include "node-visitor.h"
USE_SWIFT_NS


CaseStatement::CaseStatement()
    :Statement(NodeType::Case)
{
}
CaseStatement::~CaseStatement()
{
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
        StatementPtr st = getStatement(i);
        st->serialize(out);
        out<<std::endl;
    }
}
void CaseStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitCase);
}


void CaseStatement::addCondition(const PatternPtr& condition, const ExpressionPtr& guard)
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

void CaseStatement::addStatement(const StatementPtr& statement)
{
    statements.push_back(statement);
}
StatementPtr CaseStatement::getStatement(int idx)
{
    return statements[idx];
}
int CaseStatement::numStatements()
{
    return statements.size();
}


