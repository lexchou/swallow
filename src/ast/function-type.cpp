
#include "function-type.h"
USE_SWIFT_NS


FunctionType::FunctionType()
    :TypeNode(NodeType::FunctionType)
{
}
FunctionType::~FunctionType()
{
}

void FunctionType::serialize(std::wostream& out)
{
    getArgumentsType()->serialize(out);
    out<<L" -> ";
    getReturnType()->serialize(out);
}
void FunctionType::accept(NodeVisitor* visitor)
{
//
}
void FunctionType::setArgumentsType(const TypeNodePtr& argumentsType)
{
    this->argumentsType = argumentsType;
}

TypeNodePtr FunctionType::getArgumentsType()
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
