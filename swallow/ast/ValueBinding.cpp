#include "ValueBinding.h"
#include "NodeVisitor.h"
USE_SWIFT_NS

ValueBinding::ValueBinding()
    :Declaration(NodeType::ValueBinding)
{
}

void ValueBinding::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitValueBinding);
}

void ValueBinding::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& ValueBinding::getTypeAttributes() const
{
    return typeAttributes;
}
void ValueBinding::setDeclaredType(const TypeNodePtr& t)
{
    this->declaredType = t;
}
TypeNodePtr ValueBinding::getDeclaredType()
{
    return declaredType;
}

const TypePtr& ValueBinding::getType()const
{
    return type;
}

void ValueBinding::setType(const TypePtr& type)
{
    this->type = type;
}

const PatternPtr& ValueBinding::getName()const
{
    return name;
}
void ValueBinding::setName(const PatternPtr& name)
{
    this->name = name;
}

void ValueBinding::setInitializer(const ExpressionPtr& initializer)
{
    this->initializer = initializer;
}
const ExpressionPtr& ValueBinding::getInitializer()const
{
    return initializer;
}

std::shared_ptr<class ValueBindings> ValueBinding::getOwner() const
{
    return owner.lock();
}