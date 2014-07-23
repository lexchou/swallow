
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
    SafeDelete(condition);
    SafeDelete(step);
    SafeDelete(codeBlock);
    SafeDeleteAll(inits);
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
void ForLoop::accept(NodeVisitor* visitor)
{
    visitor->visitForLoop(this);
}

void ForLoop::addInit(Expression* init)
{
    inits.push_back(init);
}
int ForLoop::numInit()
{
    return inits.size();
}
Expression* ForLoop::getInit(int idx)
{
    return inits[idx];
}

void ForLoop::setCondition(Expression* cond)
{
    condition = cond;
}
Expression* ForLoop::getCondition()
{
    return condition;
}

void ForLoop::setStep(Expression* step)
{
    this->step = step;
}
Expression* ForLoop::getStep()
{
    return step;
}

void ForLoop::setCodeBlock(CodeBlock* codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlock* ForLoop::getCodeBlock()
{
    return codeBlock;
}
