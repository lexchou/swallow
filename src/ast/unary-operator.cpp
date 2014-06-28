#include "unary-operator.h"
#include "identifier.h"
USE_SWIFT_NS;

UnaryOperator::UnaryOperator(const std::wstring& op, OperatorType::T type)
    :Operator(1, type, Associativity::None ,precedence), op(op)
{
}



void UnaryOperator::serialize(std::wostream& out)
{
    if(type == OperatorType::PrefixUnary)
    {
        out<<op;
        this->getOperand()->serialize(out);
    }
    else
    {
        this->getOperand()->serialize(out);
        out<<op;
    }
}

