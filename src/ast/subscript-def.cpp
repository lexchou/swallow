
#include "subscript-def.h"
#include "type-node.h"
#include "code-block.h"
#include "parameters.h"
USE_SWIFT_NS


SubscriptDef::SubscriptDef()
    :Declaration(4)
{
}
void SubscriptDef::serialize(std::wostream& out)
{
//    out<<value;
}


void SubscriptDef::setParameters(Parameters* params)
{
    set(0, params);
}
Parameters* SubscriptDef::getParameters()
{
    return static_cast<Parameters*>(get(0));
}

void SubscriptDef::setReturnType(TypeNode* type)
{
    set(1, type);
}
TypeNode* SubscriptDef::getReturnType()
{
    return static_cast<TypeNode*>(get(1));
}

void SubscriptDef::setReturnTypeAttributes(const Attributes& attrs)
{
    returnTypeAttributes = attrs;
}
const Attributes& SubscriptDef::getReturnTypeAttributes()const
{
    return returnTypeAttributes;
}

void SubscriptDef::setGetter(CodeBlock* getter)
{
    set(2, getter);
}
CodeBlock* SubscriptDef::getGetter()
{
    return static_cast<CodeBlock*>(get(2));
}

void SubscriptDef::setSetter(CodeBlock* setter)
{
    set(3, setter);
}
CodeBlock* SubscriptDef::getSetter()
{
    return static_cast<CodeBlock*>(get(3));
}

void SubscriptDef::setSetterName(const std::wstring& name)
{
    setterName = name;
}
const std::wstring& SubscriptDef::getSetterName()const
{
    return setterName;
}