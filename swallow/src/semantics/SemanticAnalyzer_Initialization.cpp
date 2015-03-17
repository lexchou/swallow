/* SemanticAnalyzer_Initialization.cpp --
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
#include "semantics/SemanticAnalyzer.h"
#include "semantics/GlobalScope.h"
#include "semantics/SymbolRegistry.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include "semantics/TypeBuilder.h"
#include <cassert>
#include <algorithm>
#include "ast/NodeFactory.h"
#include "semantics/CollectionTypeAnalyzer.h"
#include <iostream>
#include "common/ScopedValue.h"
#include "semantics/InitializationTracer.h"

USE_SWALLOW_NS
using namespace std;
/*!
 * Validate initializer delegation
 */
void SemanticAnalyzer::validateInitializerDelegation(const FunctionCallPtr& node)
{
    MemberAccessPtr ma = static_pointer_cast<MemberAccess>(node->getFunction());
    const wstring& name = static_pointer_cast<Identifier>(ma->getSelf())->getIdentifier();
    bool self_init = name == L"self";
    bool super_init = name == L"super";
    bool insideInit = ctx.currentFunction && ctx.currentFunction->hasFlags(SymbolFlagInit);
    //calling a super.init or super.init outside the initializer
    if(!insideInit)
    {
        if(self_init)
        {
            error(ma, Errors::E_INITIALIZER_DELEGATION_CAN_ONLY_OCCUR_WITHIN_AN_INITIALIZER);
            return;
        }
        if(super_init)
        {
            error(ma, Errors::E_SUPER_INIT_CANNOT_BE_CALLED_OUTSIDE_OF_AN_INITIALIZER);
            return;
        }
        error(ma, Errors::E_INIT_CAN_ONLY_REFER_TO_THE_INITIALIZERS_OF_SELF);
        return;
    }
    //inside initializer, apply delegation's safe check

    //Initializer Delegation Safety check 1: A designated initializer must
    //ensure that all of the properties introduced by its class are
    //initialized before it delegates up to a superclass initializer.
    assert(ctx.currentType != nullptr);
    if(super_init)
    {
        for (const SymbolPtr &prop : ctx.currentType->getDeclaredStoredProperties())
        {
            if (!prop->hasFlags(SymbolFlagInitialized))
            {
                error(node, Errors::E_PROPERTY_A_NOT_INITIALIZED_AT_SUPER_INIT_CALL_1, L"self." + prop->getName());
                return;
            }
        }
    }

    if(ctx.currentInitializationTracer)
    {
        if(self_init)
            ctx.currentInitializationTracer->selfInit = true;
        if(super_init)
            ctx.currentInitializationTracer->superInit = true;
    }


}
/*!
 * Validate initializer delegation safety check
 */
void SemanticAnalyzer::validateInitializerDelegation(const MemberAccessPtr& node)
{
    //accessing member of self(not accessing initializer) inside an initializer
    if(!Identifier::is(node->getSelf(), L"self"))
        return;
    if(!ctx.currentFunction || !ctx.currentFunction->hasFlags(SymbolFlagInit))
        return;
    if(!node->getField() || node->getField()->getIdentifier() == L"init")
        return;
    assert(ctx.currentInitializationTracer != nullptr);
    InitializationTracer* tracer = ctx.currentInitializationTracer;
    TypePtr memberDeclaringType = nullptr;
    SymbolPtr member = getMemberFromType(ctx.currentType, node->getField()->getIdentifier(), (MemberFilter) (FilterLookupInExtension | FilterRecursive), &memberDeclaringType);
    bool declaredBySuperClass = ctx.currentType != memberDeclaringType;
    //assigning it on a field declared by base class but not yet called super.init
    bool convenience = ctx.currentFunction->hasFlags(SymbolFlagConvenienceInit);
    bool designated = !convenience;
    if(designated && declaredBySuperClass && !tracer->superInit)
    {
        //Initializer Delegation Safety Check 2
        //A designated initializer must delegate up to a superclass initializer
        //before assigning a value to an inherited property. If it doesn’t, the
        //new value the designated initializer assigns will be overwritten by
        //the superclass as part of its own initialization.
        error(node, Errors::E_USE_OF_PROPERTY_A_IN_BASE_OBJECT_BEFORE_SUPER_INIT_INITIALIZES_IT, node->getField()->getIdentifier());
        return;
    }
    if(convenience && !tracer->selfInit)
    {
        //Initializer Delegation Safety check 3
        //A convenience initializer must delegate to another initializer before
        //assigning a value to any property (including properties defined by
        //the same class). If it doesn’t, the new value the convenience initializer
        //assigns will be overwritten by its own class’s designated initializer.
        error(node, Errors::E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED);
        return;
    }
    //Initializer Delegation Safety check 4
    //An initializer cannot call any instance methods, read the values of any
    //instance properties, or refer to self as a value until after the first
    //phase of initialization is complete.
    if (designated && ctx.currentType->getParentType() != nullptr)
    {
        //super.init must be called
        if (declaredBySuperClass && !tracer->superInit)
        {
            error(node, Errors::E_SELF_USED_BEFORE_SUPER_INIT_CALL);
            return;
        }
        //NOTE: this check exists whatever if the member defined in current type or super type
        //I'm not sure if it's official implementation's bug
        if(node->getParentNode()->getNodeType() == NodeType::FunctionCall && !tracer->superInit)
        {
            error(node, Errors::E_SELF_USED_BEFORE_SUPER_INIT_CALL);
            return;
        }
    }
    else if (convenience && !tracer->selfInit)
    {
        error(node, Errors::E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED);
        return;
    }
}

