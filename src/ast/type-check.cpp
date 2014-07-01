#include "type-check.h"
#include "type-node.h"
USE_SWIFT_NS

TypeCheck::TypeCheck(Pattern* expr, TypeNode* type)
:BinaryOperator(L"is", Associativity::None, 132)
{
    setLHS(expr);
    setType(type);
}
TypeNode* TypeCheck::getType()
{
    return static_cast<TypeNode*>(get(1));
}
void TypeCheck::setType(TypeNode* type)
{
    set(1, type);
}