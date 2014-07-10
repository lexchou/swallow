
#include "for-in-loop.h"
#include "pattern.h"
#include "expression.h"
#include "code-block.h"
USE_SWIFT_NS


ForInLoop::ForInLoop()
:loopVars(NULL), container(NULL), codeBlock(NULL)
{
}

ForInLoop::~ForInLoop()
{
    SafeDelete(loopVars);
    SafeDelete(container);
    SafeDelete(codeBlock);
}

void ForInLoop::serialize(std::wostream& out)
{

}


void ForInLoop::setLoopVars(Pattern* val)
{
    loopVars = val;
}
Pattern* ForInLoop::getLoopVars()
{
    return loopVars;
}

void ForInLoop::setContainer(Expression* val)
{
    this->container = val;
}
Expression* ForInLoop::getContainer()
{
    return container;
}

void ForInLoop::setCodeBlock(CodeBlock* val)
{
    codeBlock = val;
}
CodeBlock* ForInLoop::getCodeBlock()
{
    return codeBlock;
}
