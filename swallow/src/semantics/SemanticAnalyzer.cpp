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
#include "semantics/SemanticAnalyzer.h"
#include "semantics/InitializationTracer.h"
#include <cassert>
#include "semantics/SymbolScope.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "semantics/GenericDefinition.h"
#include "semantics/GenericArgument.h"
#include <string>
#include "semantics/TypeBuilder.h"
#include "common/CompilerResults.h"
#include "semantics/SymbolRegistry.h"
#include "ast/utils/NodeSerializer.h"
#include "semantics/GlobalScope.h"
#include "ast/NodeFactory.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "common/ScopedValue.h"
#include <set>
#include <semantics/Symbol.h>
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/LazyDeclaration.h"
#include "semantics/TypeResolver.h"
#include "semantics/ForwardDeclarationAnalyzer.h"
#include "semantics/SemanticContext.h"

USE_SWALLOW_NS
using namespace std;


SemanticAnalyzer::SemanticAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults, SemanticContext* context, DeclarationAnalyzer* declarationAnalyzer)
:SemanticPass(symbolRegistry, compilerResults)
{
    ctx = context;
    this->declarationAnalyzer = declarationAnalyzer;
}
SemanticAnalyzer::~SemanticAnalyzer()
{
}

std::wstring SemanticAnalyzer::generateTempName()
{
    std::wstringstream ss;
    ss<<L"#"<<ctx->numTemporaryNames++;
    return ss.str();
}

/*!
 * Check if the declaration is declared as global
 */
bool SemanticAnalyzer::isGlobal(const DeclarationPtr& node)
{
    NodePtr parent = node->getParentNode();
    bool ret(parent == nullptr || parent->getNodeType() == NodeType::Program);
    return ret;
}

static std::wstring getDeclarationName(const DeclarationPtr& node)
{
    if(FunctionDefPtr func = dynamic_pointer_cast<FunctionDef>(node))
    {
        return func->getName();
    }
    if(TypeDeclarationPtr type = dynamic_pointer_cast<TypeDeclaration>(node))
    {
        return type->getIdentifier()->getName();
    }
    if(TypeAliasPtr alias = dynamic_pointer_cast<TypeAlias>(node))
    {
        return alias->getName();
    }
    assert(0 && "Invalid declaration type");
    return L"";
}
/*!
  * Mark this declaration node as lazy declaration, it will be processed only when being used or after the end of the program.
  */
void SemanticAnalyzer::delayDeclare(const DeclarationPtr& node)
{
    //map<wstring, list<DeclarationPtr>> lazyDeclarations;
    std::wstring name = getDeclarationName(node);
    auto iter = ctx->lazyDeclarations.find(name);
    //wprintf(L"Delay declare %S\n", name.c_str());
    if(iter == ctx->lazyDeclarations.end())
    {
        LazyDeclarationPtr decls(new LazyDeclaration());
        ctx->lazyDeclarations.insert(make_pair(name, decls));
        decls->addDeclaration(symbolRegistry, node);
        return;
    }
    iter->second->addDeclaration(symbolRegistry, node);
}
/*!
 * The declarations that marked as lazy will be declared immediately
 */
void SemanticAnalyzer::declareImmediately(const std::wstring& name)
{
    auto entry = ctx->lazyDeclarations.find(name);
    if(entry == ctx->lazyDeclarations.end())
        return;
    LazyDeclarationPtr decls = entry->second;
    ctx->lazyDeclarations.erase(entry);
    SymbolScope* currentScope = symbolRegistry->getCurrentScope();
    SymbolScope* fileScope = symbolRegistry->getFileScope();
    try
    {
        SCOPED_SET(ctx->lazyDeclaration, false);
        SymbolPtr symbol = nullptr;
        //wprintf(L"Declare immediately %S %d definitions\n", name.c_str(), decls->size());
        for(auto decl : *decls)
        {
            //wprintf(L"   fs:%p cs:%p\n", decl.fileScope, decl.currentScope);
            symbolRegistry->setCurrentScope(decl.currentScope);
            symbolRegistry->setFileScope(decl.fileScope);
            SCOPED_SET(this->ctx->currentType, nullptr);
            SCOPED_SET(this->ctx->currentFunction, nullptr);
            SCOPED_SET(this->ctx->contextualType, nullptr);
            SCOPED_SET(this->ctx->currentExtension, nullptr);
            SCOPED_SET(this->ctx->currentCodeBlock, nullptr);
            SCOPED_SET(this->ctx->currentInitializationTracer, nullptr);
            SCOPED_SET(this->ctx->flags, SemanticContext::FLAG_PROCESS_DECLARATION | SemanticContext::FLAG_PROCESS_IMPLEMENTATION);
            decl.node->accept(this);
            if(!symbol)
                symbol = decl.currentScope->lookup(name, false);

        }
        if(symbol && symbol->getKind() == SymbolKindType)
        {
            TypePtr type = static_pointer_cast<Type>(symbol);
            declarationAnalyzer->verifyProtocolConform(type, true);
        }
    }
    catch(...)
    {
        symbolRegistry->setCurrentScope(currentScope);
        symbolRegistry->setFileScope(fileScope);
        throw;
    }
    symbolRegistry->setCurrentScope(currentScope);
    symbolRegistry->setFileScope(fileScope);
}
bool SemanticAnalyzer::resolveLazySymbol(const std::wstring& name)
{
    auto entry = ctx->lazyDeclarations.find(name);
    if(entry == ctx->lazyDeclarations.end())
    {
        //wprintf(L"Cannot resolve lazy symbol %S\n", name.c_str());
        /*
        for(auto entry : lazyDeclarations)
        {
            wprintf(L"%S, ", entry.first.c_str());
        }
        wprintf(L"\n");
        */
        return false;
    }
    declareImmediately(name);
    return true;
}
void SemanticAnalyzer::visitProgram(const ProgramPtr& node)
{
    InitializationTracer tracer(nullptr, InitializationTracer::Sequence);
    SCOPED_SET(ctx->currentInitializationTracer, &tracer);

    SemanticPass::visitProgram(node);
    //now we'll deal with the lazy declaration of functions and classes
    finalizeLazyDeclaration();
    verifyProtocolConforms();
}

/*!
    * Verification of protocol confirmation in all types of current module
    */
void SemanticAnalyzer::verifyProtocolConforms()
{
    for(const TypePtr& type : ctx->allTypes)
    {
        if(type->getCategory() == Type::Protocol)
            continue;
        declarationAnalyzer->verifyProtocolConform(type, false);
    }
}

static void resolveTypeAlias(const TypePtr& type)
{
    if(type->getCategory() == Type::Alias)
    {
        type->resolveAlias();
        return;
    }
    //check nested types
    for(auto entry : type->getAssociatedTypes())
    {
        resolveTypeAlias(entry.second);
    }
}
void SemanticAnalyzer::finalizeLazyDeclaration()
{
    ctx->lazyDeclaration = false;
    SymbolScope* scope = symbolRegistry->getCurrentScope();
    while(!ctx->lazyDeclarations.empty())
    {
        std::wstring symbolName = ctx->lazyDeclarations.begin()->first;
        //lazyDeclarations.erase(lazyDeclarations.begin());
        declareImmediately(symbolName);
    }
    symbolRegistry->setCurrentScope(scope);
    //now we make all typealias to resolve its type
    for(auto entry : scope->getSymbols())
    {
        if(entry.second->getKind() != SymbolKindType)
            continue;
        TypePtr type = static_pointer_cast<Type>(entry.second);
        resolveTypeAlias(type);
    }
}

TypePtr SemanticAnalyzer::lookupType(const TypeNodePtr& type, bool supressErrors)
{
    TypeResolver resolver(symbolRegistry, supressErrors ? nullptr : this, this, ctx, false);
    return resolver.lookupType(type);
}
/*!
 * Convert expression node to type node
 */
TypeNodePtr SemanticAnalyzer::expressionToType(const ExpressionPtr& expr)
{
    assert(expr != nullptr);
    NodeType nodeType = expr->getNodeType();
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
    if(optionalType->getCategory() == Type::Specialized && (optionalType->getInnerType() == global->Optional() || optionalType->getInnerType() == global->ImplicitlyUnwrappedOptional()))
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
    if(optionalType->getCategory() == Type::Specialized)
    {
        TypePtr innerType = optionalType->getInnerType();
        GlobalScope* g = symbolRegistry->getGlobalScope();
        if(innerType != g->Optional() && innerType != g->ImplicitlyUnwrappedOptional())
            return optionalType;
    }
    TypePtr inner = optionalType->getGenericArguments()->get(0);
    return finalTypeOfOptional(inner);
}
/*!
 * Expand variable access into self.XXXX if XXXX is defined in type hierachy
 */
ExpressionPtr SemanticAnalyzer::expandSelfAccess(const ExpressionPtr& expr)
{
    NodeType nodeType = expr->getNodeType();
    switch(nodeType)
    {
        case NodeType::Identifier:
        {
            IdentifierPtr id = static_pointer_cast<Identifier>(expr);
            const std::wstring& name = id->getIdentifier();
            SymbolPtr sym = symbolRegistry->lookupSymbol(name);
            if(sym && !sym->hasFlags(SymbolFlagMember))
                return expr;// it refers to a non-member symbol
            if(!ctx->currentFunction || ctx->currentFunction->hasFlags(SymbolFlagStatic))
                return expr;// self access will not be expanded within a static/class method
            if(!sym || sym->hasFlags(SymbolFlagMember))
            {
                //check if it's defined in super class
                TypePtr type = ctx->currentType;
                bool found = false;
                for(; type; type = type->getParentType())
                {
                    if(type->getDeclaredMember(name))
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)//it's not a member, do not wrap it
                    return expr;
                //it's a member or defined in super class, wrap it as member access
                NodeFactory* factory = expr->getNodeFactory();
                IdentifierPtr self = factory->createIdentifier(*expr->getSourceInfo());
                self->setIdentifier(L"self");
                MemberAccessPtr ma = factory->createMemberAccess(*expr->getSourceInfo());
                ma->setSelf(self);
                ma->setField(id);
                return ma;
            }
            return expr;
        }
        default:
            //TODO other expressions is not supported yet.
            return expr;
    }
}


/*!
 * This will make implicit type conversion like expanding optional
 */
ExpressionPtr SemanticAnalyzer::transformExpression(const TypePtr& contextualType, ExpressionPtr expr)
{
    SCOPED_SET(ctx->contextualType, contextualType);
    expr = expandSelfAccess(expr);
    expr->accept(this);
    if(!contextualType)
        return expr;
    GlobalScope* global = symbolRegistry->getGlobalScope();
    if(global->isOptional(contextualType) || global->isImplicitlyUnwrappedOptional(contextualType))
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
            type = type->resolveAlias();
            assert(type != nullptr);
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
void SemanticAnalyzer::declarationFinished(const std::wstring& name, const SymbolPtr& decl, const NodePtr& node)
{
    //verify if this has been declared in the type or its extension
    if(ctx->currentType)
    {
        int filter = FilterRecursive | FilterLookupInExtension;
        if(decl->hasFlags(SymbolFlagStatic))
            filter |= FilterStaticMember;

        SymbolPtr m = getMemberFromType(ctx->currentType, name, (MemberFilter)filter);

        if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(decl))
        {
            bool isFunc = dynamic_pointer_cast<FunctionSymbol>(m) || dynamic_pointer_cast<FunctionOverloadedSymbol>(m);
            if(!isFunc && m)
            {
                error(node, Errors::E_INVALID_REDECLARATION_1, name);
                return;
            }
        }
        else if(dynamic_pointer_cast<ComputedProperty>(node))
        {

        }
        else if (m)
        {
            error(node, Errors::E_INVALID_REDECLARATION_1, name);
            return;
        }
    }



    TypePtr target = ctx->currentExtension ? ctx->currentExtension : ctx->currentType;
    if(target)
    {
        TypeBuilderPtr builder = static_pointer_cast<TypeBuilder>(target);
        if(ctx->currentExtension)
        {
            decl->setFlags(SymbolFlagExtension, true);
        }
        if(name == L"init")
        {
            FunctionSymbolPtr init = dynamic_pointer_cast<FunctionSymbol>(decl);
            assert(init != nullptr);
            FunctionOverloadedSymbolPtr inits = builder->getDeclaredInitializer();
            if(!inits)
            {
                inits = FunctionOverloadedSymbolPtr(new FunctionOverloadedSymbol(name));
                builder->setInitializer(inits);
            }
            //inits->add(init);
        }
        //else
        {
            builder->addMember(name, decl);
        }
    }
}

static SymbolPtr getMember(const TypePtr& type, const std::wstring& fieldName, MemberFilter filter)
{
    if (filter & FilterStaticMember)
        return type->getDeclaredStaticMember(fieldName);
    else
        return type->getDeclaredMember(fieldName);
}
/*!
 * This implementation will try to find the member from the type, and look up from extension as a fallback.
 */
SymbolPtr SemanticAnalyzer::getMemberFromType(const TypePtr& type, const std::wstring& fieldName, MemberFilter filter, TypePtr* declaringType)
{
    SymbolPtr ret = getMember(type, fieldName, filter);
    SymbolScope* scope = this->symbolRegistry->getFileScope();
    if(!ret && scope)
    {
        if(filter & FilterLookupInExtension)
        {
            std::vector<TypePtr>* extensions = nullptr;
            if (scope->getExtension(type->getName(), &extensions))
            {
                for(const TypePtr& extension : *extensions)
                {
                    assert(extension->getCategory() == Type::Extension);
                    ret = getMember(extension, fieldName, filter);
                }
            }
        }
    }
    if(ret && declaringType)
        *declaringType = type;
    if(!ret && (filter & FilterRecursive) && type->getParentType())
    {
        ret = getMemberFromType(type->getParentType(), fieldName, filter, declaringType);
    }
    return ret;
}

struct FunctionSymbolWrapper
{
    SymbolPtr symbol;
    FunctionSymbolWrapper(const SymbolPtr& symbol)
            :symbol(symbol)
    {}
    bool operator<(const FunctionSymbolWrapper& rhs) const
    {
        return Type::compare(symbol->getType(), rhs.symbol->getType()) < 0;
    }
};
static void addCandidateMethod(std::set<FunctionSymbolWrapper>& result, const FunctionSymbolPtr& func)
{
    auto iter = result.find(FunctionSymbolWrapper(func));
    if(iter == result.end())
        result.insert(FunctionSymbolWrapper(func));
}

/*!
 * This will extract all methods that has the same name in the given type(including all base types)
 */
static void loadMethods(const TypePtr& type, const std::wstring& fieldName, MemberFilter filter, std::set<FunctionSymbolWrapper>& result)
{
    SymbolPtr sym;
    if (filter & FilterStaticMember)
        sym = type->getDeclaredStaticMember(fieldName);
    else
        sym = type->getDeclaredMember(fieldName);
    if(!sym)
        return;
    if(FunctionOverloadedSymbolPtr funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(sym))
    {
        for(auto func : *funcs)
            addCandidateMethod(result, func);
    }
    else if(FunctionSymbolPtr func = dynamic_pointer_cast<FunctionSymbol>(sym))
    {
        addCandidateMethod(result, func);
    }
    TypePtr parent = type->getParentType();
    if(parent && (filter & FilterRecursive))
    {
       loadMethods(parent, fieldName, filter, result);
    }
}
/*!
 * This implementation will try to all methods from the type, including defined in parent class or extension
 */
void SemanticAnalyzer::getMethodsFromType(const TypePtr& type, const std::wstring& fieldName, MemberFilter filter, std::vector<SymbolPtr>& result)
{
    std::set<FunctionSymbolWrapper> functions;
    loadMethods(type, fieldName, filter, functions);
    SymbolScope* scope = this->symbolRegistry->getFileScope();
    if(scope)
    {
        if(filter & FilterLookupInExtension)
        {
            std::vector<TypePtr> *extensions = nullptr;
            if(scope->getExtension(type->getName(), &extensions))
            {
                for(const TypePtr& extension : *extensions)
                {
                    assert(extension->getCategory() == Type::Extension);
                    loadMethods(extension, fieldName, filter, functions);
                }
            }
        }
    }
    for(const FunctionSymbolWrapper& wrapper : functions)
    {
        result.push_back(wrapper.symbol);
    }
}

/*!
 * Mark specified symbol initialized.
 */
void SemanticAnalyzer::markInitialized(const SymbolPtr& sym)
{
    if(sym->hasFlags(SymbolFlagInitialized))
        return;
    sym->setFlags(SymbolFlagInitialized, true);
    if(ctx->currentInitializationTracer)
    {
        ctx->currentInitializationTracer->add(sym);
    }
}
/*!
 * Make a chain of member access on a tuple variable(specified by given tempName) by a set of indices
 */
static MemberAccessPtr makeAccess(SourceInfo* info, NodeFactory* nodeFactory, const std::wstring& tempName, const std::vector<int>& indices)
{
    assert(!indices.empty());
    IdentifierPtr self = nodeFactory->createIdentifier(*info);
    self->setIdentifier(tempName);
    ExpressionPtr ret = self;
    for(int i : indices)
    {
        MemberAccessPtr next = nodeFactory->createMemberAccess(*info);
        next->setSelf(ret);
        next->setIndex(i);
        ret = next;
    }
    return static_pointer_cast<MemberAccess>(ret);
}

void SemanticAnalyzer::expandTuple(vector<TupleExtractionResult>& results, vector<int>& indices, const PatternPtr& name, const std::wstring& tempName, const TypePtr& type, PatternAccessibility accessibility)
{
    TypeNodePtr declaredType;
    switch (name->getNodeType())
    {
        case NodeType::Identifier:
        {
            IdentifierPtr id = static_pointer_cast<Identifier>(name);
            if(accessibility == AccessibilityUndefined)
            {
                //undefined variable
                error(name, Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, id->getIdentifier());
                abort();
                return;
            }
            name->setType(type);
            bool readonly = accessibility == AccessibilityConstant;
            results.push_back(TupleExtractionResult(id, type, makeAccess(id->getSourceInfo(), id->getNodeFactory(), tempName, indices), readonly));
            //check if the identifier already has a type definition
            break;
        }
        case NodeType::TypedPattern:
        {
            TypedPatternPtr pat = static_pointer_cast<TypedPattern>(name);
            assert(pat->getDeclaredType());
            TypePtr declaredType = lookupType(pat->getDeclaredType());
            pat->setType(declaredType);
            if(!Type::equals(declaredType, type))
            {
                error(name, Errors::E_TYPE_ANNOTATION_DOES_NOT_MATCH_CONTEXTUAL_TYPE_A_1, type->toString());
                abort();
                return;
            }
            expandTuple(results, indices, pat->getPattern(), tempName, declaredType, accessibility);
            break;
        }
        case NodeType::Tuple:
        {
            if(type && type->getCategory() != Type::Tuple)
            {
                error(name, Errors::E_TUPLE_PATTERN_CANNOT_MATCH_VALUES_OF_THE_NON_TUPLE_TYPE_A_1, type->toString());
                return;
            }
            TuplePtr tuple = static_pointer_cast<Tuple>(name);
            declaredType = tuple->getDeclaredType();
            if(type)
            {
                if((type->getCategory() != Type::Tuple) || (tuple->numElements() != type->numElementTypes()))
                {
                    error(name, Errors::E_TYPE_ANNOTATION_DOES_NOT_MATCH_CONTEXTUAL_TYPE_A_1, type->toString());
                    abort();
                    return;
                }
            }
            //check each elements
            int elements = tuple->numElements();
            for(int i = 0; i < elements; i++)
            {
                PatternPtr element = tuple->getElement(i);
                TypePtr elementType = type ? type->getElementType(i) : nullptr;
                indices.push_back(i);
                //validateTupleType(isReadonly, element, elementType);
                expandTuple(results, indices, element, tempName, elementType, accessibility);
                indices.pop_back();
            }
            break;
        }
        case NodeType::ValueBindingPattern:
        {
            if(accessibility != AccessibilityUndefined)
            {
                error(name, Errors::E_VARLET_CANNOT_APPEAR_INSIDE_ANOTHER_VAR_OR_LET_PATTERN_1, accessibility == AccessibilityConstant ? L"let" : L"var");
                abort();
            }
            else
            {
                ValueBindingPatternPtr p = static_pointer_cast<ValueBindingPattern>(name);
                expandTuple(results, indices, p->getBinding(), tempName, type, p->isReadOnly() ? AccessibilityConstant : AccessibilityVariable);
            }
            break;
        }
        case NodeType::EnumCasePattern:
        {
            EnumCasePatternPtr p(static_pointer_cast<EnumCasePattern>(name));
            if(p->getAssociatedBinding())
            {
                expandTuple(results, indices, p->getAssociatedBinding(), tempName, type, accessibility);
            }
            break;
        }
        case NodeType::TypeCase:
        case NodeType::TypeCheck:
        default:
            error(name, Errors::E_EXPECT_TUPLE_OR_IDENTIFIER);
            break;
    }
}
