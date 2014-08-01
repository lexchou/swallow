#ifndef TYPE_H
#define TYPE_H
#include "swift_conf.h"
#include <memory>
#include <string>
#include "symbol.h"
#include "swift_types.h"
#include "semantic-types.h"
#include <vector>
SWIFT_NS_BEGIN


class Type : public Symbol
{
    friend class SymbolRegistry;
public:
    enum Category
    {
        Primitive,
        Tuple,
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
    static TypePtr newTuple(const std::wstring& name, const std::vector<TypePtr>& types);
public:
    bool isPrimitive()const;
    bool isArray()const;
    bool isDictionary() const;
    bool isTuple()const;

    /**
     * Does the value copies by value or by reference
     */
    bool isValueType()const;

    /**
     * Gets the name of this type
     * e.g. SubClass
     */
    const std::wstring& getName()const;

    /**
     * Gets the full qualified name of the type
     * e.g.  com.carbinch.demo.MyClass.SubClass
     */
    const std::wstring& getFullName() const;

    /**
     * Gets the parent type that defined this type
     */
    TypePtr getParentType();

    /**
     * Gets the module name where defined this type
     * e.g.  com.carbinch.demo
     */
    const std::wstring& getModuleName() const;

    /**
     * Gets the element type of the Array type
     */
    TypePtr getElementType();

    /**
     * Gets the element type of the tuple type
     */
    TypePtr getElementType(int index);

    /**
     * Gets the number of element types of the tuple type
     */
    int numElementTypes()const;

    /**
     * Gets the key type of the dictionary type
     */
    TypePtr getKeyType();

    /**
     * Gets the value type of the dictionary type
     */
    TypePtr getValueType();
public:
    bool operator ==(const Type& rhs)const;
    bool operator !=(const Type& rhs)const;

private:
    std::wstring name;
    std::wstring fullName;
    std::wstring moduleName;

    Category category;
    TypePtr parentType;
    TypePtr keyType;
    TypePtr valueType;
    std::vector<TypePtr> elementTypes;
};


SWIFT_NS_END


#endif//TYPE_H