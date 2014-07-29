
#include "return-statement.h"
#include "expression.h"
#include "node-visitor.h"
USE_SWIFT_NS


ReturnStatement::ReturnStatement()
    :Statement(NodeType::Return), expression(NULL)
{
}
ReturnStatement::~ReturnStatement()
{
}

void ReturnStatement::serialize(std::wostream& out)
{
    out<<L"return";
    if(this->getExpression())
    {
        out<<L" ";
        getExpression()->serialize(out);
    }
}
void ReturnStatement::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitReturn);
}

void ReturnStatement::setExpression(const ExpressionPtr& expr)
{
    this->expression = expr;
}
ExpressionPtr ReturnStatement::getExpression()
{
    return expression;
}
