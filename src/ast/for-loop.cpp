
#include "for-loop.h"
#include "expression.h"
#include "code-block.h"
#include "node-visitor.h"
USE_SWIFT_NS


ForLoop::ForLoop()
    :Statement(NodeType::For), condition(NULL), step(NULL), codeBlock(NULL)
{
}
ForLoop::~ForLoop()
{

}

void ForLoop::serialize(std::wostream& out)
{
    out<<L"for ";
    for(int i = 0; i < numInit(); i++)
    {
        ExpressionPtr init = getInit(i);
        if(i > 0)
            out<<L", ";
        init->serialize(out);
    }
    out<<L"; ";
    if(getCondition() != NULL)
        getCondition()->serialize(out);
    out<<L"; ";
    if(getStep() != NULL)
        getStep()->serialize(out);
    out<<std::endl;
    getCodeBlock()->serialize(out);
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
