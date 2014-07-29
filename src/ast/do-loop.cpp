
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
    accept2(visitor, &NodeVisitor::visitDoLoop);
}

void DoLoop::setCodeBlock(const CodeBlockPtr& codeBlock)
{
    this->codeBlock = codeBlock;
}
CodeBlockPtr DoLoop::getCodeBlock()
{
    return codeBlock;
}

void DoLoop::setCondition(const ExpressionPtr& expression)
{
    this->condition = expression;
}
ExpressionPtr DoLoop::getCondition()
{
    return condition;
}