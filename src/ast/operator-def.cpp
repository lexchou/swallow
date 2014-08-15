#include "operator-def.h"
#include "node-visitor.h"
USE_SWIFT_NS


OperatorDef::OperatorDef()
    :Declaration(NodeType::Operator)
{
    associativity = Associativity::Left;
    precedence = 100;
    type = OperatorType::_;
}

void OperatorDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitOperator);
}

void OperatorDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& OperatorDef::getName()const
{
    return name;
}
void OperatorDef::setType(OperatorType::T type)
{
    this->type = type;
}
OperatorType::T OperatorDef::getType()
{
    return type;
}


void OperatorDef::setPrecedence(int val)
{
    precedence = val;
}
int OperatorDef::getPrecedence() const
{
    return precedence;
}

void OperatorDef::setAssociativity(Associativity::T val)
{
    associativity = val;
}
Associativity::T OperatorDef::getAssociativity()
{
    return associativity;
}