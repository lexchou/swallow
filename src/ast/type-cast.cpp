#include "type-cast.h"
#include "type-node.h"
USE_SWIFT_NS

TypeCast::TypeCast(ExpressionNode* expr, TypeNode* type)
:BinaryOperator(L"as", Associativity::None, 132)
{
    setLHS(expr);
    setType(type);
}

TypeNode* TypeCast::getType()
{
    return static_cast<TypeNode*>(get(1));
}
void TypeCast::setType(TypeNode* type)
{
    set(1, type);
}