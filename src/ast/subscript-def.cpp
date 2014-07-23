
#include "subscript-def.h"
#include "type-node.h"
#include "code-block.h"
#include "parameters.h"
#include "node-visitor.h"
USE_SWIFT_NS


SubscriptDef::SubscriptDef()
    :Declaration(NodeType::Subscript), parameters(NULL), returnType(NULL), getter(NULL), setter(NULL)
{
}

SubscriptDef::~SubscriptDef()
{
    SafeDelete(parameters);
    SafeDelete(returnType);
    SafeDelete(getter);
    SafeDelete(setter);
    SafeDeleteAll(returnTypeAttributes);
}

void SubscriptDef::serialize(std::wostream& out)
{
//    out<<value;
}

void SubscriptDef::accept(NodeVisitor* visitor)
{
    visitor->visitSubscript(this);
}

void SubscriptDef::setParameters(Parameters* params)
{
    this->parameters = params;
}
Parameters* SubscriptDef::getParameters()
{
    return parameters;
}

void SubscriptDef::setReturnType(TypeNode* type)
{
    this->returnType = type;
}
TypeNode* SubscriptDef::getReturnType()
{
    return returnType;
}

void SubscriptDef::setReturnTypeAttributes(const Attributes& attrs)
{
    SafeDeleteAll(returnTypeAttributes);
    returnTypeAttributes = attrs;
}
const Attributes& SubscriptDef::getReturnTypeAttributes()const
{
    return returnTypeAttributes;
}

void SubscriptDef::setGetter(CodeBlock* getter)
{
    this->getter = getter;
}
CodeBlock* SubscriptDef::getGetter()
{
    return getter;
}

void SubscriptDef::setSetter(CodeBlock* setter)
{
    this->setter = setter;
}
CodeBlock* SubscriptDef::getSetter()
{
    return setter;
}

void SubscriptDef::setSetterName(const std::wstring& name)
{
    setterName = name;
}
const std::wstring& SubscriptDef::getSetterName()const
{
    return setterName;
}