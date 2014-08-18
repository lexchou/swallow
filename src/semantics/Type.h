#ifndef TYPE_H
#define TYPE_H
#include "swift_conf.h"
#include <memory>
#include <string>
#include "Symbol.h"
#include "swift_types.h"
#include "semantic-types.h"
#include <vector>
#include <map>

SWIFT_NS_BEGIN

typedef std::shared_ptr<class TypeDeclaration> TypeDeclarationPtr;
typedef std::weak_ptr<class TypeDeclaration> TypeDeclarationWeakPtr;

class Type : public Symbol
{
    friend class SymbolRegistry;
    friend class SymbolResolveAction;
public:
    typedef std::map<std::wstring, SymbolPtr> SymbolMap;
    struct Parameter
    {
        std::wstring name;
        bool inout;
        TypePtr type;
        Parameter(const std::wstring&name, bool inout, const TypePtr& type)
                :name(name), inout(inout), type(type)
        {}
        Parameter(const TypePtr& type)
                :inout(false), type(type)
        {}
    };
    enum Category
    {
        Aggregate,
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
        MetaType
    };
private:
    Type(const std::wstring& name, Category category, TypePtr keyType, TypePtr valueType);
public:
    static TypePtr newArrayType(TypePtr elementType);
    static TypePtr newDictionaryType(TypePtr keyType, TypePtr valueType);
    static TypePtr newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference = nullptr);
    static TypePtr newTuple(const std::vector<TypePtr>& types);
    static TypePtr newTypeReference(const TypePtr& innerType);
    static TypePtr newFunction(const std::vector<Parameter>& parameters, const TypePtr& returnType, bool hasVariadicParameters);
public:
    /**
     * Does the value copies by value or by reference
     */
    bool isValueType()const;

    /**
     * return true if the type is a class/struct/protocol/enum
     */
    bool isObjectType()const;

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
    TypeDeclarationPtr getReference()const;

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
    const std::vector<Parameter>& getParameters();

    /**
     * Check if the function has variadic parameters
     */
    bool hasVariadicParameters()const;


    /**
     * Returns the initializer of class/structure
     */
    FunctionOverloadedSymbolPtr getInitializer();

    /**
     * Return all internal symbols
     */
    SymbolMap& getSymbols();

public:
    bool operator ==(const Type& rhs)const;
    bool operator !=(const Type& rhs)const;

private:
    std::wstring name;
    std::wstring fullName;
    std::wstring moduleName;

    TypeDeclarationWeakPtr reference;

    Category category;
    TypePtr parentType;
    TypePtr keyType;
    TypePtr valueType;
    TypePtr innerType;
    TypePtr returnType;
    std::vector<Parameter> parameters;
    FunctionOverloadedSymbolPtr initializer;
    bool variadicParameters;
    std::vector<TypePtr> elementTypes;
    SymbolMap symbols;
};


SWIFT_NS_END


#endif//TYPE_H
