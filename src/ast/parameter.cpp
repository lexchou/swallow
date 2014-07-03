
#include "parameter.h"
#include "expression.h"
#include "type-node.h"
USE_SWIFT_NS


Parameter::Parameter()
    :Node(2)
{
    inout = false;
    accessibility = None;
    shorthandExternalName = false;
}
Parameter::~Parameter()
{
    Attribute::disposeAll(typeAttributes);
}
void Parameter::serialize(std::wostream& out)
{
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

void Parameter::setType(TypeNode* type)
{
    set(0, type);
}
TypeNode* Parameter::getType()
{
    return static_cast<TypeNode*>(get(0));
}
void Parameter::setDefaultValue(Expression* def)
{
    set(1, def);
}
Expression* Parameter::getDefaultValue()
{
    return static_cast<Expression*>(get(1));
}