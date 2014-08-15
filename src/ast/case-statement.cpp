#include "case-statement.h"
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


