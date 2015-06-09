/* TypeResolver.h --
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
#ifndef TYPE_RESOLVER_H
#define TYPE_RESOLVER_H
#include <memory>
#include "swallow_conf.h"

SWALLOW_NS_BEGIN

    class CompilerResultEmitter;
    class GlobalScope;
    class SymbolRegistry;
    struct LazySymbolResolver;
    class SymbolScope;
    struct SemanticContext;
    typedef std::shared_ptr<class Type> TypePtr;
    typedef std::shared_ptr<class TypeNode> TypeNodePtr;
    typedef std::shared_ptr<class TypeIdentifier> TypeIdentifierPtr;
    typedef std::shared_ptr<class TupleType> TupleTypePtr;
    typedef std::shared_ptr<class ArrayType> ArrayTypePtr;
    typedef std::shared_ptr<class DictionaryType> DictionaryTypePtr;
    typedef std::shared_ptr<class Module> ModulePtr;
    typedef std::shared_ptr<class Node> NodePtr;
    typedef std::shared_ptr<class OptionalType> OptionalTypePtr;
    typedef std::shared_ptr<class ImplicitlyUnwrappedOptional> ImplicitlyUnwrappedOptionalPtr;
    typedef std::shared_ptr<class FunctionType> FunctionTypePtr;
    typedef std::shared_ptr<class ProtocolComposition> ProtocolCompositionPtr;
    class SWALLOW_EXPORT TypeResolver
    {
    public:
        TypeResolver(SymbolRegistry* registry, CompilerResultEmitter* resultEmitter, LazySymbolResolver* symbolResolver, SemanticContext* ctx);
    public:
        TypePtr lookupType(const TypeNodePtr& node);
        static bool assertGenericArgumentMatches(CompilerResultEmitter* emitter, const NodePtr& node, int expected, int actual);
    private:
        TypePtr resolve(const TypeNodePtr& typeNode);
        TypePtr resolveIdentifier(TypeIdentifierPtr id);
        TypePtr resolveIdentifier(const ModulePtr& module, TypeIdentifierPtr id);
        TypePtr resolveTuple(const TupleTypePtr& tuple);
        TypePtr resolveArray(const ArrayTypePtr& array);
        TypePtr resolveDictionaryType(const DictionaryTypePtr& dict);
        TypePtr resolveOptionalType(const OptionalTypePtr& opt);
        TypePtr resolveImplicitlyUnwrappedOptional(const ImplicitlyUnwrappedOptionalPtr& opt);
        TypePtr resolveFunctionType(const FunctionTypePtr& func);
        TypePtr resolveProtocolComposition(const ProtocolCompositionPtr& composition);
        TypePtr handleGeneric(const TypePtr& type, const TypeIdentifierPtr& id);

    private:
        CompilerResultEmitter* emitter;
        SymbolRegistry* symbolRegistry;
        LazySymbolResolver* symbolResolver;
        SemanticContext* ctx;
        SymbolScope* lookupScope;
    };

SWALLOW_NS_END


#endif//TYPE_RESOLVER_H
