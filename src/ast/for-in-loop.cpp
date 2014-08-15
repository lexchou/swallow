#include "for-in-loop.h"
#include "node-visitor.h"
USE_SWIFT_NS


ForInLoop::ForInLoop()
    :Statement(NodeType::ForIn), loopVars(NULL), container(NULL), codeBlock(NULL)
{
}

ForInLoop::~ForInLoop()
{

}

void ForInLoop::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitForIn);
}


void ForInLoop::setLoopVars(const PatternPtr& val)
{
    loopVars = val;
}
PatternPtr ForInLoop::getLoopVars()
{
    return loopVars;
}

void ForInLoop::setContainer(const ExpressionPtr& val)
{
    this->container = val;
}
ExpressionPtr ForInLoop::getContainer()
{
    return container;
}

void ForInLoop::setCodeBlock(const CodeBlockPtr& val)
{
    codeBlock = val;
}
CodeBlockPtr ForInLoop::getCodeBlock()
{
    return codeBlock;
}
