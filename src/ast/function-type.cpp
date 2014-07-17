
#include "function-type.h"
USE_SWIFT_NS


FunctionType::FunctionType()
    :TypeNode(NodeType::FunctionType)
{
}
FunctionType::~FunctionType()
{
    SafeDelete(returnType);
    SafeDelete(argumentsType);
}

void FunctionType::serialize(std::wostream& out)
{
    getArgumentsType()->serialize(out);
    out<<L" -> ";
    getReturnType()->serialize(out);
}

void FunctionType::setArgumentsType(TypeNode* argumentsType)
{
    this->argumentsType = argumentsType;
}

TypeNode* FunctionType::getArgumentsType()
{
    return argumentsType;
}
void FunctionType::setReturnType(TypeNode* retType)
{
    this->returnType = retType;
}


TypeNode* FunctionType::getReturnType()
{
    return returnType;
}
