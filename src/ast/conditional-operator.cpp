#include "conditional-operator.h"
USE_SWIFT_NS

ConditionalOperator::ConditionalOperator(ExpressionNode* expr, ExpressionNode* trueExpr, ExpressionNode* falseExpr)
:OperatorNode(3, OperatorType::Ternary, Associativity::Right, 100)
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