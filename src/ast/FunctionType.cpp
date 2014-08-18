#include "FunctionType.h"
#include "TupleType.h"
USE_SWIFT_NS


FunctionType::FunctionType()
    :TypeNode(NodeType::FunctionType)
{
}
FunctionType::~FunctionType()
{
}

void FunctionType::accept(NodeVisitor* visitor)
{
//
}
void FunctionType::setArgumentsType(const TupleTypePtr& argumentsType)
{
    this->argumentsType = argumentsType;
}

TupleTypePtr FunctionType::getArgumentsType()
{
    return argumentsType;
}
void FunctionType::setReturnType(const TypeNodePtr& retType)
{
    this->returnType = retType;
}


TypeNodePtr FunctionType::getReturnType()
{
    return returnType;
}
