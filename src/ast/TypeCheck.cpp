#include "TypeCheck.h"
#include "TypeNode.h"
USE_SWIFT_NS

TypeCheck::TypeCheck()
{
    op = L"is";
    associativity = Associativity::None;
    precedence = 132;
}
TypeCheck::~TypeCheck()
{
}

TypeNodePtr TypeCheck::getDeclaredType()
{
    return declaredType;
}
void TypeCheck::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}
int TypeCheck::numChildren()
{
    return 2;
}
NodePtr TypeCheck::get(int i)
{
    switch(i)
    {
        case 0:
            return lhs;
        case 1:
            return declaredType;
        default:
            return NULL;
    }
}
void TypeCheck::set(int i, const NodePtr& val)
{
    switch(i)
    {
        case 0:
            lhs = std::dynamic_pointer_cast<Pattern>(val);
            break;
        case 1:
            declaredType = std::dynamic_pointer_cast<TypeNode>(val);
            break;
        default:
            break;
    }
}
