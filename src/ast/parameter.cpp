
#include "parameter.h"
#include "expression.h"
#include "type-node.h"
#include "node-visitor.h"
USE_SWIFT_NS


Parameter::Parameter()
:Node(NodeType::Parameter)
{
    inout = false;
    accessibility = None;
    shorthandExternalName = false;
    type = NULL;
    defaultValue = NULL;
}
Parameter::~Parameter()
{

}
void Parameter::serialize(std::wostream& out)
{
}
void Parameter::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitParameter);

}

void Parameter::setInout(bool inout)
{
    this->inout = inout;
}
bool Parameter::isInout()const
{
    return inout;
}

void Parameter::setAccessibility(Accessibility accessibility)
{
    this->accessibility = accessibility;
}
Parameter::Accessibility Parameter::getAccessibility() const
{
    return accessibility;
}

void Parameter::setExternalName(const std::wstring& name)
{
    this->externalName = name;
}
const std::wstring& Parameter::getExternalName()const
{
    return externalName;
}

void Parameter::setLocalName(const std::wstring& name)
{
    localName = name;
}
const std::wstring& Parameter::getLocalName()const
{
    return localName;
}

void Parameter::setShorthandExternalName(bool shorthandExternalName)
{
    this->shorthandExternalName = shorthandExternalName;
}
bool Parameter::isShorthandExternalName()const
{
    return shorthandExternalName;
}

void Parameter::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& Parameter::getTypeAttributes()const
{
    return typeAttributes;
}

void Parameter::setType(const TypeNodePtr& type)
{
    this->type = type;
}
TypeNodePtr Parameter::getType()
{
    return type;
}
void Parameter::setDefaultValue(const ExpressionPtr& def)
{
    this->defaultValue = def;
}
ExpressionPtr Parameter::getDefaultValue()
{
    return defaultValue;
}