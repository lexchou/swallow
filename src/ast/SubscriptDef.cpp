#include "SubscriptDef.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


SubscriptDef::SubscriptDef()
    :Declaration(NodeType::Subscript), parameters(NULL), returnType(NULL), getter(NULL), setter(NULL)
{
}

SubscriptDef::~SubscriptDef()
{

}

void SubscriptDef::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitSubscript);
}

void SubscriptDef::setParameters(const ParametersPtr& params)
{
    this->parameters = params;
}
ParametersPtr SubscriptDef::getParameters()
{
    return parameters;
}

void SubscriptDef::setReturnType(const TypeNodePtr& type)
{
    this->returnType = type;
}
TypeNodePtr SubscriptDef::getReturnType()
{
    return returnType;
}

void SubscriptDef::setReturnTypeAttributes(const Attributes& attrs)
{
    returnTypeAttributes = attrs;
}
const Attributes& SubscriptDef::getReturnTypeAttributes()const
{
    return returnTypeAttributes;
}

void SubscriptDef::setGetter(const CodeBlockPtr& getter)
{
    this->getter = getter;
}
CodeBlockPtr SubscriptDef::getGetter()
{
    return getter;
}

void SubscriptDef::setSetter(const CodeBlockPtr& setter)
{
    this->setter = setter;
}
CodeBlockPtr SubscriptDef::getSetter()
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
