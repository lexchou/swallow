
#include "while-loop.h"
#include "expression-node.h"
#include "code-block.h"
USE_SWIFT_NS


WhileLoop::WhileLoop()
    :Statement(2)
{
}
void WhileLoop::serialize(std::wostream& out)
{
    out<<L"while ";
    getCondition()->serialize(out);
    getCodeBlock()->serialize(out);
    out<<std::endl;
}



void WhileLoop::setCodeBlock(CodeBlock* codeBlock)
{
    set(1, codeBlock);
}
CodeBlock* WhileLoop::getCodeBlock()
{
    return static_cast<CodeBlock*>(get(1));
}

void WhileLoop::setCondition(ExpressionNode* expression)
{
    set(0, expression);
}
ExpressionNode* WhileLoop::getCondition()
{
    return static_cast<ExpressionNode*>(get(0));
}