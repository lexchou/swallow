#include "SwitchCase.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


SwitchCase::SwitchCase()
    :Statement(NodeType::SwitchCase), controlExpression(NULL), defaultCase(NULL)
{
}

SwitchCase::~SwitchCase()
{
    

}

void SwitchCase::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSwitchCase);
}

void SwitchCase::setControlExpression(const ExpressionPtr& expr)
{
    this->controlExpression = expr;
}
ExpressionPtr SwitchCase::getControlExpression()
{
    return controlExpression;
}

void SwitchCase::addCase(const CaseStatementPtr& c)
{
    cases.push_back(c);
}
int SwitchCase::numCases()
{
    return cases.size();
}
CaseStatementPtr SwitchCase::getCase(int idx)
{
    return cases[idx];
}

void SwitchCase::setDefaultCase(const CaseStatementPtr& c)
{
    this->defaultCase = c;
}
CaseStatementPtr SwitchCase::getDefaultCase()
{
    return defaultCase;
}
