
#include "variable.h"
#include "expression.h"
#include "code-block.h"
#include "type-node.h"
#include "pattern.h"
USE_SWIFT_NS


Variable::Variable()
    :Declaration(7)
{
}

Variable::~Variable()
{
    Attribute::disposeAll(typeAttributes);
}
void Variable::serialize(std::wostream& out)
{
}


void Variable::setName(Pattern* pattern)
{
    set(0, pattern);
}
Pattern* Variable::getName()
{
    return static_cast<Pattern*>(get(0));
}

void Variable::setTypeAttributes(const Attributes& attrs)
{
    typeAttributes = attrs;
}
const Attributes& Variable::getTypeAttributes() const
{
    return typeAttributes;
}
void Variable::setType(TypeNode* t)
{
    set(1, t);
}
TypeNode* Variable::getType()
{
    return static_cast<TypeNode*>(get(1));
}
void Variable::setInitializer(Expression* expr)
{
    set(2, expr);
}
Expression* Variable::getInitializer()
{
    return static_cast<Expression*>(get(2));
}

void Variable::setSetter(CodeBlock* setter)
{
    set(3, setter);
}
CodeBlock* Variable::getSetter()
{
    return static_cast<CodeBlock*>(get(3));
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
    set(4, getter);
}
CodeBlock* Variable::getGetter()
{
    return static_cast<CodeBlock*>(get(4));
}


void Variable::setWillSet(CodeBlock* willSet)
{
    set(5, willSet);
}
CodeBlock* Variable::getWillSet()
{
    return static_cast<CodeBlock*>(get(5));
}

void Variable::setDidSet(CodeBlock* didSet)
{
    set(6, didSet);
}
CodeBlock* Variable::getDidSet()
{
    return static_cast<CodeBlock*>(get(6));
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



