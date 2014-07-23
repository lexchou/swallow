
#include "break-statement.h"
#include "node-visitor.h"
USE_SWIFT_NS


BreakStatement::BreakStatement()
    :Statement(NodeType::Break)
{
}
void BreakStatement::serialize(std::wostream& out)
{
    out<<L"break";
    if(!loop.empty())
        out<<L" "<<loop;
}
void BreakStatement::accept(NodeVisitor* visitor)
{
    visitor->visitBreak(this);
}

void BreakStatement::setLoop(const std::wstring& loop)
{
    this->loop = loop;
}
const std::wstring& BreakStatement::getLoop()const
{
    return loop;
}