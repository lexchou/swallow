/* SemanticAnalyzer_TypeCreation.cpp --
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
#include "SemanticAnalyzer.h"
#include "ast/ast.h"
#include "SymbolRegistry.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "common/Errors.h"
#include "ast/NodeSerializer.h"
#include "TypeBuilder.h"
#include "GlobalScope.h"
#include <cassert>
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"

USE_SWALLOW_NS
using namespace std;
static bool isLiteralTypeForEnum(GlobalScope* global, const TypePtr& type)
{
    if(type->canAssignTo(global->IntegerLiteralConvertible()))
        return true;
    if(type->canAssignTo(global->StringLiteralConvertible()))
        return true;
    if(type->canAssignTo(global->FloatLiteralConvertible()))
        return true;
    //if(type->canAssignTo(global->BooleanLiteralConvertible))
    //    return true;
    if(type->canAssignTo(global->UnicodeScalarLiteralConvertible()))
        return true;
    if(type->canAssignTo(global->ExtendedGraphemeClusterLiteralConvertible()))
        return true;
    //nil, array, dictionary literal is not working for enum
    return false;
}
static bool isLiteralExpression(const ExpressionPtr& expr)
{
    if(!expr)
        return false;
    NodeType::T nodeType = expr->getNodeType();
    switch(nodeType)
    {
        case NodeType::StringLiteral:
        case NodeType::FloatLiteral:
        case NodeType::IntegerLiteral:
            return true;
        default:
            break;
    }
    return false;
}
TypePtr SemanticAnalyzer::defineType(const std::shared_ptr<TypeDeclaration>& node)
{
    TypeIdentifierPtr id = node->getIdentifier();
    SymbolScope* scope = NULL;
    TypePtr type;

    //Analyze the type's category
    Type::Category category;
    switch(node->getNodeType())
    {
        case NodeType::Enum:
            category = Type::Enum;
            break;
        case NodeType::Class:
            category = Type::Class;
            break;
        case NodeType::Struct:
            category = Type::Struct;
            break;
        case NodeType::Protocol:
            category = Type::Protocol;
            break;
        default:
            assert(0 && "Impossible to execute here.");
    }

    //it's inside the type's scope, so need to access parent scope;
    SymbolScope* typeScope = symbolRegistry->getCurrentScope();
    SymbolScope* currentScope = typeScope->getParentScope();

    //check if this type is already defined
    symbolRegistry->lookupType(id->getName(), &scope, &type);
    if(type && scope == currentScope)
    {
        //invalid redeclaration of type T
        error(node, Errors::E_INVALID_REDECLARATION_1, id->getName());
        return nullptr;
    }
    //prepare for generic types
    GenericDefinitionPtr generic;
    GenericParametersDefPtr genericParams = node->getGenericParametersDef();

    //check if it's defined as a nested type
    if(currentType)
    {
        if(genericParams)
        {
            error(node, Errors::E_GENERIC_TYPE_A_NESTED_IN_TYPE_B_IS_NOT_ALLOWED_2, id->getName(), currentType->getName());
            return nullptr;
        }
        if(currentType->isGenericType())
        {
            error(node, Errors::E_TYPE_A_NESTED_IN_GENERIC_TYPE_B_IS_NOT_ALLOWED_2, id->getName(), currentType->getName());
            return nullptr;
        }
    }

    if(genericParams)
    {
        generic = prepareGenericTypes(genericParams);
        generic->registerTo(typeScope);
    }

    //check inheritance clause
    TypePtr parent = nullptr;
    std::vector<TypePtr> protocols;
    bool first = true;

    for(const TypeIdentifierPtr& parentType : node->getParents())
    {
        parentType->accept(this);
        TypePtr ptr = this->lookupType(parentType);
        if(ptr->getCategory() == Type::Class && category == Type::Class)
        {
            if(!first)
            {
                //only the first type can be class type
                std::wstringstream out;
                NodeSerializerW serializer(out);
                parentType->accept(&serializer);
                error(parentType, Errors::E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE_1, out.str());
                return nullptr;
            }
            parent = ptr;
        }
        else if(category == Type::Enum && ptr->getCategory() != Type::Protocol)
        {

            if(parent)//already has a raw type
            {
                error(parentType, Errors::E_MULTIPLE_ENUM_RAW_TYPES_A_AND_B_2, parent->toString(), ptr->toString());
                return nullptr;
            }
            if(!first)
            {
                error(parentType, Errors::E_RAW_TYPE_A_MUST_APPEAR_FIRST_IN_THE_ENUM_INHERITANCE_CLAUSE_1, ptr->toString());
                return nullptr;
            }
            //check if the raw type is literal convertible
            if(!isLiteralTypeForEnum(symbolRegistry->getGlobalScope(), ptr))
            {
                error(parentType, Errors::E_RAW_TYPE_A_IS_NOT_CONVERTIBLE_FROM_ANY_LITERAL_1, ptr->toString());
                return nullptr;
            }
            if(!ptr->canAssignTo(symbolRegistry->getGlobalScope()->Equatable()))
            {
                error(parentType, Errors::E_RAWREPRESENTABLE_INIT_CANNOT_BE_SYNTHESIZED_BECAUSE_RAW_TYPE_A_IS_NOT_EQUATABLE_1, ptr->toString());
                return nullptr;
            }
            parent = ptr;
        }
        else if(ptr->getCategory() == Type::Protocol)
        {
            protocols.push_back(ptr);
        }
        else
        {
            std::wstringstream out;
            NodeSerializerW serializer(out);
            parentType->accept(&serializer);
            if(category == Type::Class)
                error(parentType, Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE_1, out.str());
            else
                error(parentType, Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE_1, out.str());
            return nullptr;
        }
        first = false;
    }

    //register this type
    type = Type::newType(node->getIdentifier()->getName(), category, node, parent, protocols, generic);
    node->setType(type);
    currentScope->addSymbol(type);

    declarationFinished(type->getName(), type, node);
    return type;
}

void SemanticAnalyzer::prepareDefaultInitializers(const TypePtr& type)
{

    /*
    Rule of initializers for class/structure:
    1) If no custom initializers, compiler will prepare one or two initializers:
        1.1) A default initializer with no arguments if all let/var fields are defined with a default value
        1.2) Skip this rule if it's a class. A default initializer with all let/var fields as initializer's parameters with the same external name,
            the order of the parameters are the exactly the same as them defined in structure
    2) Compiler will not generate initializers if there's custom initializers
     */

    if(type->getInitializer() && type->getInitializer()->numOverloads() > 0)
        return;

    bool createDefaultInit = true;
    vector<Type::Parameter> initParams;
    if(type->getCategory() == Type::Struct)
    {
        //check all fields if they all have initializer
        for (auto sym : type->getDeclaredStoredProperties())
        {
            SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(sym);
            if(!s || sym->hasFlags(SymbolFlagTemporary))
                continue;
            initParams.push_back(Type::Parameter(sym->getName(), false, sym->getType()));
            //do not create default init if there's a variable has no initializer
            if (!s->hasFlags(SymbolFlagHasInitializer))
            {
                createDefaultInit = false;
                break;
            }
        }
    }
    if(createDefaultInit)
    {
        //apply rule 1
        std::vector<Type::Parameter> params;
        TypePtr initType = Type::newFunction(params, type, false);
        std::wstring s = initType->toString();
        FunctionSymbolPtr initializer(new FunctionSymbol(L"init", initType, nullptr));
        declarationFinished(initializer->getName(), initializer, type->getReference());
    }
    if(type->getCategory() == Type::Struct && !initParams.empty())
    {
        TypePtr initType = Type::newFunction(initParams, type, false);
        FunctionSymbolPtr initializer(new FunctionSymbol(L"init", initType, nullptr));
        declarationFinished(initializer->getName(), initializer, type->getReference());
    }
}

void SemanticAnalyzer::visitTypeAlias(const TypeAliasPtr& node)
{
    SymbolScope* scope = nullptr;
    TypePtr type;
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();

    //check if this type is already defined
    symbolRegistry->lookupType(node->getName(), &scope, &type);
    if(type && scope == currentScope)
    {
        //invalid redeclaration of type T
        error(node, Errors::E_INVALID_REDECLARATION_1, node->getName());
        return;
    }
    type = Type::newType(node->getName(), Type::Alias);
    if(currentType && currentType->getCategory() == Type::Protocol && !node->getType())
    {
        //register a type place holder for protocol
    }
    else
    {
        TypePtr innerType = lookupType(node->getType());
        if(innerType)
            type = innerType;
        //static_pointer_cast<TypeBuilder>(type)->setInnerType(innerType);
    }
    currentScope->addSymbol(node->getName(), type);
    declarationFinished(node->getName(), type, node);
}


void SemanticAnalyzer::visitClass(const ClassDefPtr& node)
{
    TypePtr type = defineType(node);

    SCOPED_SET(currentType, type);


    NodeVisitor::visitClass(node);
    verifyProtocolConform(type);
    prepareDefaultInitializers(type);
}

void SemanticAnalyzer::visitStruct(const StructDefPtr& node)
{
    TypePtr type = defineType(node);

    SCOPED_SET(currentType, type);

    NodeVisitor::visitStruct(node);
    //prepare default initializers
    this->prepareDefaultInitializers(type);
    //Type verification and typealias inference

    for(auto entry : type->getAllParents())
    {
        TypePtr parent = entry.first;
        if(parent->getCategory() != Type::Protocol || !(parent->containsAssociatedType() || parent->containsSelfType()))
            continue;
        //this parent is a protocol that contains associated type, now validate protocol's methods and infer the types out
        std::map<std::wstring, TypePtr> associatedTypes;
        //collect all defined associated types
        for(auto entry : parent->getAssociatedTypes())
        {
            TypePtr type = entry.second->unwrap();
            if(type->getCategory() != Type::Alias)
                associatedTypes.insert(make_pair(entry.first, type));
        }
        if(parent->containsSelfType())
            associatedTypes.insert(make_pair(L"Self", type));

        for(const FunctionOverloadedSymbolPtr& funcs : parent->getDeclaredFunctions())
        {
            for(const FunctionSymbolPtr& expectedFunc : *funcs)
            {
                TypePtr expectedType = expectedFunc->getType();
                assert(expectedType != nullptr);
                bool matched = false;
                //get all methods with the same name, and check their signature one by one
                vector<SymbolPtr> funcs;
                bool staticMember = expectedFunc->hasFlags(SymbolFlagStatic);
                this->getMethodsFromType(type, expectedFunc->getName(), staticMember, funcs);
                for(const SymbolPtr& func : funcs)
                {
                    TypePtr actualType = func->getType();
                    assert(actualType != nullptr);
                    if(expectedType->canSpecializeTo(actualType, associatedTypes))
                    {
                        matched = true;
                        break;
                    }
                }
                if(!matched)
                {
                    //no matched function
                    error(node, Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), parent->getName(), expectedFunc->getName());
                    return;
                }
            }
        }
        //now make types infered above visible

        for(auto entry : associatedTypes)
        {
            if(entry.first == L"Self")
                continue;
            static_pointer_cast<TypeBuilder>(type)->addMember(entry.first, entry.second);
        }
    }

    verifyProtocolConform(type);






}


/*!
 *  Make the given enum type implements the protocol RawRepresentable.
 *  TODO Only one member is added for test case, add the rest members
 */
static void makeRawRepresentable(const TypeBuilderPtr& type, GlobalScope* global)
{
    type->addProtocol(global->RawRepresentable());
    //var rawValue: RawValue { get }
    TypePtr RawValue = type->getParentType();
    SymbolPlaceHolderPtr rawValue(new SymbolPlaceHolder(L"rawValue", RawValue, SymbolPlaceHolder::R_PROPERTY, SymbolFlagMember | SymbolFlagReadable));
    type->addMember(rawValue);
}
void SemanticAnalyzer::visitEnum(const EnumDefPtr& node)
{
    TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(defineType(node));
    //check member declaration of enum
    for(const DeclarationPtr& decl : *node)
    {
        if(decl->getNodeType() == NodeType::ValueBindings)
        {
            error(node, Errors::E_ENUMS_MAY_NOT_CONTAIN_STORED_PROPERTIES);
            continue;
        }
        decl->accept(this);
    }


    GlobalScope* global = symbolRegistry->getGlobalScope();
    //check if it's raw value enum
    bool isRawValues = false;
    if(node->numParents() > 0)
    {
        TypePtr firstParent = lookupType(node->getParent(0));
        if(firstParent)
        {
            Type::Category  category = firstParent->getCategory();
            isRawValues = category == Type::Struct || category == Type::Enum || category == Type::Aggregate || category == Type::Class;
        }
    }


    if(isRawValues)
    {
        if(!node->numCases())
        {
            error(node, Errors::E_ENUM_WITH_NO_CASES_CANNOT_DECLARE_A_RAW_TYPE);
            return;
        }
        //Add RawRepresentable protocol if it's not implemented
        if(!type->canAssignTo(global->RawRepresentable()))
        {
            makeRawRepresentable(type, global);
        }

        TypePtr rawType = type->getParentType();
        assert(rawType != nullptr);
        bool integerConvertible = rawType->canAssignTo(global->IntegerLiteralConvertible());
        for(auto c : node->getCases())
        {
            if(!c.value && !integerConvertible)
            {
                //Enum cases require explicit raw values when the raw type is not integer literal convertible
                error(node, Errors::E_ENUM_CASES_REQUIRE_EXPLICIT_RAW_VALUES_WHEN_THE_RAW_TYPE_IS_NOT_INTEGER_LITERAL_CONVERTIBLE);
                return;
            }
            if(c.value)
            {
                if(dynamic_pointer_cast<TupleType>(c.value))
                {
                    error(node, Errors::E_ENUM_WITH_RAW_TYPE_CANNOT_HAVE_CASES_WITH_ARGUMENTS);
                    return;
                }
                ExpressionPtr initializer = dynamic_pointer_cast<Expression>(c.value);
                assert(initializer != nullptr);
                if(!isLiteralExpression(initializer))
                {
                    error(node, Errors::E_RAW_VALUE_FOR_ENUM_CASE_MUST_BE_LITERAL);
                    return;
                }

                SCOPED_SET(t_hint, rawType);
                initializer->accept(this);
                TypePtr caseType = initializer->getType();
                assert(caseType != nullptr);
                if(!caseType->canAssignTo(rawType))
                {
                    error(node, Errors::E_A_IS_NOT_CONVERTIBLE_TO_B_2, caseType->toString(), rawType->toString());
                    return;
                }
            }
            type->addEnumCase(c.name, global->Void());
        }
    }
    else
    {
        //it's an associated-values enum
        for(auto c : node->getCases())
        {
            TypePtr associatedType = global->Void();
            if(c.value)
            {
                TypeNodePtr typeNode = dynamic_pointer_cast<TypeNode>(c.value);
                if(!typeNode)
                {
                    error(node, Errors::E_ENUM_CASE_CANNOT_HAVE_A_RAW_VALUE_IF_THE_ENUM_DOES_NOT_HAVE_A_RAW_TYPE);
                    return;
                }
                assert(typeNode != nullptr);
                associatedType = lookupType(typeNode);
            }
            type->addEnumCase(c.name, associatedType);
        }
    }
}
void SemanticAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    TypePtr type = defineType(node);

    SCOPED_SET(currentType, type);

    NodeVisitor::visitProtocol(node);
}
void SemanticAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    if(parentNode && parentNode->getNodeType() != NodeType::Program)
    {
        error(node, Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, node->getIdentifier()->getName());
        return;
    }
    if(node->getGenericParametersDef())
    {
        error(node, Errors::E_GENERIC_ARGUMENTS_ARE_NOT_ALLOWED_ON_AN_EXTENSION);
        return;
    }
    TypePtr type = lookupType(node->getIdentifier());
    Type::Category category = type->getCategory();
    if(category == Type::Protocol)
    {
        error(node, Errors::E_PROTOCOL_A_CANNOT_BE_EXTENDED_1, type->getName());
        return;
    }
    if(category != Type::Struct && category != Type::Enum && category != Type::Class)
    {
        error(node, Errors::E_NON_NOMINAL_TYPE_A_CANNOT_BE_EXTENDED_1, node->getIdentifier()->getName());
        return;
    }

    SCOPED_SET(currentType, type);
    TypePtr extension = Type::newExtension(type);
    symbolRegistry->getFileScope()->addExtension(extension);
    SCOPED_SET(currentExtension, extension);
    for(const DeclarationPtr& decl : *node)
    {
        if(decl->getNodeType() == NodeType::ValueBindings)
        {
            error(node, Errors::E_EXTENSIONS_MAY_NOT_CONTAIN_STORED_PROPERTIES);
            continue;
        }
        decl->accept(this);
    }
}



/*!
 * Verify if the specified type conform to the given protocol
 */
void SemanticAnalyzer::verifyProtocolConform(const TypePtr& type)
{
    for(const TypePtr& protocol : type->getProtocols())
    {
        if(protocol->containsAssociatedType())
            continue;//it's already done in SymbolResolveAction.cpp
        verifyProtocolConform(type, protocol);
    }
}
void SemanticAnalyzer::verifyProtocolConform(const TypePtr& type, const TypePtr& protocol)
{
    for(auto entry : protocol->getDeclaredMembers())
    {
        SymbolPtr requirement = entry.second;
        if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(requirement))
        {
            //verify function
            for(auto func : *funcs)
            {
                verifyProtocolFunction(type, protocol, func);
            }
        }
        else if(FunctionSymbolPtr func = std::dynamic_pointer_cast<FunctionSymbol>(requirement))
        {
            //verify function
            verifyProtocolFunction(type, protocol, func);
        }
            /*
            else if(requirement == Type::getPlaceHolder())
            {
                //verify inner type
                SymbolPtr sym = type->getAssociatedType(entry.first);
                if(!(std::dynamic_pointer_cast<Type>(sym)))
                {
                    //Type %0 does not conform to protocol %1, unimplemented type %2
                    error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3, type->getName(), protocol->getName(), entry.first);
                }
            }*/
        else if(TypePtr t = std::dynamic_pointer_cast<Type>(requirement))
        {
            //type can be ignored
        }
        else if(SymbolPlaceHolderPtr prop = std::dynamic_pointer_cast<SymbolPlaceHolder>(requirement))
        {
            //verify computed properties
            assert(prop->hasFlags(SymbolFlagMember) && prop->getRole() == SymbolPlaceHolder::R_PROPERTY);
            SymbolPtr sym = type->getMember(entry.first);
            SymbolPlaceHolderPtr sp = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
            if(!sp)
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
            bool expectedSetter = prop->hasFlags(SymbolFlagWritable);
            bool actualSetter = sp->hasFlags(SymbolFlagWritable);
            if(expectedSetter && !actualSetter)
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
        }
    }
}
void SemanticAnalyzer::verifyProtocolFunction(const TypePtr& type, const TypePtr& protocol, const FunctionSymbolPtr& expected)
{
    SymbolPtr sym = type->getMember(expected->getName());
    TypePtr expectedType = expected->getType();
    assert(expectedType != nullptr);
    if(!sym)
    {
        error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
        return;
    }
    else if(const FunctionSymbolPtr& func = std::dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        //verify if they're the same type
        TypePtr funcType = func->getType();
        assert(funcType != nullptr);
        if(!Type::equals(funcType, expectedType))
        {
            error(type->getReference(),  Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
        }
        return;
    }
    else if(FunctionOverloadedSymbolPtr funcs = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
    {
        //verify if they're the same type
        bool found = false;
        for(const FunctionSymbolPtr& func : *funcs)
        {
            if(Type::equals(func->getType(), expectedType))
            {
                found = true;
                break;
            }
        }
        if(!found)
            error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
        return;
    }
    else if(SymbolPlaceHolderPtr prop = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym))
    {


    }
    error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, type->getName(), protocol->getName(), expected->getName());
}

void SemanticAnalyzer::visitOptionalType(const OptionalTypePtr& node)
{
    TypePtr innerType = lookupType(node->getInnerType());
    TypePtr type = symbolRegistry->getGlobalScope()->makeOptional(innerType);
    node->setType(type);
}
