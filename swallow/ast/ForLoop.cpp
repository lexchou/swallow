#include "ForLoop.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ForLoop::ForLoop()
    :Statement(NodeType::For), condition(NULL), step(NULL), codeBlock(NULL)
{
}
ForLoop::~ForLoop()
{

}

void ForLoop::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitForLoop);
}

void ForLoop::addInit(const ExpressionPtr& init)
{
    inits.push_back(init);
}
int ForLoop::numInit()
{
    return inits.size();
}
ExpressionPtr ForLoop::getInit(int idx)
{
    return inits[idx];
}

void ForLoop::setCondition(const ExpressionPtr& cond)
{
    condition = cond;
}
ExpressionPtr ForLoop::getCondition()
{
    return condition;
}

void ForLoop::setStep(const ExpressionPtr& step)
{
    this->step = step;
}
ExpressionPtr ForLoop::getStep()
{
    return step;
}

void ForLoop::setCodeBlock(const CodeBlockPtr& codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlockPtr ForLoop::getCodeBlock()
{
    return codeBlock;
}
