
#include "variable.h"
#include "expression.h"
#include "code-block.h"
#include "type-node.h"
#include "pattern.h"
USE_SWIFT_NS


Variable::Variable()
    :Declaration(NodeType::Variable), name(NULL), declaredType(NULL), initializer(NULL), getter(NULL), setter(NULL), willSet(NULL), didSet(NULL), type(NULL)
{
}

Variable::~Variable()
{
    SafeDeleteAll(typeAttributes);
    SafeDelete(name);
    SafeDelete(declaredType);
    SafeDelete(initializer);
    SafeDelete(getter);
    SafeDelete(setter);
    SafeDelete(willSet);
    SafeDelete(didSet);
}
void Variable::serialize(std::wostream& out)
{
}

TypePtr Variable::getType()
{
    return type;
}
void Variable::setType(TypePtr type)
{
    this->type = type;
}

void Variable::setName(Pattern* pattern)
{
    this->name = pattern;
}
Pattern* Variable::getName()
{
    return name;
}

void Variable::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& Variable::getTypeAttributes() const
{
    return typeAttributes;
}
void Variable::setDeclaredType(TypeNode* t)
{
    this->declaredType = t;
}
TypeNode* Variable::getDeclaredType()
{
    return declaredType;
}
void Variable::setInitializer(Expression* expr)
{
    this->initializer = expr;
}
Expression* Variable::getInitializer()
{
    return initializer;
}

void Variable::setSetter(CodeBlock* setter)
{
    this->setter = setter;
}
CodeBlock* Variable::getSetter()
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

void Variable::setGetter(CodeBlock* getter)
{
    this->getter = getter;
}
CodeBlock* Variable::getGetter()
{
    return getter;
}


void Variable::setWillSet(CodeBlock* willSet)
{
    this->willSet = willSet;
}
CodeBlock* Variable::getWillSet()
{
    return willSet;
}

void Variable::setDidSet(CodeBlock* didSet)
{
    this->didSet = didSet;
}
CodeBlock* Variable::getDidSet()
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



