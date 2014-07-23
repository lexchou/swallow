
#include "do-loop.h"
#include "code-block.h"
#include "expression.h"
#include "node-visitor.h"
USE_SWIFT_NS


DoLoop::DoLoop()
    :Statement(NodeType::Do), codeBlock(NULL), condition(NULL)
{
}
DoLoop::~DoLoop()
{
    SafeDelete(codeBlock);
    SafeDelete(condition);
}

void DoLoop::serialize(std::wostream& out)
{
    using std::endl;
    out<<L"do"<<endl;
    getCodeBlock()->serialize(out);
    out<<" while ";
    getCondition()->serialize(out);
}
void DoLoop::accept(NodeVisitor* visitor)
{
    visitor->visitDoLoop(this);
}

void DoLoop::setCodeBlock(CodeBlock* codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlock* DoLoop::getCodeBlock()
{
    return codeBlock;
}

void DoLoop::setCondition(Expression* expression)
{
    this->condition = expression;
}
Expression* DoLoop::getCondition()
{
    return condition;
}