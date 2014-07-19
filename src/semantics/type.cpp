#include "type.h"

USE_SWIFT_NS


Type::Type(const std::wstring& name, Category category, TypePtr keyType, TypePtr valueType)
:name(name), category(category), keyType(keyType), valueType(valueType)
{

}



Type* Type::newDictionaryType(const std::wstring& name, TypePtr keyType, TypePtr valueType)
{
    return new Type(name, Dictionary, keyType, valueType);
}
Type* Type::newType(const std::wstring& name, Category category)
{
    return new Type(name, category, nullptr, nullptr);
}


Type* Type::newArrayType(const std::wstring& name, TypePtr elementType)
{
    return new Type(name, Array, nullptr, elementType);
}

bool Type::isPrimitive()const
{
    return category == Primitive;
}
bool Type::isArray()
{
    return category == Array;
}
const std::wstring& Type::getName()const
{
    return name;
}
TypePtr Type::getElementType()
{
    return valueType;
}