/* DeclarationAnalyzer_TypeCreation.cpp --
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
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/SemanticAnalyzer.h"
#include "ast/ast.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ScopedNodes.h"
#include "common/Errors.h"
#include "semantics/TypeBuilder.h"
#include "semantics/GlobalScope.h"
#include <cassert>
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"
#include "semantics/TypeResolver.h"
#include "semantics/SemanticContext.h"

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
TypePtr DeclarationAnalyzer::defineType(const std::shared_ptr<TypeDeclaration>& node)
{
    TypeIdentifierPtr id = node->getIdentifier();

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
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();

    //prepare for generic types
    GenericParametersDefPtr genericParams = node->getGenericParametersDef();

    //check if it's defined as a nested type
    if(ctx->currentType)
    {
        if(genericParams)
        {
            error(node, Errors::E_GENERIC_TYPE_A_NESTED_IN_TYPE_B_IS_NOT_ALLOWED_2, id->getName(), ctx->currentType->getName());
            return nullptr;
        }
        if(ctx->currentType->isGenericType())
        {
            error(node, Errors::E_TYPE_A_NESTED_IN_GENERIC_TYPE_B_IS_NOT_ALLOWED_2, id->getName(), ctx->currentType->getName());
            return nullptr;
        }
    }
    
    //register this type
    TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(currentScope->getForwardDeclaration(id->getName()));

    assert(type != nullptr);
    assert(type->getCategory() == category);
    //prepare for generic
    if(!type->getGenericDefinition() && node->getGenericParametersDef())
    {
        GenericParametersDefPtr genericParams = node->getGenericParametersDef();
        GenericDefinitionPtr generic = prepareGenericTypes(genericParams);
        generic->registerTo(type->getScope());
        type->setGenericDefinition(generic);
    }
    
    if(node->hasModifier(DeclarationModifiers::Final))
        type->setFlags(SymbolFlagFinal, true);
    static_pointer_cast<TypeBuilder>(type)->setModuleName(ctx->currentModule->getName());
    ctx->allTypes.push_back(type);
    
    //check inheritance clause
    TypePtr parent = nullptr;
    bool first = true;

    for(const TypeIdentifierPtr& parentType : node->getParents())
    {
        parentType->accept(this);
        TypePtr ptr = resolveType(parentType, true);
        if(ptr->getCategory() == Type::Class && category == Type::Class)
        {
            if(!first)
            {
                //only the first type can be class type
                error(parentType, Errors::E_SUPERCLASS_MUST_APPEAR_FIRST_IN_INHERITANCE_CLAUSE_1, toString(parentType));
                return nullptr;
            }
            parent = ptr;
            if(parent->hasFlags(SymbolFlagFinal))
            {
                error(parentType, Errors::E_INHERITANCE_FROM_A_FINAL_CLASS_A_1, parentType->getName());
                return nullptr;
            }
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
            type->addProtocol(ptr);
        }
        else
        {
            if(category == Type::Class)
                error(parentType, Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_NON_CLASS_TYPE_1, toString(parentType));
            else
                error(parentType, Errors::E_INHERITANCE_FROM_NONE_PROTOCOL_TYPE_1, toString(parentType));
            return nullptr;
        }
        first = false;
    }
    type->setParentType(parent);
    if(parent && parent->getAccessLevel() == AccessLevelPublic && (node->getModifiers() & DeclarationModifiers::AccessModifiers) == 0)
        type->setAccessLevel(AccessLevelPublic);//when access level is undefined, try to inherit base's access level
    else
        type->setAccessLevel(parseAccessLevel(node->getModifiers()));
    

    declarationFinished(type->getName(), type, node);
    return type;
}

/*!
 * Calculates the number of designated initializers
 */
static int numDesignatedInitializers(const FunctionOverloadedSymbolPtr& initializers)
{
    if(!initializers)
        return 0;
    int total = 0;
    for(const FunctionSymbolPtr& init : *initializers)
    {
        if(!init->getType()->hasFlags(SymbolFlagConvenienceInit))
        {
            total++;
        }
    }
    return total;
}

void DeclarationAnalyzer::prepareDefaultInitializers(const TypePtr& type)
{

    /*
    Rule of initializers for class/structure:
    1) If no custom initializers, compiler will prepare one or two initializers:
        1.1) A default initializer with no arguments if all let/var fields are defined with a default value
        1.2) Skip this rule if it's a class. A default initializer with all let/var fields as initializer's parameters with the same external name,
            the order of the parameters are the exactly the same as them defined in structure
    2) Compiler will not generate initializers if there's custom initializers
    3) Convenience initializers will not be counted
     */
    SCOPED_SET(ctx->currentType, type);
    FunctionOverloadedSymbolPtr initializers = type->getDeclaredInitializer();
    int designatedInitializers = numDesignatedInitializers(initializers);
    if(designatedInitializers == 0)
    {
        bool createDefaultInit = true;
        vector<Parameter> initParams;
        if (type->getCategory() == Type::Struct)
        {
            //check all fields if they all have initializer
            for (auto sym : type->getDeclaredStoredProperties())
            {
                SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(sym);
                if (!s || sym->hasFlags(SymbolFlagTemporary))
                    continue;
                initParams.push_back(Parameter(sym->getName(), false, sym->getType()));
                //do not create default init if there's a variable has no initializer
                if (!s->hasFlags(SymbolFlagHasInitializer))
                    createDefaultInit = false;
            }
        }
        GlobalScope* global = symbolRegistry->getGlobalScope();
        bool initCreated = false;
        if (createDefaultInit)
        {
            //apply rule 1
            std::vector<Parameter> params;
            TypePtr initType = Type::newFunction(params, global->Void(), false);
            initType->setFlags(SymbolFlagInit, true);
            FunctionSymbolPtr initializer(new FunctionSymbol(L"init", initType, FunctionRoleInit, nullptr));
            declarationFinished(initializer->getName(), initializer, nullptr);
            initCreated = true;
        }
        if (type->getCategory() == Type::Struct && !initParams.empty())
        {
            TypePtr initType = Type::newFunction(initParams, global->Void(), false);
            initType->setFlags(SymbolFlagInit, true);
            FunctionSymbolPtr initializer(new FunctionSymbol(L"init", initType, FunctionRoleInit, nullptr));
            declarationFinished(initializer->getName(), initializer, nullptr);
            initCreated = true;
        }
        //make all stored properties initialized after the default initializer created
        if(initCreated)
        {
            for(const SymbolPtr& s : type->getDeclaredStoredProperties())
            {
                s->setFlags(SymbolFlagInitialized, true);
            }
        }

    }
    //inherit designated initializers from parent type
    if(type->getParentType() && type->getCategory() == Type::Class)
    {
        FunctionOverloadedSymbolPtr initializers = type->getDeclaredInitializer();

        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(type);
        if(!initializers)
        {
            initializers = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(L"init"));
            builder->setInitializer(initializers);
        }
        TypePtr parent = type->getParentType();
        FunctionOverloadedSymbolPtr baseInitializers = parent->getDeclaredInitializer();
        if(baseInitializers)
        {
            for(const FunctionSymbolPtr& baseInitializer : *baseInitializers)
            {
                //skip convenience initializer
                if(baseInitializer->hasFlags(SymbolFlagConvenienceInit))
                    continue;
                //check if it's defined in current type
                TypePtr initType = baseInitializer->getType();
                FunctionSymbolPtr initializer = initializers->lookupByType(initType);
                if(!initializer)
                {
                    //this initializer exists in base type, but not in current type
                    //so we need to create it in current type
                    initializer = FunctionSymbolPtr(new FunctionSymbol(L"init", initType, FunctionRoleInit, nullptr));
                    //initializers->add(initializer);
                    builder->addMember(initializer);
                }
                
            }
        }
    }
}

void DeclarationAnalyzer::visitTypeAlias(const TypeAliasPtr& node)
{
    if(!ctx->currentType && ctx->lazyDeclaration)
    {
        delayDeclare(node);
        return;
    }
    TypePtr type;
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();
    
    if(currentScope->isSymbolDefined(node->getName()))
    {
        error(node, Errors::E_INVALID_REDECLARATION_1, node->getName());
        return;
    }

    //type = currentScope->getForwardDeclaration(node->getName());
    if(ctx->currentType && ctx->currentType->getCategory() == Type::Protocol && !node->getType())
    {
        //register a type place holder for protocol
        type = Type::newTypeAlias(node->getName(), nullptr, nullptr);
    }
    else
    {
        shared_ptr<TypeResolver> typeResolver(new TypeResolver(symbolRegistry, semanticAnalyzer, this, ctx, true));
        //TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(type);
        type = resolveType(node->getType(), true);
        //builder->setInnerType(type);
        //builder->initAlias(node->getType(), typeResolver);
    }
    validateDeclarationModifiers(node);
    declarationFinished(node->getName(), type, node);
    currentScope->addSymbol(node->getName(), type);
}


void DeclarationAnalyzer::visitClass(const ClassDefPtr& node)
{
    TypePtr type = defineType(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    SCOPED_SET(ctx->currentFunction, nullptr);

    assert(type->getAccessLevel() != AccessLevelUndefined);
    if(type->getParentType() != nullptr)
        verifyAccessLevel(node, type->getParentType(), DeclarationTypeClass, ComponentTypeSuperclass);
    visitDeclaration(node);
    
    prepareDefaultInitializers(type);
    validateDeclarationModifiers(node);
    //visitImplementation(node);
}
void DeclarationAnalyzer::visitDeclaration(const TypeDeclarationPtr &node)
{
    SCOPED_SET(ctx->flags, (ctx->flags & (~SemanticContext::FLAG_PROCESS_IMPLEMENTATION)) | SemanticContext::FLAG_PROCESS_DECLARATION);
    for (auto decl : *node)
        decl->accept(this);
}
void DeclarationAnalyzer::visitImplementation(const TypeDeclarationPtr &node)
{
    SCOPED_SET(ctx->flags, (ctx->flags & (~SemanticContext::FLAG_PROCESS_DECLARATION)) | SemanticContext::FLAG_PROCESS_IMPLEMENTATION);
    for (auto decl : *node)
    {
        NodeType t = decl->getNodeType();
        switch(t)
        {
            case NodeType::Class:
            case NodeType::Struct:
            case NodeType::Enum:
            case NodeType::Protocol:
            case NodeType::TypeAlias:
            case NodeType::EnumCasePattern:
                break;
            case NodeType::Init:
                decl->accept(this);
                break;
            case NodeType::ComputedProperty:
            case NodeType::Function:
            case NodeType::Subscript:
            case NodeType::Deinit:
                //mark all stored properties initialized
                for(const SymbolPtr& prop : ctx->currentType->getDeclaredStoredProperties())
                {
                    prop->setFlags(SymbolFlagInitialized, true);
                }
                decl->accept(this);
                break;
            default:
                decl->accept(this);
                break;
        }
    }
}

void DeclarationAnalyzer::visitStruct(const StructDefPtr& node)
{
    TypePtr type = defineType(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    SCOPED_SET(ctx->currentFunction, nullptr);
    visitDeclaration(node);
    //prepare default initializers
    //NOTE: default initializer was generated when code has implicitly accessed it.
    //this->prepareDefaultInitializers(type);
    //Type verification and typealias inference
    validateDeclarationModifiers(node);
//    visitImplementation(node);
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
    type->addMember(L"RawValue", RawValue);
    type->addMember(rawValue);
}
void DeclarationAnalyzer::visitEnum(const EnumDefPtr& node)
{
    TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(defineType(node));
    ScopeGuard scope(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    SCOPED_SET(ctx->currentFunction, nullptr);


    GlobalScope* global = symbolRegistry->getGlobalScope();
    SymbolScope* local = symbolRegistry->getCurrentScope();
    //check if it's raw value enum
    bool isRawValues = false;
    if(node->numParents() > 0)
    {
        TypePtr firstParent = lookupType(node->getParent(0));
        if(firstParent)
        {
            Type::Category  category = firstParent->getCategory();
            isRawValues = category == Type::Struct || category == Type::Enum || category == Type::Aggregate || category == Type::Class;
            if(isRawValues)
                type->setParentType(firstParent);
        }
    }

    //initialize enum's cases
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
            }
            type->addEnumCase(c.name, global->Void());
            //register it to scope
            local->addSymbol(SymbolPlaceHolderPtr(new SymbolPlaceHolder(c.name, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagReadable | SymbolFlagMember | SymbolFlagStatic | SymbolFlagInitialized)));
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
            if(associatedType == global->Void())
                local->addSymbol(SymbolPlaceHolderPtr(new SymbolPlaceHolder(c.name, type, SymbolPlaceHolder::R_PARAMETER, SymbolFlagReadable | SymbolFlagMember | SymbolFlagStatic | SymbolFlagInitialized)));
        }
    }
    //check member declaration of enum
    {
        SCOPED_SET(ctx->flags, (ctx->flags & (~SemanticContext::FLAG_PROCESS_IMPLEMENTATION)) | SemanticContext::FLAG_PROCESS_DECLARATION);
        for (const DeclarationPtr &decl : *node)
        {
            bool isStatic = decl->hasModifier(DeclarationModifiers::Class) || decl->hasModifier(DeclarationModifiers::Static);
            if (!isStatic && (decl->getNodeType() == NodeType::ValueBindings))
            {
                error(node, Errors::E_ENUMS_MAY_NOT_CONTAIN_STORED_PROPERTIES);
                continue;
            }
            decl->accept(this);
        }
        //add a default initializer for raw value enum
        if(isRawValues)
        {
            vector<Parameter> params = {Parameter(L"rawValue", false, type->getParentType())};
            TypePtr initType = Type::newFunction(params, global->Void(), false, nullptr);
            initType->setFlags(SymbolFlagAllocatingInit, true);
            static_pointer_cast<TypeBuilder>(initType)->setDeclaringType(type);
            initType->setFlags(SymbolFlagInit | SymbolFlagFailableInitializer | SymbolFlagMember | SymbolFlagAllocatingInit);
            FunctionSymbolPtr init(new FunctionSymbol(L"init", initType, FunctionRoleInit, nullptr));
            declarationFinished(init->getName(), init, nullptr);
        }
    }
    if(type->getParentType() != nullptr)
        verifyAccessLevel(node, type->getParentType(), DeclarationTypeEnum, ComponentTypeRawType);
    validateDeclarationModifiers(node);
    //visitImplementation(node);
}
void DeclarationAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    TypePtr type = defineType(node);
    ScopeGuard scope(symbolRegistry, type->getScope());
    SCOPED_SET(ctx->currentType, type);
    SCOPED_SET(ctx->currentFunction, nullptr);

    visitDeclaration(node);
    validateDeclarationModifiers(node);
}
void DeclarationAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    if(ctx->currentFunction || ctx->currentType)
    {
        error(node, Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, node->getIdentifier()->getName());
        return;
    }
    //if(parentNode && parentNode->getNodeType() != NodeType::Program)
    if(node->getGenericParametersDef())
    {
        error(node, Errors::E_GENERIC_ARGUMENTS_ARE_NOT_ALLOWED_ON_AN_EXTENSION);
        return;
    }
    //TypePtr type = lookupType(node->getIdentifier());
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    TypePtr type = symbolRegistry->lookupType(node->getIdentifier()->getName());
    if(!type)
        type = scope->getForwardDeclaration(node->getIdentifier()->getName());
    if(!type)
    {
        error(node->getIdentifier(), Errors::E_USE_OF_UNDECLARED_TYPE_1, node->getIdentifier()->getName());
        return;
    }
    ScopeGuard guard(symbolRegistry, type->getScope());

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

    TypePtr extension = Type::newExtension(type);
    static_pointer_cast<SemanticExtensionDef>(node)->extension = extension;
    SCOPED_SET(ctx->currentType, type);
    SCOPED_SET(ctx->currentExtension, extension);
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


