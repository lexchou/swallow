#include "parameter.h"
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

void Parameter::setType(const TypePtr& type)
{
    this->type = type;
}
TypePtr Parameter::getType()
{
    return type;
}
void Parameter::setDeclaredType(const TypeNodePtr& type)
{
    this->declaredType = type;
}
TypeNodePtr Parameter::getDeclaredType()
{
    return declaredType;
}
void Parameter::setDefaultValue(const ExpressionPtr& def)
{
    this->defaultValue = def;
}
ExpressionPtr Parameter::getDefaultValue()
{
    return defaultValue;
}