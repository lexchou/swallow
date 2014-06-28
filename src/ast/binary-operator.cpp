#include "binary-operator.h"
USE_SWIFT_NS;

BinaryOperator::BinaryOperator(const std::wstring& op, Associativity::T associativity, int precedence)
:Operator(2, OperatorType::InfixBinary, associativity, precedence), op(op)
{
}
void BinaryOperator::serialize(std::wostream& out)
{
    out<<L"(";
    getLHS()->serialize(out);
    out<<op;
    getRHS()->serialize(out);
    out<<L")";    
}