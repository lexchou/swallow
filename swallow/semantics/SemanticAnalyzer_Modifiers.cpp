/* SemanticAnalyzer_Modifiers.cpp -- Validation of all modifiers are defined here.
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
#include "Symbol.h"
#include "SymbolScope.h"
#include "SymbolRegistry.h"
#include "common/Errors.h"
#include "ScopedNodes.h"
#include "TypeBuilder.h"
#include "common/CompilerResults.h"
#include <cassert>
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "GlobalScope.h"
#include "ast/NodeFactory.h"
#include "common/ScopedValue.h"
#include "swallow_types.h"

USE_SWALLOW_NS
using namespace std;

/*!


    Class         =        1,
            Convenience   =        2,
            Dynamic       =        4,
            Final         =        8,
            Infix         =     0x10,
            Lazy          =     0x20,
            Mutating      =     0x40,
            NonMutating   =     0x80,
            Optional      =    0x100,
            Override      =    0x200,
            Postfix       =    0x400,
            Prefix        =    0x800,
            Required      =   0x1000,
            Static        =   0x2000,
            Unowned       =   0x4000,
            Unowned_Safe  =   0x4000 | 0x8000,
            Unowned_Unsafe=   0x4000 | 0x10000,
            Weak          =  0x20000,
            //Access Level Modifiers
            Internal      =  0x40000,
            Internal_Set  =  0x40000 | 0x80000,
            Private       = 0x100000,
            Private_Set   = 0x100000 | 0x200000,
            Public        = 0x400000,
            Public_Set    = 0x400000 | 0x800000
 */

/*!
 * Return true if the current position is under a type declaration and not in global or function scope
 */
static bool isDeclaredInTypeLevel(SemanticAnalyzer* analyzer)
{
    if(analyzer->getCurrentFunction())
        return false;
    if(analyzer->getCurrentType())
        return true;
    return false;
}

static bool validateGeneralModifiers(SemanticAnalyzer* analyzer, const DeclarationPtr& decl)
{
    int modifiers = decl->getModifiers();
    TypePtr currentType = analyzer->getCurrentType();
    if(modifiers & DeclarationModifiers::Static)
    {
        //static only exists in struct/enum
        if(!isDeclaredInTypeLevel(analyzer))
            return analyzer->error(decl, Errors::E_STATIC_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE);
        if(currentType->getCategory() != Type::Struct && currentType->getCategory() != Type::Enum)
            return analyzer->error(decl, Errors::E_STATIC_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_STRUCTS_AND_ENUMS);

    }
    if(modifiers & DeclarationModifiers::Class)
    {
        //class only exists in class/protocol
        if(!isDeclaredInTypeLevel(analyzer))
            return analyzer->error(decl, Errors::E_CLASS_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE);
        if(currentType->getCategory() != Type::Class && currentType->getCategory() != Type::Protocol)
            return analyzer->error(decl, Errors::E_CLASS_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_CLASSES_AND_PROTOCOLS);
    }
    if(modifiers & (DeclarationModifiers::Mutating | DeclarationModifiers::NonMutating))
    {
        if(decl->getNodeType() != NodeType::Function)
        {
            if(modifiers & DeclarationModifiers::Mutating)
                return analyzer->error(decl, Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, L"mutating", L"func");
            else
                return analyzer->error(decl, Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, L"nonmutating", L"func");
        }
    }
    return true;
}

/*!
 * Add flags to symbols identified by identifier/tuples
 */
static void addFlags(SymbolRegistry* symbolRegistry, const PatternPtr& pattern, SymbolFlags flags)
{
    if(IdentifierPtr id = dynamic_pointer_cast<Identifier>(pattern))
    {
        SymbolPtr sym = symbolRegistry->getCurrentScope()->lookup(id->getIdentifier());
        assert(sym != nullptr);
        sym->setFlags(flags, true);
    }
    else if(TuplePtr tuple = dynamic_pointer_cast<Tuple>(pattern))
    {
        for(const PatternPtr& p : *tuple)
        {
            addFlags(symbolRegistry, p, flags);
        }
    }
    else
    {
        assert(0 && "Cannot add flags to unsupported pattern type");
    }
}
/*!
 * Validate declaration modifiers for value bindings
 */
static bool validateDeclarationModifiers(SemanticAnalyzer* analyzer, const ValueBindingsPtr& bindings)
{
    if(!validateGeneralModifiers(analyzer, bindings))
        return false;
    SymbolRegistry* symbolRegistry = analyzer->getSymbolRegistry();
    TypePtr currentType = analyzer->getCurrentType();
    for(const ValueBindingPtr binding : *bindings)
    {
        if(bindings->hasModifier(DeclarationModifiers::Lazy))
        {
            if (bindings->isReadOnly())
                return analyzer->error(bindings, Errors::E_LAZY_CANNOT_BE_USED_ON_A_LET);
            if (binding->getInitializer() == nullptr)//Lazy properties must have an initializer
                return analyzer->error(binding, Errors::E_LAZY_PROPERTIES_MUST_HAVE_AN_INITIALIZER);
            if (binding->getName()->getNodeType() != NodeType::Identifier)
                return analyzer->error(binding, Errors::E_LAZY_CANNOT_DESTRUCTURE_AN_INITIALIZER);
            if (analyzer->getCurrentFunction() || !currentType || (currentType->getCategory() != Type::Struct && currentType->getCategory() != Type::Class))
                return analyzer->error(binding, Errors::E_LAZY_IS_ONLY_VALID_FOR_MEMBERS_OF_A_STRUCT_OR_CLASS);

            addFlags(symbolRegistry, binding->getName(), SymbolFlagLazy);
        }
        if(bindings->hasModifier(DeclarationModifiers::Class) || bindings->hasModifier(DeclarationModifiers::Static))
        {
            addFlags(symbolRegistry, binding->getName(), SymbolFlagStatic);
        }
    }
    return true;
}
static bool validateDeclarationModifiers(SemanticAnalyzer* analyzer, const ComputedPropertyPtr& prop)
{
    if(!validateGeneralModifiers(analyzer, prop))
        return false;
    if(prop->hasModifier(DeclarationModifiers::Lazy))
        return analyzer->error(prop, Errors::E_LAZY_MAY_NOT_BE_USED_ON_A_COMPUTED_PROPERTY);
    return true;
}
static bool validateDeclarationModifiers(SemanticAnalyzer* analyzer, const FunctionDefPtr& func)
{
    if(!validateGeneralModifiers(analyzer, func))
        return false;
    bool mutating = func->hasModifier(DeclarationModifiers::Mutating);
    bool nonmutating = func->hasModifier(DeclarationModifiers::NonMutating);
    TypePtr currentType = analyzer->getCurrentType();
    if(mutating && nonmutating)
        return analyzer->error(func, Errors::E_METHOD_MAY_NOT_BE_DECLARED_BOTH_MUTATING_AND_NONMUTATING);
    if(!currentType)
    {
        if(mutating || nonmutating)
            return analyzer->error(func, Errors::E_A_IS_ONLY_VALID_ON_METHODS_1, mutating ? L"mutating" : L"nonmutating");
    }
    else
    {
        //the type must be struct or enum
        Type::Category  category = currentType->getCategory();
        //TODO: what is class-bound protocol?
        if(category != Type::Struct && category != Type::Enum && category != Type::Protocol)
        {
            if(mutating || nonmutating)
                return analyzer->error(func, Errors::E_A_ISNT_VALID_ON_METHODS_IN_CLASSES_OR_CLASS_BOUND_PROTOCOLS, mutating ? L"mutating" : L"nonmutating");
        }

    }

    if(func->hasModifier(DeclarationModifiers::Static))
    {
        if(mutating || nonmutating)
            return analyzer->error(func, Errors::E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1, mutating ? L"mutating" : L"nonmutating");
    }
    return true;
}
/*!
 * Validate modifiers for declarations.
 */
void SemanticAnalyzer::validateDeclarationModifiers(const DeclarationPtr& declaration)
{
    NodeType::T nodeType = declaration->getNodeType();
    switch(nodeType)
    {
        case NodeType::ComputedProperty:
        {
            ComputedPropertyPtr property = static_pointer_cast<ComputedProperty>(declaration);
            ::validateDeclarationModifiers(this, property);
            break;
        };
        case NodeType::ValueBindings:
        {
            ValueBindingsPtr bindings = static_pointer_cast<ValueBindings>(declaration);
            ::validateDeclarationModifiers(this, bindings);
            break;
        };
        case NodeType::Function:
        {
            FunctionDefPtr func = static_pointer_cast<FunctionDef>(declaration);
            ::validateDeclarationModifiers(this, func);
            break;
        };
        case NodeType::Class:
        case NodeType::Struct:
        case NodeType::Enum:
        case NodeType::Protocol:
        {
            TypeDeclarationPtr decl = static_pointer_cast<TypeDeclaration>(declaration);
            validateGeneralModifiers(this, decl);
            break;
        };
        default:
            assert(0 && "Unimplemented validation on unsupported node type");
            break;
    }
}