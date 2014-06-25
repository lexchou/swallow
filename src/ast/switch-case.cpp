
#include "switch-case.h"
#include "expression-node.h"
#include "case-statement.h"
USE_SWIFT_NS


SwitchCase::SwitchCase()
    :Statement(2)
{
}
void SwitchCase::serialize(std::wostream& out)
{
    out<<L"switch ";
    getControlExpression()->serialize(out);
    out<<L"{"<<std::endl;
    out<<L"}"<<std::endl;
}

void SwitchCase::setControlExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* SwitchCase::getControlExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}

void SwitchCase::addCase(CaseStatement* c)
{
    children.push_back(c);
}
int SwitchCase::numCases()
{
    return children.size() - 2;
}
CaseStatement* SwitchCase::getCase(int idx)
{
    return static_cast<CaseStatement*>(get(idx + 2));
}

void SwitchCase::setDefaultCase(CaseStatement* c)
{
    set(1, c);
}
CaseStatement* SwitchCase::getDefaultCase()
{
    return static_cast<CaseStatement*>(get(1));
}
