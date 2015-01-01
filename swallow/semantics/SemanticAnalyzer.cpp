/* SemanticAnalyzer.cpp --
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
#include <cassert>
#include "SymbolScope.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "GenericDefinition.h"
#include "GenericArgument.h"
#include <string>
#include "TypeBuilder.h"
#include "common/CompilerResults.h"
#include "SymbolRegistry.h"
#include "ast/NodeSerializer.h"
#include "GlobalScope.h"
#include "ast/NodeFactory.h"
#include "FunctionOverloadedSymbol.h"
#include "FunctionSymbol.h"
#include "common/ScopedValue.h"

USE_SWALLOW_NS
using namespace std;


SemanticAnalyzer::SemanticAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
:SemanticPass(symbolRegistry, compilerResults)
{
    numTemporaryNames = 0;
}

std::wstring SemanticAnalyzer::generateTempName()
{
    std::wstringstream ss;
    ss<<L"#"<<this->numTemporaryNames++;
    return ss.str();
}

void SemanticAnalyzer::prepareParameters(SymbolScope* scope, const ParametersPtr& params)
{
    //check and prepare for parameters

    for(const ParameterPtr& param : *params)
    {
        assert(param->getType() != nullptr);
        SymbolPtr sym = scope->lookup(param->getLocalName());
        if(sym)
        {
            error(param, Errors::E_DEFINITION_CONFLICT, param->getLocalName());
            return;
        }
        sym = SymbolPtr(new SymbolPlaceHolder(param->getLocalName(), param->getType(), SymbolPlaceHolder::R_PARAMETER, SymbolFlagInitialized));
        scope->addSymbol(sym);
    }
    //prepare for implicit parameter self

}

GenericDefinitionPtr SemanticAnalyzer::prepareGenericTypes(const GenericParametersDefPtr& params)
{
    GenericDefinitionPtr ret(new GenericDefinition());
    for (const TypeIdentifierPtr &typeId : *params)
    {
        if (typeId->getNestedType())
        {
            error(typeId->getNestedType(), Errors::E_NESTED_TYPE_IS_NOT_ALLOWED_HERE);
            continue;
        }
        std::wstring name = typeId->getName();
        TypePtr old = ret->get(name);
        if (old != nullptr)
        {
            error(typeId, Errors::E_DEFINITION_CONFLICT);
            continue;
        }
        std::vector<TypePtr> protocols;
        TypePtr type = Type::newType(name, Type::GenericParameter, nullptr, nullptr, protocols);
        ret->add(name, type);
    }
    //add constraint
    for(const GenericConstraintDefPtr& constraint : params->getConstraints())
    {
        //constraint->getConstraintType()
        list<wstring> types;
        TypeIdentifierPtr typeId = constraint->getIdentifier();
        TypeBuilderPtr type = static_pointer_cast<TypeBuilder>(ret->get(typeId->getName()));
        TypePtr expectedType = lookupType(constraint->getExpectedType());

        if(type == nullptr)
        {
            error(typeId, Errors::E_USE_OF_UNDECLARED_TYPE_1, typeId->getName());
            continue;
        }
        if(constraint->getConstraintType() == GenericConstraintDef::EqualsTo)
        {
            //Same-type requirement makes generic parameter 'T' non-generic
            if(typeId->getNestedType() == nullptr)
            {
                error(typeId, Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1, typeId->getName());
                continue;
            }
            //check if the reference type is a protocol that contains Self or associated type
            if(expectedType->getCategory() == Type::Protocol)
            {
                if(expectedType->containsSelfType() || expectedType->containsAssociatedType())
                {
                    error(typeId, Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1, expectedType->getName());
                    continue;
                }
            }
        }
        types.push_back(typeId->getName());
        typeId = typeId->getNestedType();
        while(typeId != nullptr)
        {
            wstring name = typeId->getName();
            types.push_back(name);
            TypePtr childType = type->getAssociatedType(name);
            if(!childType)
            {
                //childType = Type::newType(name, Type::Placeholder, nullptr);
                //type->getSymbols()[name] = childType;
                error(typeId, Errors::E_IS_NOT_A_MEMBER_OF_2, name, type->getName());
                return ret;
            }
            type = static_pointer_cast<TypeBuilder>(childType);
            typeId = typeId->getNestedType();
        }
        if(expectedType->getCategory() == Type::Protocol)
        {
            type->addProtocol(expectedType);
        }
        else
        {
            //it's a base type
            if(type->getParentType() != nullptr)
            {
                error(constraint->getIdentifier(), Errors::E_MULTIPLE_INHERITANCE_FROM_CLASS_2_, type->getParentType()->getName(), expectedType->getName());
                continue;
            }
            type->setParentType(expectedType);
        }

        //ret->addConstraint(types);
        GenericDefinition::ConstraintType ct = constraint->getConstraintType() == GenericConstraintDef::DerivedFrom ? GenericDefinition::DerivedFrom : GenericDefinition::EqualsTo;
        ret->addConstraint(types, ct, expectedType);
    }

    return ret;
}


std::wstring SemanticAnalyzer::toString(const NodePtr& node)
{
    std::wstringstream out;
    NodeSerializerW serializer(out);
    node->accept(&serializer);
    return out.str();
}
std::wstring SemanticAnalyzer::toString(int i)
{
    std::wstringstream s;
    s<<i;
    return s.str();
}
TypePtr SemanticAnalyzer::lookupType(const TypeNodePtr& type, bool supressErrors)
{
    if(!type)
        return nullptr;
    TypePtr ret = type->getType();
    if(!ret)
    {
        ret = lookupTypeImpl(type, supressErrors);
        type->setType(ret);
    }
    return ret;
}
TypePtr SemanticAnalyzer::lookupTypeImpl(const TypeNodePtr &type, bool supressErrors)
{
    if(TypeIdentifierPtr id = std::dynamic_pointer_cast<TypeIdentifier>(type))
    {
        //TODO: make a generic type if possible
        TypePtr ret = symbolRegistry->lookupType(id->getName());
        if(!ret)
        {
            if(!supressErrors)
            {
                std::wstring str = toString(type);
                error(type, Errors::E_USE_OF_UNDECLARED_TYPE_1, str);
                abort();
            }
            return nullptr;
        }
        GenericDefinitionPtr generic = ret->getGenericDefinition();
        if(generic == nullptr && id->numGenericArguments() == 0)
            return ret;
        if(generic == nullptr && id->numGenericArguments() > 0)
        {
            if(!supressErrors)
            {
                std::wstring str = toString(type);
                error(id, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, str);
            }
            return nullptr;
        }
        if(generic != nullptr && id->numGenericArguments() == 0)
        {
            if(!supressErrors)
            {
                std::wstring str = toString(type);
                error(id, Errors::E_GENERIC_TYPE_ARGUMENT_REQUIRED, str);
            }
            return nullptr;
        }
        if(id->numGenericArguments() > generic->numParameters())
        {
            if(!supressErrors)
            {
                std::wstring str = toString(type);
                std::wstring got = toString(id->numGenericArguments());
                std::wstring expected = toString(generic->numParameters());
                error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_TOO_MANY_TYPE_PARAMETERS_3, str, got, expected);
            }
            return nullptr;
        }
        if(id->numGenericArguments() < generic->numParameters())
        {
            if(!supressErrors)
            {
                std::wstring str = toString(type);
                std::wstring got = toString(id->numGenericArguments());
                std::wstring expected = toString(generic->numParameters());
                error(id, Errors::E_GENERIC_TYPE_SPECIALIZED_WITH_INSUFFICIENT_TYPE_PARAMETERS_3, str, got, expected);
            }
            return nullptr;
        }
        //check type
        GenericArgumentPtr genericArgument(new GenericArgument(generic));
        for(auto arg : *id)
        {
            TypePtr argType = lookupType(arg);
            if(!argType)
                return nullptr;
            genericArgument->add(argType);
        }
        TypePtr base = Type::newSpecializedType(ret, genericArgument);
        ret = base;
        //access rest nested types
        for(TypeIdentifierPtr n = id->getNestedType(); n != nullptr; n = n->getNestedType())
        {
            TypePtr childType = ret->getAssociatedType(n->getName());
            if(!childType)
            {
                if(!supressErrors)
                    error(n, Errors::E_A_IS_NOT_A_MEMBER_TYPE_OF_B_2, n->getName(), ret->toString());
                return nullptr;
            }
            if(n->numGenericArguments())
            {
                //nested type is always a non-generic type
                if(!supressErrors)
                    error(n, Errors::E_CANNOT_SPECIALIZE_NON_GENERIC_TYPE_1, childType->toString());
                return nullptr;
            }
            ret = childType;
        }

        return ret;
    }
    if(TupleTypePtr tuple = std::dynamic_pointer_cast<TupleType>(type))
    {
        std::vector<TypePtr> elementTypes;
        for(const TupleType::TupleElement& e : *tuple)
        {
            TypePtr t = lookupType(e.type);
            elementTypes.push_back(t);
        }
        return Type::newTuple(elementTypes);
    }
    if(ArrayTypePtr array = std::dynamic_pointer_cast<ArrayType>(type))
    {
        GlobalScope* global = symbolRegistry->getGlobalScope();
        TypePtr innerType = lookupType(array->getInnerType());
        assert(innerType != nullptr);
        TypePtr ret = global->makeArray(innerType);
        return ret;
    }
    if(DictionaryTypePtr array = std::dynamic_pointer_cast<DictionaryType>(type))
    {
        GlobalScope* scope = symbolRegistry->getGlobalScope();
        TypePtr keyType = lookupType(array->getKeyType());
        TypePtr valueType = lookupType(array->getValueType());
        assert(keyType != nullptr);
        assert(valueType != nullptr);
        TypePtr ret = scope->makeDictionary(keyType, valueType);
        return ret;
    }
    if(OptionalTypePtr opt = std::dynamic_pointer_cast<OptionalType>(type))
    {
        GlobalScope* global = symbolRegistry->getGlobalScope();
        TypePtr innerType = lookupType(opt->getInnerType());
        assert(innerType != nullptr);
        TypePtr ret = global->makeOptional(innerType);
        return ret;
    }
    if(FunctionTypePtr func = std::dynamic_pointer_cast<FunctionType>(type))
    {
        TypePtr retType = nullptr;
        if(func->getReturnType())
        {
            retType = lookupType(func->getReturnType());
        }
    }
    assert(0 && "Unsupported type");
    return nullptr;
}
/*!
 * Convert expression node to type node
 */
TypeNodePtr SemanticAnalyzer::expressionToType(const ExpressionPtr& expr)
{
    assert(expr != nullptr);
    NodeType::T nodeType = expr->getNodeType();
    switch(nodeType)
    {
        case NodeType::Identifier:
        {
            IdentifierPtr id = static_pointer_cast<Identifier>(expr);
            TypeIdentifierPtr ret = id->getNodeFactory()->createTypeIdentifier(*id->getSourceInfo());
            ret->setName(id->getIdentifier());
            if(id->getGenericArgumentDef())
            {
                for(const TypeNodePtr& type : *id->getGenericArgumentDef())
                {
                    ret->addGenericArgument(type);
                }
            }
            return ret;
        }
        case NodeType::MemberAccess:
        {
            MemberAccessPtr memberAccess = static_pointer_cast<MemberAccess>(expr);
            TypeIdentifierPtr self = dynamic_pointer_cast<TypeIdentifier>(expressionToType(memberAccess->getSelf()));
            assert(memberAccess->getField() != nullptr);
            TypeIdentifierPtr tail = dynamic_pointer_cast<TypeIdentifier>(expressionToType(memberAccess->getField()));
            if(!self || !tail)
                return nullptr;
            //append the tail to the end of the expression
            TypeIdentifierPtr p = self;
            while(p->getNestedType())
            {
                p = p->getNestedType();
            }
            p->setNestedType(tail);
            return self;
        }
        case NodeType::Tuple:
        {
            TuplePtr tuple = static_pointer_cast<Tuple>(expr);
            TupleTypePtr ret = tuple->getNodeFactory()->createTupleType(*tuple->getSourceInfo());
            for(const PatternPtr& pattern : *tuple)
            {
                ExpressionPtr expr = dynamic_pointer_cast<Expression>(pattern);
                TypeNodePtr t = expressionToType(expr);
                if(!t)
                    return nullptr;
                ret->add(false, L"", t);
            }
            return ret;
        }
        default:
            return nullptr;
    }
}

/*!
 * Expand given expression to given Optional<T> type by adding implicit Optional<T>.Some calls
 * Return false if the given expression cannot be conform to given optional type
 */
bool SemanticAnalyzer::expandOptional(const TypePtr& optionalType, ExpressionPtr& expr)
{
    GlobalScope* global = symbolRegistry->getGlobalScope();
    TypePtr exprType = expr->getType();
    assert(exprType != nullptr);
    if(exprType->canAssignTo(optionalType))
        return true;
    if(optionalType->getCategory() == Type::Specialized && optionalType->getInnerType() == global->Optional())
    {
        TypePtr innerType = optionalType->getGenericArguments()->get(0);
        bool ret = expandOptional(innerType, expr);
        if(!ret)
            return ret;
        //wrap it with an Optional
        NodeFactory* factory = expr->getNodeFactory();
        MemberAccessPtr ma = factory->createMemberAccess(*expr->getSourceInfo());
        IdentifierPtr Some = factory->createIdentifier(*expr->getSourceInfo());
        Some->setIdentifier(L"Some");
        ma->setType(optionalType);
        ma->setField(Some);
        ParenthesizedExpressionPtr args = factory->createParenthesizedExpression(*expr->getSourceInfo());
        args->append(expr);
        FunctionCallPtr call = factory->createFunctionCall(*expr->getSourceInfo());
        call->setFunction(ma);
        call->setArguments(args);
        call->setType(optionalType);
        expr = call;
        return ret;
    }
    else
    {
        return false;
    }
}
/*!
 * Returns the final actual type of Optional in a sequence of Optional type chain.
 * e.g. T?????? will return T
 */
TypePtr SemanticAnalyzer::finalTypeOfOptional(const TypePtr& optionalType)
{
    if(optionalType->getCategory() == Type::Specialized && optionalType->getInnerType() != symbolRegistry->getGlobalScope()->Optional())
        return optionalType;
    TypePtr inner = optionalType->getGenericArguments()->get(0);
    return finalTypeOfOptional(inner);
}


/*!
 * This will make implicit type conversion like expanding optional
 */
ExpressionPtr SemanticAnalyzer::transformExpression(const TypePtr& contextualType, ExpressionPtr expr)
{
    SCOPED_SET(t_hint, contextualType);
    expr->accept(this);
    if(!contextualType)
        return expr;
    if(symbolRegistry->getGlobalScope()->isOptional(contextualType))
    {
        ExpressionPtr transformed = expr;
        bool ret = expandOptional(contextualType, transformed);
        if(ret)
            return transformed;
        return expr;
    }
    //if(!expr->getType()->canAssignTo(contextualType))
    //{
    //   error(expr, Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, toString(expr), contextualType->toString());
    //}
    return expr;
}
/*!
 * Gets all functions from current scope to top scope with given name, if flagMasks is specified, only functions
 * with given mask will be returned
 */
std::vector<SymbolPtr> SemanticAnalyzer::allFunctions(const std::wstring& name, int flagMasks, bool allScopes)
{
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    std::vector<SymbolPtr> ret;
    for(; scope; scope = scope->getParentScope())
    {
        SymbolPtr sym = scope->lookup(name);
        if(!sym)
            continue;
        if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
        {
            for(const FunctionSymbolPtr& func : *funcs)
            {
                if((func->getFlags() & flagMasks) == flagMasks)
                    ret.push_back(func);
            }
        }
        else if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(sym))
        {
            if((func->getFlags() & flagMasks) == flagMasks)
                ret.push_back(func);
        }
        else if(sym->getType() && sym->getType()->getCategory() == Type::Function)
        {
            if((sym->getFlags() & flagMasks) == flagMasks)
                ret.push_back(sym);
            else
                continue;
        }
        else if(TypePtr type = dynamic_pointer_cast<Type>(sym))
        {
            ret.push_back(type);
        }
        else
            continue;
        if(!allScopes)
            break;
    }
    return std::move(ret);
}

/*!
 * Declaration finished, added it as a member to current type or current type extension.
 */
void SemanticAnalyzer::declarationFinished(const std::wstring& name, const SymbolPtr& decl)
{
    TypePtr target = currentExtension ? currentExtension : currentType;
    if(target)
    {
        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(target);
        if(currentExtension)
        {
            decl->setFlags(SymbolFlagExtension, true);
        }
        if(name == L"init")
        {
            FunctionSymbolPtr init = dynamic_pointer_cast<FunctionSymbol>(decl);
            assert(init != nullptr);
            FunctionOverloadedSymbolPtr inits = builder->getInitializer();
            if(!inits)
            {
                inits = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(name));
                builder->setInitializer(inits);
            }
            inits->add(init);
        }
        else
        {
            builder->addMember(name, decl);
        }
    }
}

static SymbolPtr getMember(const TypePtr& type, const std::wstring& fieldName, bool staticMember)
{
    if (staticMember)
        return type->getDeclaredStaticMember(fieldName);
    else
        return type->getMember(fieldName);
}
/*!
 * This implementation will try to find the member from the type, and look up from extension as a fallback.
 */
SymbolPtr SemanticAnalyzer::getMemberFromType(const TypePtr& type, const std::wstring& fieldName, bool staticMember)
{
    SymbolPtr ret = getMember(type, fieldName, staticMember);
    SymbolScope* scope = this->symbolRegistry->getFileScope();
    if(!ret && scope)
    {
        TypePtr extension = scope->getExtension(type->getName());
        if(extension)
        {
            assert(extension->getCategory() == Type::Extension);
            ret = getMember(extension, fieldName, staticMember);
        }
    }
    return ret;
}