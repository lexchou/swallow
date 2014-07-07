
#include "return-statement.h"
#include "expression.h"
USE_SWIFT_NS


ReturnStatement::ReturnStatement()
    :expression(NULL)
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

void ReturnStatement::setExpression(Expression* expr)
{
    this->expression = expr;
}
Expression* ReturnStatement::getExpression()
{
    return expression;
}
