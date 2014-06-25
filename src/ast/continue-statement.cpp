
#include "continue-statement.h"
USE_SWIFT_NS


ContinueStatement::ContinueStatement()
    :Statement(0)
{
}
void ContinueStatement::serialize(std::wostream& out)
{
    out<<L"continue";
    if(!loop.empty())
        out<<L" "<<loop;
}


void ContinueStatement::setLoop(const std::wstring& loop)
{
    this->loop = loop;
}
const std::wstring& ContinueStatement::getLoop()const
{
    return loop;
}