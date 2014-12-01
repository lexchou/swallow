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
#include "ast/NodeSerializer.h"
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
//#include <codecvt>
using namespace std;
wstring readFile(const char* fileName)
{
    wifstream wif;
    wif.open(fileName, istream::in);
    if(!wif.is_open())
    {
        cerr << "Failed to open swift source file for testing, file name " <<fileName<<endl;
        abort();
    }
    //wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    wstringstream wss;
    assert(!wif.eof() && "Failed to open file.");
    wss << wif.rdbuf();
    wstring ret = wss.str();
    return ret;
}

void dumpCompilerResults(Swallow::CompilerResults& compilerResults)
{
    using namespace Swallow;
    if(compilerResults.numResults() > 0)
    {
        for(int i = 0; i < compilerResults.numResults(); i++)
        {
            const CompilerResult& res = compilerResults.getResult(i);
            switch(res.level)
            {
                case ErrorLevel::Fatal:
                    std::wcout<<L"Fatal:";
                    break;
                case ErrorLevel::Error:
                    std::wcout<<L"Error:";
                    break;
                case ErrorLevel::Warning:
                    std::wcout<<L"Warning:";
                    break;
                case ErrorLevel::Note:
                    std::wcout<<L"Note:";
                    break;
            }
            std::wcout<<L"("<<res.line<<L", "<<res.column<<") ";
            std::wstring msg = Errors::format(res.code, res.items);
            std::wcout<<msg<<std::endl;
        }

    }
}


Swallow::NodePtr parseStatement(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swallow;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    NodePtr ret = parser.parseStatement(str);
    dumpCompilerResults(compilerResults);
    return ret;
}

Swallow::ProgramPtr parseStatements(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swallow;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ProgramPtr ret = parser.parse(str);
    dumpCompilerResults(compilerResults);
    return ret;
}
Swallow::ScopedProgramPtr analyzeStatement(Swallow::SymbolRegistry& registry, Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swallow;
    registry.getGlobalScope()->declareFunction(L"println", 0, L"Void", L"Int", NULL);
    registry.getGlobalScope()->declareFunction(L"println", 0, L"Void", L"String", NULL);

    ScopedNodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ScopedProgramPtr ret = std::dynamic_pointer_cast<ScopedProgram>(parser.parse(str));
//ScopedProgram* f;
    //cdumpCompilerResults(compilerResults);
//d.g();
    if(!ret)
        return ret;
    try
    {
        SemanticAnalyzer analyzer(&registry, &compilerResults);
        ret->accept(&analyzer);
    }
    catch(const Abort&)
    {
//ignore this
    }

    return ret;
}


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
