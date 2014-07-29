#ifndef TYPE_H
#define TYPE_H
#include "swift_conf.h"
#include <memory>
#include <string>
#include "symbol.h"
#include "swift_types.h"
#include "semantic-types.h"
SWIFT_NS_BEGIN


class Type : public Symbol
{
    friend class SymbolRegistry;
public:
    enum Category
    {
        Primitive,
        Array,
        Dictionary,
        Class,
        Struct,
        Enum,
        Protocol,
        Extension
    };
private:
    Type(const std::wstring& name, Category category, TypePtr keyType, TypePtr valueType);
public:
    static TypePtr newArrayType(const std::wstring& name, TypePtr elementType);
    static TypePtr newDictionaryType(const std::wstring& name, TypePtr keyType, TypePtr valueType);
    static TypePtr newType(const std::wstring& name, Category category);
public:
    bool isPrimitive()const;
    bool isArray();
    const std::wstring& getName()const;
    TypePtr getElementType();

private:
    std::wstring name;
    Category category;
    TypePtr keyType;
    TypePtr valueType;
};

SWIFT_NS_END


#endif//TYPE_H