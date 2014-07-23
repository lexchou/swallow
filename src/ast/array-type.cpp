
#include "array-type.h"
#include "node-visitor.h"
USE_SWIFT_NS


ArrayType::ArrayType()
    :TypeNode(NodeType::ArrayType), innerType(NULL)
{
}
ArrayType::~ArrayType()
{
    SafeDelete(innerType);
}
void ArrayType::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"[]";
}

void ArrayType::setInnerType(TypeNode* innerType)
{
    this->innerType = innerType;
}
TypeNode* ArrayType::getInnerType()
{
    return innerType;
}
void ArrayType::accept(NodeVisitor* visitor)
{
//    visitor->visitArrayType(this);
}