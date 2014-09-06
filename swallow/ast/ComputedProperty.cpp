#include "ComputedProperty.h"
#include "NodeVisitor.h"
USE_SWIFT_NS


ComputedProperty::ComputedProperty()
:Declaration(NodeType::ComputedProperty)
{

}

ComputedProperty::~ComputedProperty()
{

}

void ComputedProperty::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitComputedProperty);
}


void ComputedProperty::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& ComputedProperty::getTypeAttributes() const
{
    return typeAttributes;
}
void ComputedProperty::setDeclaredType(const TypeNodePtr& t)
{
    this->declaredType = t;
}
TypeNodePtr ComputedProperty::getDeclaredType()
{
    return declaredType;
}

const TypePtr& ComputedProperty::getType()const
{
    return type;
}

void ComputedProperty::setType(const TypePtr& type)
{
    this->type = type;
}

const std::wstring& ComputedProperty::getName()const
{
    return name;
}
void ComputedProperty::setName(const std::wstring& name)
{
    this->name = name;
}

void ComputedProperty::setInitializer(const ExpressionPtr& initializer)
{
    this->initializer = initializer;
}
const ExpressionPtr& ComputedProperty::getInitializer()const
{
    return initializer;
}

void ComputedProperty::setSetter(const CodeBlockPtr& setter)
{
    this->setter = setter;
}
CodeBlockPtr ComputedProperty::getSetter()
{
    return setter;
}

void ComputedProperty::setSetterName(const std::wstring& name)
{
    setterName = name;
}
const std::wstring& ComputedProperty::getSetterName()
{
    return setterName;
}

void ComputedProperty::setGetter(const CodeBlockPtr& getter)
{
    this->getter = getter;
}
CodeBlockPtr ComputedProperty::getGetter()
{
    return getter;
}


void ComputedProperty::setWillSet(const CodeBlockPtr& willSet)
{
    this->willSet = willSet;
}
CodeBlockPtr ComputedProperty::getWillSet()
{
    return willSet;
}

void ComputedProperty::setDidSet(const CodeBlockPtr& didSet)
{
    this->didSet = didSet;
}
CodeBlockPtr ComputedProperty::getDidSet()
{
    return didSet;
}


void ComputedProperty::setWillSetSetter(const std::wstring& name)
{
    willSetSetter = name;
}
const std::wstring& ComputedProperty::getWillSetSetter()const
{
    return willSetSetter;
}


void ComputedProperty::setDidSetSetter(const std::wstring& name)
{
    didSetSetter = name;
}
const std::wstring& ComputedProperty::getDidSetSetter()const
{
    return didSetSetter;
}


