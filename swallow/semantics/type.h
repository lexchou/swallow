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

class Type : public Symbol, public  std::enable_shared_from_this<Symbol>
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
        MetaType,
        Placeholder
    };
private:
    Type(const std::wstring& name, Category category, TypePtr keyType, TypePtr valueType);
public:
    static TypePtr newArrayType(TypePtr elementType);
    static TypePtr newDictionaryType(TypePtr keyType, TypePtr valueType);
    static TypePtr newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference = nullptr, const TypePtr& parentType = nullptr, const std::vector<TypePtr>& protocols = std::vector<TypePtr>());
    static TypePtr newTuple(const std::vector<TypePtr>& types);
    static TypePtr newTypeReference(const TypePtr& innerType);
    static TypePtr newFunction(const std::vector<Parameter>& parameters, const TypePtr& returnType, bool hasVariadicParameters);

    /**
     * A type place holder for protocol's typealias
     */
    static const TypePtr& getPlaceHolder();

    static bool equals(const TypePtr& lhs, const TypePtr& rhs);
public://methods
    /**
     * Gets the common parent class between current class and rhs with the minimum inheritance distance.
     */
    TypePtr getCommonParent(const TypePtr& rhs);
public://properties
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
    TypePtr getParentType()const;
    /**
     * Gets the protocols that this type conform to
     */
    const std::vector<TypePtr>& getProtocols()const;

    /**
     * Gets the module name where defined this type
     * e.g.  com.carbinch.demo
     */
    const std::wstring& getModuleName() const;

    /**
     * Gets the element type of the Array type
     */
    TypePtr getElementType() const;

    /**
     * Gets the element type of the tuple type
     */
    TypePtr getElementType(int index) const;

    /**
     * Gets the number of element types of the tuple type
     */
    int numElementTypes()const;

    /**
     * Gets the key type of the dictionary type
     */
    TypePtr getKeyType() const;

    /**
     * Gets the value type of the dictionary type
     */
    TypePtr getValueType() const;

    /**
     * Gets which declaration this type referenced to
     */
    TypeDeclarationPtr getReference()const;

    /**
     * Gets the inner type for Reference category.
     */
    TypePtr getInnerType() const;

    /**
     * Gets the return type if it's a function/closure type
     */
    TypePtr getReturnType() const;

    /**
     * Gets the parameter types if it's a function/closure type
     */
    const std::vector<Parameter>& getParameters() const;

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
    std::vector<TypePtr> protocols;

    TypePtr keyType;
    TypePtr valueType;
    TypePtr innerType;
    TypePtr returnType;
    std::vector<Parameter> parameters;
    FunctionOverloadedSymbolPtr initializer;
    bool variadicParameters;
    std::vector<TypePtr> elementTypes;
    SymbolMap symbols;

    int inheritantDepth;
};


SWIFT_NS_END


#endif//TYPE_H
