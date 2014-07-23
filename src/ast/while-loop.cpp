
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
    SafeDelete(codeBlock);
    SafeDelete(condition);
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
    visitor->visitWhileLoop(this);
}


void WhileLoop::setCodeBlock(CodeBlock* codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlock* WhileLoop::getCodeBlock()
{
    return codeBlock;
}

void WhileLoop::setCondition(Expression* expression)
{
    this->condition = expression;
}
Expression* WhileLoop::getCondition()
{
    return condition;
}