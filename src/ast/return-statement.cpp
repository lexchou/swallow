
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
    SafeDelete(expression);
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
    visitor->visitReturn(this);
}

void ReturnStatement::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* ReturnStatement::getExpression()
{
    return expression;
}
