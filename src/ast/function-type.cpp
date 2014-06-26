
#include "function-type.h"
USE_SWIFT_NS


FunctionType::FunctionType()
    :TypeNode(2)
{
}
void FunctionType::serialize(std::wostream& out)
{
    getArgumentsType()->serialize(out);
    out<<L" -> ";
    getReturnType()->serialize(out);
}

void FunctionType::setArgumentsType(TypeNode* argumentsType)
{
    set(0, argumentsType);
}

TypeNode* FunctionType::getArgumentsType()
{
    return static_cast<TypeNode*>(get(0));
}
void FunctionType::setReturnType(TypeNode* retType)
{
    set(1, retType);
}


TypeNode* FunctionType::getReturnType()
{
    return static_cast<TypeNode*>(get(1));
}
