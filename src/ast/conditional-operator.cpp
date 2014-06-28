#include "conditional-operator.h"
USE_SWIFT_NS

ConditionalOperator::ConditionalOperator(Expression* expr, Expression* trueExpr, Expression* falseExpr)
:Operator(3, OperatorType::Ternary, Associativity::Right, 100)
{
    
}

void ConditionalOperator::serialize(std::wostream& out)
{
    out<<L"((";
    this->getCondition()->serialize(out);
    out<<L")?(";
    this->getTrueExpression()->serialize(out);
    out<<L"):(";
    this->getTrueExpression()->serialize(out);
    out<<L"))";
}