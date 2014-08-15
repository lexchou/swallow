#include "function-def.h"
#include <ast/node-visitor.h>
USE_SWIFT_NS


FunctionDef::FunctionDef()
    :Declaration(NodeType::Function), returnType(NULL), body(NULL)
{
}

FunctionDef::~FunctionDef()
{

}

void FunctionDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitFunction);
}

void FunctionDef::setName(const std::wstring& name)
{
    this->name = name;
}
const std::wstring& FunctionDef::getName()const
{
    return name;
}




void FunctionDef::addParameters(const ParametersPtr& parameters)
{
    parametersList.push_back(parameters);
}
int FunctionDef::numParameters()
{
    return parametersList.size();
}
ParametersPtr FunctionDef::getParameters(int i)
{
    return parametersList[i];
}

void FunctionDef::setReturnType(const TypeNodePtr& type)
{
    returnType = type;
}
TypeNodePtr FunctionDef::getReturnType()
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

void FunctionDef::setBody(const CodeBlockPtr& body)
{
    this->body = body;
}
CodeBlockPtr FunctionDef::getBody()
{
    return body;
}