
#include "function-def.h"
#include "code-block.h"
#include "type-node.h"
#include "parameters.h"
USE_SWIFT_NS


FunctionDef::FunctionDef()
    :Declaration(2)
{
}

FunctionDef::~FunctionDef()
{
    Attribute::disposeAll(returnAttributes);
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
    children.push_back(parameters);
}
int FunctionDef::numParameters()
{
    return numChildren() - 2;
}
Parameters* FunctionDef::getParameters(int i)
{
    return static_cast<Parameters*>(get(i + 2));
}

void FunctionDef::setReturnType(TypeNode* type)
{
    set(0, type);
}
TypeNode* FunctionDef::getReturnType()
{
    return static_cast<TypeNode*>(get(0));
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
    set(1, body);
}
CodeBlock* FunctionDef::getBody()
{
    return static_cast<CodeBlock*>(get(1));
}