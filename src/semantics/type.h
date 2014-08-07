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

typedef std::shared_ptr<class TypeDeclaration> TypeDeclarationPtr;
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
        Extension,
        Function,
        Closure,
        Reference
    };
private:
    Type(const std::wstring& name, Category category, TypePtr keyType, TypePtr valueType);
public:
    static TypePtr newArrayType(TypePtr elementType);
    static TypePtr newDictionaryType(TypePtr keyType, TypePtr valueType);
    static TypePtr newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference = nullptr);
    static TypePtr newTuple(const std::vector<TypePtr>& types);
    static TypePtr newTypeReference(const TypePtr& innerType);
    static TypePtr newFunction(const std::vector<TypePtr>& parameters, const TypePtr& returnType);
public:
    bool isPrimitive()const;
    bool isArray()const;
    bool isDictionary() const;
    bool isTuple()const;

    /**
     * Does the value copies by value or by reference
     */
    bool isValueType()const;
    Category getCategory()const;

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

    /**
     * Gets which declaration this type referenced to
     */
    TypeDeclarationPtr getReference();

    /**
     * Gets the inner type for Reference category.
     */
    TypePtr getInnerType();

    /**
     * Gets the return type if it's a function/closure type
     */
    TypePtr getReturnType();

    /**
     * Gets the parameter types if it's a function/closure type
     */
    const std::vector<TypePtr>& getParameterTypes();
public:
    bool operator ==(const Type& rhs)const;
    bool operator !=(const Type& rhs)const;

private:
    std::wstring name;
    std::wstring fullName;
    std::wstring moduleName;

    TypeDeclarationPtr reference;

    Category category;
    TypePtr parentType;
    TypePtr keyType;
    TypePtr valueType;
    TypePtr innerType;
    TypePtr returnType;
    std::vector<TypePtr> parameterTypes;
    std::vector<TypePtr> elementTypes;
};


SWIFT_NS_END


#endif//TYPE_H