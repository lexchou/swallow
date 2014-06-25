
#include "code-block.h"
USE_SWIFT_NS


CodeBlock::CodeBlock()
    :Statement(0)
{
}
void CodeBlock::serialize(std::wostream& out)
{
    out<<L"{"<<std::endl;
    for(int i = 0; i < numStatements(); i++)
    {
        Statement* st = getStatement(i);
        st->serialize(out);
        out<<std::endl;
    }
    out<<L"}"<<std::endl;
}


void CodeBlock::addStatement(Statement* st)
{
    children.push_back(st);
}
int CodeBlock::numStatements()
{
    return children.size();
}
Statement* CodeBlock::getStatement(int idx)
{
    return static_cast<Statement*>(get(idx));
}