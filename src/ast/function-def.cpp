
#include "function-def.h"
#include "code-block.h"
#include "type-node.h"
#include "parameters.h"
USE_SWIFT_NS


FunctionDef::FunctionDef()
    :Declaration(NodeType::Function), returnType(NULL), body(NULL)
{
}

FunctionDef::~FunctionDef()
{
    SafeDeleteAll(returnAttributes);
    SafeDeleteAll(parametersList);
    SafeDelete(returnType);
    SafeDelete(body);
    
}

void FunctionDef::serialize(std::wostream& out)
{
}

void FunctionDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& FunctionDef::getName()const
{
    return name;
}




void FunctionDef::addParameters(Parameters* parameters)
{
    parametersList.push_back(parameters);
}
int FunctionDef::numParameters()
{
    return parametersList.size();
}
Parameters* FunctionDef::getParameters(int i)
{
    return parametersList[i];
}

void FunctionDef::setReturnType(TypeNode* type)
{
    returnType = type;
}
TypeNode* FunctionDef::getReturnType()
{
    return returnType;
}

void FunctionDef::setReturnTypeAttributes(const Attributes& attrs)
{
    returnAttributes = attrs;
}
const Attributes& FunctionDef::getReturnTypeAttributes()const
{
    return returnAttributes;
}

void FunctionDef::setBody(CodeBlock* body)
{
    this->body = body;
}
CodeBlock* FunctionDef::getBody()
{
    return body;
}