
#include "switch-case.h"
#include "expression.h"
#include "case-statement.h"
#include "node-visitor.h"
USE_SWIFT_NS


SwitchCase::SwitchCase()
    :Statement(NodeType::SwitchCase), controlExpression(NULL), defaultCase(NULL)
{
}

SwitchCase::~SwitchCase()
{
    
    SafeDelete(controlExpression);
    SafeDeleteAll(cases);
    SafeDelete(defaultCase);
}

void SwitchCase::serialize(std::wostream& out)
{
    out<<L"switch ";
    getControlExpression()->serialize(out);
    out<<L"{"<<std::endl;
    out<<L"}"<<std::endl;
}
void SwitchCase::accept(NodeVisitor* visitor)
{
    visitor->visitSwitchCase(this);
}

void SwitchCase::setControlExpression(Expression* expr)
{
    this->controlExpression = expr;
}
Expression* SwitchCase::getControlExpression()
{
    return controlExpression;
}

void SwitchCase::addCase(CaseStatement* c)
{
    cases.push_back(c);
}
int SwitchCase::numCases()
{
    return cases.size();
}
CaseStatement* SwitchCase::getCase(int idx)
{
    return cases[idx];
}

void SwitchCase::setDefaultCase(CaseStatement* c)
{
    this->defaultCase = c;
}
CaseStatement* SwitchCase::getDefaultCase()
{
    return defaultCase;
}
