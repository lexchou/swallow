
#include "type-alias.h"
#include "type-node.h"
USE_SWIFT_NS


TypeAlias::TypeAlias()
    :Declaration(1)
{
}
void TypeAlias::serialize(std::wostream& out)
{
    out<<L"typealias " << name << " = ";
    getType()->serialize(out);
}


const std::wstring& TypeAlias::getName()const
{
    return name;
}
void TypeAlias::setName(const std::wstring& name)
{
    this->name = name;
}

void TypeAlias::setType(TypeNode* type)
{
    set(0, type);
}
TypeNode* TypeAlias::getType()
{
    return static_cast<TypeNode*>(get(0));
}