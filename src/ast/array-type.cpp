
#include "array-type.h"
USE_SWIFT_NS


ArrayType::ArrayType()
    :TypeNode(1)
{
}
void ArrayType::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"[]";
}

void ArrayType::setInnerType(TypeNode* innerType)
{
    set(0, innerType);
}
TypeNode* ArrayType::getInnerType()
{
    return static_cast<TypeNode*>(get(0));
}
