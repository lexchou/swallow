
#include "return-statement.h"
#include "expression-node.h"
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

void ReturnStatement::setExpression(ExpressionNode* expr)
{
    set(0, expr);
}
ExpressionNode* ReturnStatement::getExpression()
{
    return static_cast<ExpressionNode*>(get(0));
}
