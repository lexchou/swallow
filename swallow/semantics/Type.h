/* Type.h --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef TYPE_H
#define TYPE_H
#include "swallow_conf.h"
#include <memory>
#include <string>
#include "Symbol.h"
#include "swallow_types.h"
#include "semantic-types.h"
#include <vector>
#include <map>

SWALLOW_NS_BEGIN

typedef std::shared_ptr<class TypeDeclaration> TypeDeclarationPtr;
typedef std::weak_ptr<class TypeDeclaration> TypeDeclarationWeakPtr;
typedef std::shared_ptr<class GenericDefinition> GenericDefinitionPtr;
typedef std::shared_ptr<class GenericArgument> GenericArgumentPtr;
typedef std::shared_ptr<class FunctionOverloadedSymbol> FunctionOverloadedSymbolPtr;
class SWALLOW_EXPORT Type : public Symbol, public  std::enable_shared_from_this<Symbol>
{
    friend class TypeBuilder;
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
        Alias,// Alias for another type
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
    /**
     * Gets the protocols that this type conform to
     */
    const std::vector<TypePtr>& getProtocols() const;


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
     * Gets the number of total enum cases
     */
    int numEnumCases()const;

    /**
     * Gets which declaration this type referenced to
     */
    TypeDeclarationPtr getReference()const;

    /**
     * Gets the inner type for Specialized type
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

    /**
     * Unwrap the alias chain, return the last node of the alias chain
     */
    TypePtr unwrap() const;
public://member access
    SymbolPtr getDeclaredStaticMember(const std::wstring& name)const;
    SymbolPtr getMember(const std::wstring& name) const;
    SymbolPtr getDeclaredMember(const std::wstring& name) const;
    const SymbolMap& getDeclaredMembers() const;

    TypePtr getAssociatedType(const std::wstring& name) const;
    TypePtr getDeclaredAssociatedType(const std::wstring& name) const;
    const std::map<std::wstring, TypePtr> getAssociatedTypes() const;
    const std::vector<SymbolPtr>& getDeclaredStoredProperties() const;
    const std::vector<FunctionOverloadedSymbolPtr>& getDeclaredFunctions() const;

    /**
     * Check if current type can be specialized to given type
     */
    bool canSpecializeTo(const TypePtr& type, std::map<std::wstring, TypePtr>& typeMap)const;

    /**
     * Check if an instance of current type can be assigned to a variable with given type
     */
    bool canAssignTo(const TypePtr& type)const;

    const std::map<TypePtr, int>& getAllParents() const;

    const std::vector<FunctionOverloadedSymbolPtr>& getFunctions() const;

public:
    bool operator ==(const Type& rhs)const;
    bool operator !=(const Type& rhs)const;
private:

    /**
    * Check if the definition of this type contains a Self type
    */
    bool containsSelfTypeImpl() const;
protected:
    std::wstring name;
    std::wstring fullName;
    std::wstring moduleName;

    TypeDeclarationWeakPtr reference;

    Category category;

    //for function and custom type
    GenericDefinitionPtr genericDefinition;

    //for specialized type
    TypePtr innerType;
    GenericArgumentPtr genericArguments;

    //for enum type
    int enumCases;


    //for function type
    TypePtr returnType;
    std::vector<Parameter> parameters;
    bool variadicParameters;

    //for tuple type
    std::vector<TypePtr> elementTypes;

    //for custom type
    TypePtr parentType;//The direct inherited parent type
    std::vector<TypePtr> protocols; //Protocols that this type directly conform to
    std::map<TypePtr, int> parents;//All parent types and protocols in inheritance tree
    SymbolMap members;
    SymbolMap staticMembers;
    FunctionOverloadedSymbolPtr initializer;
    std::vector<SymbolPtr> storedProperties;
    std::vector<SymbolPlaceHolderPtr> computedProperties;
    std::map<std::wstring, TypePtr> associatedTypes;
    std::vector<FunctionOverloadedSymbolPtr> functions;
    int inheritantDepth;

    //for protocol
    mutable short _containsSelfType;

};


SWALLOW_NS_END


#endif//TYPE_H
