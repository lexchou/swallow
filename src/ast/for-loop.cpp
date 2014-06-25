
#include "for-loop.h"
#include "expression-node.h"
#include "code-block.h"
USE_SWIFT_NS


ForLoop::ForLoop()
    :Statement(3)
{
}
void ForLoop::serialize(std::wostream& out)
{
    out<<L"for ";
    for(int i = 0; i < numInit(); i++)
    {
        ExpressionNode* init = getInit(i);
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


void ForLoop::addInit(ExpressionNode* init)
{
    children.push_back(init);
}
int ForLoop::numInit()
{
    return children.size() - 3;
}
ExpressionNode* ForLoop::getInit(int idx)
{
    return static_cast<ExpressionNode*>(get(idx + 3));
}

void ForLoop::setCondition(ExpressionNode* cond)
{
    set(0, cond);
}
ExpressionNode* ForLoop::getCondition()
{
    return static_cast<ExpressionNode*>(get(0));
}

void ForLoop::setStep(ExpressionNode* step)
{
    set(1, step);
}
ExpressionNode* ForLoop::getStep()
{
    return static_cast<ExpressionNode*>(get(1));
}

void ForLoop::setCodeBlock(CodeBlock* codeBlock)
{
    set(2, codeBlock);
}
CodeBlock* ForLoop::getCodeBlock()
{
    return static_cast<CodeBlock*>(get(2));
}
