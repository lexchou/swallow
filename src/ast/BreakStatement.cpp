
#include "BreakStatement.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


BreakStatement::BreakStatement()
    :Statement(NodeType::Break)
{
}

void BreakStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitBreak);
}

void BreakStatement::setLoop(const std::wstring& loop)
{
    this->loop = loop;
}
const std::wstring& BreakStatement::getLoop()const
{
    return loop;
}
