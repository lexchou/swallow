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
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
typedef std::shared_ptr<class GenericArgument> GenericArgumentPtr;
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
        Class,
        Struct,
        Specialized,
        Enum,
        Protocol,
        Extension,
        Function,
        Closure,
        MetaType,
        GenericParameter,//Placeholder for generic type
        Self// A fake place holder, protocol use this type to present the final type that conform to the protocol
    };
private:
    Type(Category category);
public:
    static TypePtr newType(const std::wstring& name, Category category, const TypeDeclarationPtr& reference = nullptr, const TypePtr& parentType = nullptr, const std::vector<TypePtr>& protocols = std::vector<TypePtr>(), const GenericDefinitionPtr& generic = nullptr);
    static TypePtr newTuple(const std::vector<TypePtr>& types);
    static TypePtr newTypeReference(const TypePtr& innerType);
    static TypePtr newFunction(const std::vector<Parameter>& parameters, const TypePtr& returnType, bool hasVariadicParameters, const GenericDefinitionPtr& generic = nullptr);
    static TypePtr newSpecializedType(const TypePtr& innerType, const GenericArgumentPtr& arguments);
    static TypePtr newSpecializedType(const TypePtr& innerType, const TypePtr& argument);
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
    void setParentType(const TypePtr& type);
    /**
     * Gets the protocols that this type conform to
     */
    const std::vector<TypePtr>& getProtocols() const;

    /**
     * Adds a protocol that this type conform to
     */
    void addProtocol(const TypePtr& protocol);

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
     * Returns the subscript getters defined by this type
     */
    FunctionOverloadedSymbolPtr getSubscript();

    /**
     * Check if this is a generic type
     */
    bool isGenericType()const;

    /*
     * Gets the generic arguments that used to specialize a concrete type
     */
    const GenericArgumentPtr& getGenericArguments() const;

    /**
     * Gets the generic definition
     */
    const GenericDefinitionPtr& getGenericDefinition() const;

    /**
     * Check if current type is conform to the given protocol or it is a child class of given class
     */
    bool isKindOf(const TypePtr& protocolOrBase) const;

    TypePtr getType() override;

    /**
     * Check if the definition of this type contains a Self type
     */
    bool containsSelfType() const;

    /**
     * Check if the definition of this type contains an associated type
     */
    bool containsAssociatedType() const;

    /**
     * Check if this type's generic arguments contains generic parameters
     */
    bool containsGenericParameters() const;

    /**
     * convert this type to string representation
     */
    std::wstring toString() const;
public://member access
    void addMember(const std::wstring& name, const SymbolPtr& member);
    void addMember(const SymbolPtr& symbol);
    SymbolPtr getMember(const std::wstring& name) const;
    SymbolPtr getDeclaredMember(const std::wstring& name) const;
    const SymbolMap& getDeclaredMembers() const;

    TypePtr getAssociatedType(const std::wstring& name) const;
    TypePtr getDeclaredAssociatedType(const std::wstring& name) const;

    const std::vector<SymbolPtr>& getDeclaredStoredProperties() const;
public:
    bool operator ==(const Type& rhs)const;
    bool operator !=(const Type& rhs)const;
private:
    void addParentTypesFrom(const TypePtr& type);
    void addParentType(const TypePtr& type, int distance);

    /**
    * Check if the definition of this type contains a Self type
    */
    bool containsSelfTypeImpl() const;
private:
    std::wstring name;
    std::wstring fullName;
    std::wstring moduleName;

    TypeDeclarationWeakPtr reference;

    Category category;
    TypePtr parentType;//The direct inherited parent type
    std::vector<TypePtr> protocols; //Protocols that this type directly conform to
    std::map<TypePtr, int> parents;//All parent types and protocols in inheritance tree
    GenericDefinitionPtr genericDefinition;
    GenericArgumentPtr genericArguments;
    TypePtr innerType;
    TypePtr returnType;
    std::vector<Parameter> parameters;
    FunctionOverloadedSymbolPtr initializer;
    bool variadicParameters;
    std::vector<TypePtr> elementTypes;

    //for custom type
    SymbolMap symbols;
    std::vector<SymbolPtr> storedProperties;

    //for protocol
    mutable short _containsSelfType;
    mutable short _containsAssociatedType;

    int inheritantDepth;
};


SWIFT_NS_END


#endif//TYPE_H
