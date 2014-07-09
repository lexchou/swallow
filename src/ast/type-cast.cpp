#include "type-cast.h"
#include "type-node.h"
USE_SWIFT_NS

TypeCast::TypeCast(Expression* expr, TypeNode* type)
:BinaryOperator(L"as", Associativity::None, 132)
{
    setLHS(expr);
    setType(type);
    optional = false;
}
TypeCast::~TypeCast()
{
    SafeDelete(type);
}

void TypeCast::setOptional(bool val)
{
    optional = val;
    this->op = val ? L"as?" : L"as";
}
bool TypeCast::isOptional()const
{
    return optional;
}
TypeNode* TypeCast::getType()
{
    return type;
}
void TypeCast::setType(TypeNode* type)
{
    this->type = type;
}

int TypeCast::numChildren()
{
    return 2;
}
Node* TypeCast::get(int i)
{
    switch(i)
    {
        case 0:
            return lhs;
        case 1:
            return type;
        default:
            return NULL;
    }
}
void TypeCast::set(int i, Node* val)
{
    switch(i)
    {
        case 0:
            lhs = dynamic_cast<Pattern*>(val);
            break;
        case 1:
            type = dynamic_cast<TypeNode*>(val);
            break;
        default:
            break;
    }
}

