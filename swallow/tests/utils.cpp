/* utils.cpp --
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
#include "utils.h"
#include "ast/utils/NodeSerializer.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodeFactory.h"
#include "semantics/SemanticAnalyzer.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include <sstream>
#include <fstream>
#include "common/Errors.h"
#include <semantics/GlobalScope.h>
#include "common/SwallowUtils.h"
#include "ast/utils/ASTHierachyDumper.h"
#include "semantics/OperatorResolver.h"
#include <iostream>

using namespace std;
USE_SWALLOW_NS
static bool opt_dumpAST = false;


void testInit(int argc, char** argv)
{
    for(int i = 1; i < argc; i++)
    {
        const char* arg = argv[i];
        if(!strcmp("--ast", arg))
        opt_dumpAST = true;
    }
    testing::InitGoogleTest(&argc, argv);
}


wstring readFile(const char* fileName)
{
    return SwallowUtils::readFile(fileName);
}

void dumpCompilerResults(Swallow::CompilerResults& compilerResults)
{
    SwallowUtils::dumpCompilerResults(compilerResults, std::wcout);
}


const CompilerResult* getCompilerResultByError(CompilerResults& results, int error)
{
    for(const CompilerResult& res : results)
    {
        if(res.code == error)
            return &res;
    }
    return nullptr;
}


Swallow::NodePtr parseStatement(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swallow;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setSourceFile(SourceFilePtr(new SourceFile(L"<code>", str)));
    NodePtr ret = parser.parseStatement(str);
    dumpCompilerResults(compilerResults);
    return ret;
}

Swallow::ProgramPtr parseStatements(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swallow;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setSourceFile(SourceFilePtr(new SourceFile(L"<code>", str)));
    ProgramPtr ret = parser.parse(str);
    dumpCompilerResults(compilerResults);
    return ret;
}
/*
static void dumpAST(const NodePtr& node, const wchar_t* title)
{
    if(node && opt_dumpAST)
    {
        std::wcout<<title<<std::endl;
        ASTHierachyDumper dumper(std::wcout);
        node->accept(&dumper);
    }
}
*/
void initTestMethods(const std::shared_ptr<Swallow::SwallowCompiler>& compiler)
{
    GlobalScope* global = compiler->getSymbolRegistry()->getGlobalScope();
    global->declareFunction(L"println", 0, L"Void", L"Int", NULL);
    global->declareFunction(L"println", 0, L"Void", L"String", NULL);
    global->declareFunction(L"print", 0, L"Void", L"String", NULL);
    global->declareFunction(L"assert", 0, L"Void", L"Bool", L"String", NULL);
}
/*
Swallow::ScopedProgramPtr analyzeStatement(Swallow::SymbolRegistry& registry, Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swallow;

    ScopedNodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setSourceFile(SourceFilePtr(new SourceFile(L"<code>", str)));
    ScopedProgramPtr ret = std::dynamic_pointer_cast<ScopedProgram>(parser.parse(str));
    ModulePtr module(new Module(L"test", registry.getGlobalScope()->getModuleType()));
    if(!ret)
        return ret;
    try
    {
        dumpAST(ret, L"Before transform AST:");
        OperatorResolver operatorResolver(&registry, &compilerResults);
        SemanticAnalyzer analyzer(&registry, &compilerResults, module);
        ret->accept(&operatorResolver);
        ret->accept(&analyzer);

        dumpAST(ret, L"Successed to transform AST< result:");
    }
    catch(const Abort&)
    {
        dumpAST(ret, L"Failed to transform AST< result:");
    }

    return ret;
}

*/

Tracer::Tracer(const char* file, int line, const char* func)
{
    strcpy(this->file, file);
    strcpy(this->func, func);
    this->line = line;
#ifdef TRACE_NODE
    using namespace Swallow;
    Node::UnreleasedNodes.clear();
    Node::NodeCount = 0;
#endif
}
Tracer::~Tracer()
{
#ifdef TRACE_NODE
    using namespace Swallow;
    using namespace std;
    list<Node*>& nodes = Node::UnreleasedNodes;
    int unreleasedNodes = nodes.size();
    if(Node::NodeCount != 0)
    {
        stringstream ss;
        list<Node*>::iterator iter = nodes.begin();
        ss<<unreleasedNodes<<" unreleased AST nodes detected in [" << file << ":" << func << "]:";
        for(; iter != nodes.end(); iter++)
        {
            Node* node = *iter;
            const char* nodeName = Node::nodeTypeToName(node->getNodeType());
            NodeSerializerA serializer(ss);
            ss << nodeName << " : " << static_cast<const void*>(node) << "(";
            node->accept(&serializer);
            ss << ")" << ", ";
        }
        std::cout<<ss.str()<<std::endl;
    }
#endif//TRACE_NODE
}
