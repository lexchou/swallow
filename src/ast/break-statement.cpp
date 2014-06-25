
#include "break-statement.h"
USE_SWIFT_NS


BreakStatement::BreakStatement()
    :Statement(0)
{
}
void BreakStatement::serialize(std::wostream& out)
{
    out<<L"break";
    if(!loop.empty())
        out<<L" "<<loop;
}


void BreakStatement::setLoop(const std::wstring& loop)
{
    this->loop = loop;
}
const std::wstring& BreakStatement::getLoop()const
{
    return loop;
}