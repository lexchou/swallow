
#include "continue-statement.h"
#include "node-visitor.h"
USE_SWIFT_NS


ContinueStatement::ContinueStatement()
    :Statement(NodeType::Continue)
{
}
void ContinueStatement::serialize(std::wostream& out)
{
    out<<L"continue";
    if(!loop.empty())
        out<<L" "<<loop;
}
void ContinueStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitContinue);
}


void ContinueStatement::setLoop(const std::wstring& loop)
{
    this->loop = loop;
}
const std::wstring& ContinueStatement::getLoop()const
{
    return loop;
}