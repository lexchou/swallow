
#include "code-block.h"
#include "node-visitor.h"
USE_SWIFT_NS


CodeBlock::CodeBlock()
    :Statement(NodeType::CodeBlock)
{
}
CodeBlock::~CodeBlock()
{
}

void CodeBlock::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitCodeBlock);
}


void CodeBlock::addStatement(const StatementPtr& st)
{
    statements.push_back(st);
}
int CodeBlock::numStatements()
{
    return statements.size();
}
StatementPtr CodeBlock::getStatement(int idx)
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

const TypePtr& CodeBlock::getType()const
{
    return type;
}
void CodeBlock::setType(const TypePtr& type)
{
    this->type = type;
}