#include "unary-operator.h"
#include "identifier.h"
USE_SWIFT_NS;

UnaryOperator::UnaryOperator(const std::wstring& op, OperatorType::T type)
    :Operator(type, Associativity::None ,precedence), op(op)
{
}
UnaryOperator::~UnaryOperator()
{
    SafeDelete(operand);
}

int UnaryOperator::numChildren()
{
    return 1;
}
Node* UnaryOperator::get(int i)
{
    switch(i)
    {
        case 0:
            return operand;
        default:
            return NULL;
    }
}
void UnaryOperator::set(int i, Node* val)
{
    if(i == 0)
        operand = dynamic_cast<Expression*>(val);
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

