
#include "operator-def.h"
USE_SWIFT_NS


OperatorDef::OperatorDef()
    :Declaration(NodeType::Operator)
{
    associativity = Associativity::Left;
    precedence = 100;
    type = OperatorType::_;
}
void OperatorDef::serialize(std::wostream& out)
{
    out<<L"operator ";
    switch(type)
    {
        case OperatorType::InfixBinary:
            out<<L" infix ";
            break;
        case OperatorType::PrefixUnary:
            out<<L" prefix ";
            break;
        case OperatorType::PostfixUnary:
            out<<L" postfix ";
            break;
        default:
            break;
    }
    out<<name<<L"{";
    if(type == OperatorType::InfixBinary)
    {
        out << L"precedence " << precedence << " associativity ";
        switch(associativity)
        {
            case Associativity::Left:
                out<<"left";
                break;
            case Associativity::Right:
                out<<L"right";
                break;
            case Associativity::None:
                out<<L"none";
                break;
            default:
                break;
        }
    }
    out<<L"}";
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