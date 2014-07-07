
#include "code-block.h"
USE_SWIFT_NS


CodeBlock::CodeBlock()
{
}
CodeBlock::~CodeBlock()
{
    SafeDeleteAll(statements);
    SafeDeleteAll(attributes);
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
    statements.push_back(st);
}
int CodeBlock::numStatements()
{
    return statements.size();
}
Statement* CodeBlock::getStatement(int idx)
{
    return statements[idx];
}


void CodeBlock::setAttributes(const Attributes& attrs)
{
    attributes = attrs;
}
const Attributes& CodeBlock::getAttributes()const
{
    return attributes;
}