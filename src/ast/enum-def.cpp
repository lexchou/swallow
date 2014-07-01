
#include "enum-def.h"
USE_SWIFT_NS


EnumDef::EnumDef()
{
}
void EnumDef::serialize(std::wostream& out)
{
}

EnumDef::Type EnumDef::getType()const
{
    if(numParents() > 0)
    {
        return RawValue;
    }
    return UnionStyle;
}

void EnumDef::addAssociatedType(const std::wstring& name, TupleType* associatedType)
{
    associatedTypes.push_back(AssociatedType(name, associatedType));
}
int EnumDef::numAssociatedTypes()const
{
    return associatedTypes.size();
}
const EnumDef::AssociatedType& EnumDef::getAssociatedType(int i)
{
    return associatedTypes[i];
}

void EnumDef::addConstant(const std::wstring& name, Expression* value)
{
    constants.push_back(Constant(name, value));
}
int EnumDef::numConstants()
{
    return constants.size();
}
const EnumDef::Constant& EnumDef::getConstant(int i)
{
    return constants[i];
}
