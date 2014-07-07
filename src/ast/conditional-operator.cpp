#include "conditional-operator.h"
USE_SWIFT_NS

ConditionalOperator::ConditionalOperator(Pattern* expr, Expression* trueExpr, Expression* falseExpr)
:Operator(OperatorType::Ternary, Associativity::Right, 100)
{
    this->setCondition(expr);
    this->setTrueExpression(trueExpr);
    this->setFalseExpression(falseExpr);
}

ConditionalOperator::~ConditionalOperator()
{
    SafeDelete(condition);
    SafeDelete(trueExpression);
    SafeDelete(falseExpression);
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


int ConditionalOperator::numChildren()
{
    return 3;
}
Node* ConditionalOperator::get(int i)
{
    switch(i)
    {
        case 0:
            return condition;
        case 1:
            return trueExpression;
        case 2:
            return falseExpression;
        default:
            return NULL;
    }
}
void ConditionalOperator::set(int i, Node* val)
{
    switch(i)
    {
        case 0:
            condition = dynamic_cast<Pattern*>(val);
            break;
        case 1:
            trueExpression = dynamic_cast<Expression*>(val);
            break;
        case 2:
            falseExpression = dynamic_cast<Expression*>(val);
            break;
        default:
            break;
    }
}
