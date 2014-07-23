#include "binary-operator.h"
#include "node-visitor.h"
USE_SWIFT_NS;

BinaryOperator::BinaryOperator(const std::wstring& op, Associativity::T associativity, int precedence)
    :Operator(NodeType::BinaryOperator, OperatorType::InfixBinary, associativity, precedence), op(op), lhs(NULL), rhs(NULL)
{
}
BinaryOperator::~BinaryOperator()
{
    SafeDelete(lhs);
    SafeDelete(rhs);
}
void BinaryOperator::serialize(std::wostream& out)
{
    out<<L"(";
    getLHS()->serialize(out);
    out<<op;
    getRHS()->serialize(out);
    out<<L")";    
}
void BinaryOperator::accept(NodeVisitor* visitor)
{
    visitor->visitBinaryOperator(this);
}


int BinaryOperator::numChildren()
{
    return 2;
}
Node* BinaryOperator::get(int i)
{
    switch(i)
    {
        case 0:
            return lhs;
        case 1:
            return rhs;
        default:
            return NULL;
    }
}
void BinaryOperator::set(int i, Node* val)
{
    switch(i)
    {
        case 0:
            lhs = dynamic_cast<Pattern*>(val);
            break;
        case 1:
            rhs = dynamic_cast<Pattern*>(val);
            break;
        default:
            break;
    }
}