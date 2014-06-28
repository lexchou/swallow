
#include "for-loop.h"
#include "expression.h"
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
        Expression* init = getInit(i);
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


void ForLoop::addInit(Expression* init)
{
    children.push_back(init);
}
int ForLoop::numInit()
{
    return children.size() - 3;
}
Expression* ForLoop::getInit(int idx)
{
    return static_cast<Expression*>(get(idx + 3));
}

void ForLoop::setCondition(Expression* cond)
{
    set(0, cond);
}
Expression* ForLoop::getCondition()
{
    return static_cast<Expression*>(get(0));
}

void ForLoop::setStep(Expression* step)
{
    set(1, step);
}
Expression* ForLoop::getStep()
{
    return static_cast<Expression*>(get(1));
}

void ForLoop::setCodeBlock(CodeBlock* codeBlock)
{
    set(2, codeBlock);
}
CodeBlock* ForLoop::getCodeBlock()
{
    return static_cast<CodeBlock*>(get(2));
}
