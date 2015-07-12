/* FunctionAnalyzer.h --
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
#ifndef FUNCTION_ANALYZER_H
#define FUNCTION_ANALYZER_H
#include "SemanticPass.h"
#include "Type.h"
#include <list>
#include "SymbolScope.h"

SWALLOW_NS_BEGIN

class SemanticAnalyzer;
class DeclarationAnalyzer;
struct SemanticContext;
class SWALLOW_EXPORT FunctionAnalyzer : public SemanticPass
{
    friend class DeclarationAnalyzer;
public:
    FunctionAnalyzer(SymbolRegistry* symbolRegistry, CompilerResults* compilerResults, SemanticContext* context, SemanticAnalyzer* semanticAnalyzer, DeclarationAnalyzer* declarationAnalyzer);
public:
    virtual void visitExtension(const ExtensionDefPtr& node) override;
    virtual void visitProtocol(const ProtocolDefPtr& node) override;
    virtual void visitStruct(const StructDefPtr& node) override;
    virtual void visitClass(const ClassDefPtr& node) override;
    virtual void visitEnum(const EnumDefPtr& node) override;
public:
    virtual void visitClosure(const ClosurePtr& node) override;
    virtual void visitFunction(const FunctionDefPtr& node) override;
    virtual void visitDeinit(const DeinitializerDefPtr& node) override;
    virtual void visitInit(const InitializerDefPtr& node) override;
    virtual void visitComputedProperty(const ComputedPropertyPtr& node) override;
    virtual void visitCodeBlock(const CodeBlockPtr &node) override;
    virtual void visitSubscript(const SubscriptDefPtr &node) override;
    void visitAccessor(const CodeBlockPtr& accessor, const ParametersNodePtr& params, const SymbolPtr& setter, int modifiers);
protected:
    SemanticContext* ctx;
    SemanticAnalyzer* semanticAnalyzer;
    DeclarationAnalyzer* declarationAnalyzer;
};

SWALLOW_NS_END




#endif//SEMANTIC_ANALYZER_H
