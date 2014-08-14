
#include "variable.h"
#include "expression.h"
#include "code-block.h"
#include "type-node.h"
#include "pattern.h"
#include "node-visitor.h"
USE_SWIFT_NS


Variable::Variable()
    :ValueBinding(NodeType::Variable)
{
}

Variable::~Variable()
{

}
void Variable::serialize(std::wostream& out)
{
}
void Variable::accept(NodeVisitor* visitor)
{
    accept2(visitor, &NodeVisitor::visitVariable);
}


void Variable::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& Variable::getTypeAttributes() const
{
    return typeAttributes;
}
void Variable::setDeclaredType(const TypeNodePtr& t)
{
    this->declaredType = t;
}
TypeNodePtr Variable::getDeclaredType()
{
    return declaredType;
}


void Variable::setSetter(const CodeBlockPtr& setter)
{
    this->setter = setter;
}
CodeBlockPtr Variable::getSetter()
{
    return setter;
}

void Variable::setSetterName(const std::wstring& name)
{
    setterName = name;
}
const std::wstring& Variable::getSetterName()
{
    return setterName;
}

void Variable::setGetter(const CodeBlockPtr& getter)
{
    this->getter = getter;
}
CodeBlockPtr Variable::getGetter()
{
    return getter;
}


void Variable::setWillSet(const CodeBlockPtr& willSet)
{
    this->willSet = willSet;
}
CodeBlockPtr Variable::getWillSet()
{
    return willSet;
}

void Variable::setDidSet(const CodeBlockPtr& didSet)
{
    this->didSet = didSet;
}
CodeBlockPtr Variable::getDidSet()
{
    return didSet;
}


void Variable::setWillSetSetter(const std::wstring& name)
{
    willSetSetter = name;
}
const std::wstring& Variable::getWillSetSetter()const
{
    return willSetSetter;
}


void Variable::setDidSetSetter(const std::wstring& name)
{
    didSetSetter = name;
}
const std::wstring& Variable::getDidSetSetter()const
{
    return didSetSetter;
}



