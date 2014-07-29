
#include "while-loop.h"
#include "expression.h"
#include "code-block.h"
#include "node-visitor.h"
USE_SWIFT_NS


WhileLoop::WhileLoop()
    :Statement(NodeType::While), codeBlock(NULL), condition(NULL)
{
}

WhileLoop::~WhileLoop()
{

}

void WhileLoop::serialize(std::wostream& out)
{
    out<<L"while ";
    getCondition()->serialize(out);
    getCodeBlock()->serialize(out);
    out<<std::endl;
}
void WhileLoop::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitWhileLoop);
}


void WhileLoop::setCodeBlock(const CodeBlockPtr& codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlockPtr WhileLoop::getCodeBlock()
{
    return codeBlock;
}

void WhileLoop::setCondition(const ExpressionPtr& expression)
{
    this->condition = expression;
}
ExpressionPtr WhileLoop::getCondition()
{
    return condition;
}