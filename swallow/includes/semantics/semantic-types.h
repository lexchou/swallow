/* semantic-types.h --
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
#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H
#include "swallow_conf.h"
#include <memory>

SWALLOW_NS_BEGIN

class SymbolScope;
//typedef std::shared_ptr<SymbolScope> SymbolScopePtr;
class Type;
typedef std::shared_ptr<Type> TypePtr;
class Symbol;
typedef std::shared_ptr<Symbol> SymbolPtr;
typedef std::shared_ptr<class SymbolPlaceHolder> SymbolPlaceHolderPtr;
class SymbolRegistry;
class ScopeOwner;
typedef std::shared_ptr<ScopeOwner> ScopeOwnerPtr;
class ScopedCodeBlock;
typedef std::shared_ptr<ScopedCodeBlock> ScopedCodeBlockPtr;
class ScopedProgram;
typedef std::shared_ptr<ScopedProgram> ScopedProgramPtr;
class FunctionOverloadedSymbol;
typedef std::shared_ptr<FunctionOverloadedSymbol> FunctionOverloadedSymbolPtr;
class FunctionSymbol;
typedef std::shared_ptr<FunctionSymbol> FunctionSymbolPtr;

//class SymboledFunction;
//typedef std::shared_ptr<class SymboledFunction> SymboledFunctionPtr;

typedef std::shared_ptr<class ScopedClass> ScopedClassPtr;
typedef std::shared_ptr<class ScopedStruct> ScopedStructPtr;
typedef std::shared_ptr<class ScopedEnum> ScopedEnumPtr;
typedef std::shared_ptr<class ScopedExtension> ScopedExtensionPtr;
typedef std::shared_ptr<class ScopedProtocol> ScopedProtocolPtr;
typedef std::shared_ptr<class ScopedClosure> ScopedClosurePtr;
typedef std::shared_ptr<class SymboledVariable> SymboledVariablePtr;
typedef std::shared_ptr<class SymboledConstant> SymboledConstantPtr;


SWALLOW_NS_END

#endif//SEMANTIC_TYPES_H
