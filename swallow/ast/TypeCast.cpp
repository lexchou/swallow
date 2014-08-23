#include "TypeCast.h"
#include "TypeNode.h"
USE_SWIFT_NS

TypeCast::TypeCast()
{
    op = L"as";
    associativity = Associativity::None;
    precedence = 132;

    optional = false;
}
TypeCast::~TypeCast()
{
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
TypeNodePtr TypeCast::getDeclaredType()
{
    return declaredType;
}
void TypeCast::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}

int TypeCast::numChildren()
{
    return 2;
}
NodePtr TypeCast::get(int i)
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
void TypeCast::set(int i, const NodePtr& val)
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

