
#include "array-type.h"
#include "node-visitor.h"
USE_SWIFT_NS


ArrayType::ArrayType()
    :TypeNode(NodeType::ArrayType), innerType(NULL)
{
}
ArrayType::~ArrayType()
{
}
void ArrayType::serialize(std::wostream& out)
{
    getInnerType()->serialize(out);
    out<<L"[]";
}

void ArrayType::setInnerType(TypeNodePtr innerType)
{
    this->innerType = innerType;
}
TypeNodePtr ArrayType::getInnerType()
{
    return innerType;
}
void ArrayType::accept(NodeVisitor* visitor)
{
    //accept2(visitor, &NodeVisitor::visitArrayType);
}
