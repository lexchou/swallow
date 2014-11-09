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
#include "FunctionIterator.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include "common/Errors.h"
#include "ast/NodeSerializer.h"
#include "TypeBuilder.h"
#include <cassert>

USE_SWALLOW_NS
using namespace std;

TypePtr SemanticAnalyzer::defineType(const std::shared_ptr<TypeDeclaration>& node, Type::Category category)
{
    TypeIdentifierPtr id = node->getIdentifier();
    SymbolScope* scope = NULL;
    TypePtr type;
    //it's inside the type's scope, so need to access parent scope;
    SymbolScope* currentScope = symbolRegistry->getCurrentScope()->getParentScope();

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
    GenericParametersDefPtr genericParams;
    if(node->getNodeType() == NodeType::Class)
        genericParams = std::static_pointer_cast<ClassDef>(node)->getGenericParametersDef();
    else if(node->getNodeType() == NodeType::Struct)
        genericParams = std::static_pointer_cast<StructDef>(node)->getGenericParametersDef();

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
        generic->registerTo(currentScope);
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
            first = false;
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
    }

    //register this type
    type = Type::newType(node->getIdentifier()->getName(), category, node, parent, protocols, generic);
    node->setType(type);
    currentScope->addSymbol(type);

    if(currentType)
        static_pointer_cast<TypeBuilder>(currentType)->addMember(type->getName(), type);
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
    if(!type->getInitializer())
        static_pointer_cast<TypeBuilder>(type)->setInitializer(FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol()));


    bool createDefaultInit = true;
    vector<Type::Parameter> initParams;
    if(type->getCategory() == Type::Struct)
    {
        //check all fields if they all have initializer
        for (auto sym : type->getDeclaredStoredProperties())
        {
            SymbolPlaceHolderPtr s = dynamic_pointer_cast<SymbolPlaceHolder>(sym);
            if(!s || (s->flags & SymbolPlaceHolder::F_TEMPORARY) != 0)
                continue;
            initParams.push_back(Type::Parameter(sym->getName(), false, sym->getType()));
            //do not create default init if there's a variable has no initializer
            if ((s->flags & SymbolPlaceHolder::F_HAS_INITIALIZER) == 0)
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
        FunctionSymbolPtr initializer(new FunctionSymbol(type->getName(), initType, nullptr));
        type->getInitializer()->add(initializer);
    }
    if(type->getCategory() == Type::Struct && !initParams.empty())
    {
        TypePtr initType = Type::newFunction(initParams, type, false);
        FunctionSymbolPtr initializer(new FunctionSymbol(type->getName(), initType, nullptr));
        type->getInitializer()->add(initializer);
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
        static_pointer_cast<TypeBuilder>(type)->setInnerType(innerType);
    }
    currentScope->addSymbol(node->getName(), type);
    if(currentType)
    {
        static_pointer_cast<TypeBuilder>(currentType)->addMember(node->getName(), type);
    }
}


void SemanticAnalyzer::visitClass(const ClassDefPtr& node)
{
    TypePtr type = defineType(node, Type::Class);
    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);


    NodeVisitor::visitClass(node);
    verifyProtocolConform(type);
    prepareDefaultInitializers(type);
}

void SemanticAnalyzer::visitStruct(const StructDefPtr& node)
{
    TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(defineType(node, Type::Struct));

    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);

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

                for(auto func : FunctionIterator(type, expectedFunc->getName()))
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
            type->addMember(entry.first, entry.second);
        }
    }

    verifyProtocolConform(type);






}
void SemanticAnalyzer::visitEnum(const EnumDefPtr& node)
{
    TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(defineType(node, Type::Enum));
    NodeVisitor::visitEnum(node);
    if(node->numConstants())
    {
        for(int i = 0; i < node->numConstants(); i++)
        {
            auto c = node->getConstant(i);
            int flags = SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_HAS_INITIALIZER | SymbolPlaceHolder::F_MEMBER | SymbolPlaceHolder::F_STATIC;
            SymbolPlaceHolderPtr symb(new SymbolPlaceHolder(c.name, type, SymbolPlaceHolder::R_PROPERTY, flags));
            type->addMember(symb);
        }
    }
    else if(node->numAssociatedTypes())
    {
        for(int i = 0; i < node->numAssociatedTypes(); i++)
        {
            auto c = node->getAssociatedType(i);
            if(c.value)
            {
                //TODO: generate a function for associated type
            }
            else
            {
                int flags = SymbolPlaceHolder::F_READABLE | SymbolPlaceHolder::F_HAS_INITIALIZER | SymbolPlaceHolder::F_MEMBER | SymbolPlaceHolder::F_STATIC;
                SymbolPlaceHolderPtr symb(new SymbolPlaceHolder(c.name, type, SymbolPlaceHolder::R_PROPERTY, flags));
                type->addMember(symb);
            }
        }
    }
}
void SemanticAnalyzer::visitProtocol(const ProtocolDefPtr& node)
{
    TypePtr type = defineType(node, Type::Protocol);

    StackedValueGuard<TypePtr> currentType(this->currentType);
    currentType.set(type);

    NodeVisitor::visitProtocol(node);
}
void SemanticAnalyzer::visitExtension(const ExtensionDefPtr& node)
{
    NodeVisitor::visitExtension(node);

}



/**
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
            assert(prop->flags & SymbolPlaceHolder::F_MEMBER && prop->getRole() == SymbolPlaceHolder::R_PROPERTY);
            SymbolPtr sym = type->getMember(entry.first);
            SymbolPlaceHolderPtr sp = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym);
            if(!sp)
            {
                error(type->getReference(), Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3, type->getName(), protocol->getName(), entry.first);
            }
            bool expectedSetter = prop->flags & SymbolPlaceHolder::F_WRITABLE;
            bool actualSetter = sp->flags & SymbolPlaceHolder::F_WRITABLE;
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
        if(*funcType != *expectedType)
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
            if(*func->getType() == *expectedType)
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
