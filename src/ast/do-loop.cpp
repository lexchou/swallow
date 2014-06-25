
#include "do-loop.h"
#include "code-block.h"
#include "expression-node.h"
USE_SWIFT_NS


DoLoop::DoLoop()
    :Statement(2)
{
}
void DoLoop::serialize(std::wostream& out)
{
    using std::endl;
    out<<L"do"<<endl;
    getCodeBlock()->serialize(out);
    out<<" while ";
    getCondition()->serialize(out);
}


void DoLoop::setCodeBlock(CodeBlock* codeBlock)
{
    set(0, codeBlock);
}
CodeBlock* DoLoop::getCodeBlock()
{
    return static_cast<CodeBlock*>(get(0));
}

void DoLoop::setCondition(ExpressionNode* expression)
{
    set(1, expression);
}
ExpressionNode* DoLoop::getCondition()
{
    return static_cast<ExpressionNode*>(get(1));
}