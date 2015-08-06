/* SwallowCompiler.h --
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
#ifndef SWALLOW_COMPILER_H
#define SWALLOW_COMPILER_H
#include "swallow_conf.h"
#include <vector>
SWALLOW_NS_BEGIN


class CompilerResults;
class SymbolRegistry;
class NodeFactory;
class SymbolScope;
class SemanticPass;
struct SemanticContext;
typedef std::shared_ptr<struct SourceFile> SourceFilePtr;
typedef std::shared_ptr<class Module> ModulePtr;
typedef std::shared_ptr<class Parser> ParserPtr;
typedef std::shared_ptr<class Program> ProgramPtr;
using std::wstring;

class SWALLOW_EXPORT SwallowCompiler
{
public:
    static SwallowCompiler* newCompiler(const wstring& moduleName);
public:
    virtual ~SwallowCompiler();
protected:
    SwallowCompiler();
    void initPasses();
    void destroyPasses();

public:
    void addSourceFile(const SourceFilePtr& sourceFile);
    void addSource(const wstring& name, const wstring& code);
    void clearSources();
public:
    bool compile();
    bool compile(std::vector<ProgramPtr>& programs);
public:
    ModulePtr getModule();
    ProgramPtr getProgram();
    CompilerResults* getCompilerResults();
    SymbolRegistry* getSymbolRegistry();
    SymbolScope* getScope();

protected:
    virtual ProgramPtr createProgramNode();
    virtual ParserPtr createParser();
protected:
    SymbolRegistry* symbolRegistry;
    NodeFactory* nodeFactory;

    std::vector<SemanticPass*> passes;
    std::vector<SourceFilePtr> sourceFiles;
    SemanticContext* ctx;

    //results:
    CompilerResults* compilerResults;
    ModulePtr module;
    ProgramPtr program;
    SymbolScope* scope;
};

SWALLOW_NS_END
#endif//SWALLOW_COMPILER_H
