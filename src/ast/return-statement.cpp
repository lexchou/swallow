
#include "return-statement.h"
#include "expression.h"
USE_SWIFT_NS


ReturnStatement::ReturnStatement()
    :Statement(1)
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

void ReturnStatement::setExpression(Expression* expr)
{
    set(0, expr);
}
Expression* ReturnStatement::getExpression()
{
    return static_cast<Expression*>(get(0));
}
