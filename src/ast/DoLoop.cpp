#include "DoLoop.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


DoLoop::DoLoop()
    :Statement(NodeType::Do), codeBlock(NULL), condition(NULL)
{
}
DoLoop::~DoLoop()
{

}


void DoLoop::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitDoLoop);
}

void DoLoop::setCodeBlock(const CodeBlockPtr& codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlockPtr DoLoop::getCodeBlock()
{
    return codeBlock;
}

void DoLoop::setCondition(const ExpressionPtr& expression)
{
    this->condition = expression;
}
ExpressionPtr DoLoop::getCondition()
{
    return condition;
}
