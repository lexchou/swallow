/* SwallowCompiler.cpp --
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
#include "SwallowCompiler.h"
#include "semantics/ScopedNodeFactory.h"
#include "semantics/ScopedNodes.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/OperatorResolver.h"
#include "semantics/SemanticAnalyzer.h"
#include "semantics/SemanticContext.h"
#include "semantics/DeclarationAnalyzer.h"
#include "semantics/GlobalScope.h"
#include "semantics/ScopeGuard.h"
#include "semantics/ForwardDeclarationAnalyzer.h"
#include "semantics/GenericDeclarationAnalyzer.h"
#include "semantics/InitializationTracer.h"
#include "semantics/FunctionAnalyzer.h"
#include "common/CompilerResults.h"
#include "parser/Parser.h"
using namespace std;
USE_SWALLOW_NS

class StandardSwallowCompiler : public SwallowCompiler
{
public:
    StandardSwallowCompiler(const wstring& moduleName)
    {
        symbolRegistry = new SymbolRegistry();
        module = ModulePtr(new Module(moduleName, symbolRegistry->getGlobalScope()->getModuleType()));
        initPasses();
    }
    ~StandardSwallowCompiler()
    {
        delete scope;
        destroyPasses();
        delete symbolRegistry;
    }
};


SwallowCompiler* SwallowCompiler::newCompiler(const wstring& moduleName)
{
    return new StandardSwallowCompiler(moduleName);
}

SwallowCompiler::SwallowCompiler()
{
    compilerResults = new CompilerResults();
    nodeFactory = new ScopedNodeFactory();
}
SwallowCompiler::~SwallowCompiler()
{
    delete compilerResults;
    delete nodeFactory;
}

void SwallowCompiler::destroyPasses()
{
    for(SemanticPass* pass : passes)
        delete pass;
    delete ctx;
}
void SwallowCompiler::initPasses()
{

    ctx = new SemanticContext();
    ctx->currentModule = module;
    ctx->lazyDeclaration = true;
    OperatorResolver* operatorResolver = new OperatorResolver(symbolRegistry, compilerResults);
    DeclarationAnalyzer* declarationAnalyzer = new DeclarationAnalyzer(symbolRegistry, compilerResults, ctx);
    SemanticAnalyzer* semanticAnalyzer = new SemanticAnalyzer(symbolRegistry, compilerResults, ctx, declarationAnalyzer);
    declarationAnalyzer->setSemanticAnalyzer(semanticAnalyzer);
    ForwardDeclarationAnalyzer* forwardDeclarationAnalyzer = new ForwardDeclarationAnalyzer(symbolRegistry, compilerResults, declarationAnalyzer);
    GenericDeclarationAnalyzer* genericDeclarationAnalyzer = new GenericDeclarationAnalyzer(symbolRegistry, compilerResults, declarationAnalyzer);
    FunctionAnalyzer* functionAnalyzer = new FunctionAnalyzer(symbolRegistry, compilerResults, ctx, semanticAnalyzer, declarationAnalyzer);

    passes.push_back(operatorResolver);
    passes.push_back(forwardDeclarationAnalyzer);
    passes.push_back(genericDeclarationAnalyzer);
    passes.push_back(declarationAnalyzer);
    passes.push_back(semanticAnalyzer);
    passes.push_back(functionAnalyzer);
    scope = new SymbolScope();
}
void SwallowCompiler::addSourceFile(const SourceFilePtr& sourceFile)
{
    sourceFiles.push_back(sourceFile);
}
void SwallowCompiler::addSource(const wstring& name, const wstring& code)
{
    addSourceFile(SourceFilePtr(new SourceFile(name, code)));
}
void SwallowCompiler::clearSources()
{
    sourceFiles.clear();
}

ProgramPtr SwallowCompiler::createProgramNode()
{
    ScopedProgramPtr ret(new ScopedProgram());
    ret->setScope(scope);
    return ret;
}

bool SwallowCompiler::compile()
{
    std::vector<ProgramPtr> programs;
    return compile(programs);
}
ParserPtr SwallowCompiler::createParser()
{
    return ParserPtr(new Parser(nodeFactory, compilerResults));
}
bool SwallowCompiler::compile(std::vector<ProgramPtr>& programs)
{
    programs.clear();
    try
    {
        ScopeGuard g(symbolRegistry, scope);
        symbolRegistry->setFileScope(scope);
        for(const SourceFilePtr& source : sourceFiles)
        {
            ProgramPtr program = createProgramNode();
            programs.push_back(program);
            ParserPtr parser = createParser();
            parser->setSourceFile(source);
            if(!parser->parse(source->code.c_str(), program))
                throw Abort();

            InitializationTracer tracer(nullptr, InitializationTracer::Sequence);
            SCOPED_SET(ctx->currentInitializationTracer, &tracer);

            for(SemanticPass* pass : passes)
            {
                program->accept(pass);
            }
        }
        symbolRegistry->setFileScope(nullptr);
    }
    catch(const Abort&)
    {
        symbolRegistry->setFileScope(nullptr);
        //TODO: refactor compiler
        // ScopedProgram shouldn't maintaince its own scope.
        for(ProgramPtr p : programs)
        {
            if(ScopedProgramPtr sp = dynamic_pointer_cast<ScopedProgram>(p))
            {
                sp->setScope(scope);
            }
        }
        return false;
    }
    return true;
}
ModulePtr SwallowCompiler::getModule()
{
    return module;
}
ProgramPtr SwallowCompiler::getProgram()
{
    return program;
}
CompilerResults* SwallowCompiler::getCompilerResults()
{
    return compilerResults;
}

SymbolRegistry* SwallowCompiler::getSymbolRegistry()
{
    return symbolRegistry;
}
SymbolScope* SwallowCompiler::getScope()
{
    return scope;
}
