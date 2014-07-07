#include "type-check.h"
#include "type-node.h"
USE_SWIFT_NS

TypeCheck::TypeCheck(Pattern* expr, TypeNode* type)
:BinaryOperator(L"is", Associativity::None, 132)
{
    setLHS(expr);
    setType(type);
}
TypeCheck::~TypeCheck()
{
    SafeDelete(type);
}

TypeNode* TypeCheck::getType()
{
    return type;
}
void TypeCheck::setType(TypeNode* type)
{
    this->type = type;
}
int TypeCheck::numChildren()
{
    return 2;
}
Node* TypeCheck::get(int i)
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
void TypeCheck::set(int i, Node* val)
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
