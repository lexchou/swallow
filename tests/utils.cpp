/* utils.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
//#include <codecvt>

std::wstring readFile(const char* fileName)
{
    std::wifstream wif(fileName);
    //wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    return wss.str();
}

void dumpCompilerResults(Swift::CompilerResults& compilerResults)
{
    using namespace Swift;
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


Swift::NodePtr parseStatement(Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    NodePtr ret = parser.parseStatement(str);
    dumpCompilerResults(compilerResults);
    return ret;
}

Swift::ProgramPtr parseStatements(Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;
    NodeFactory nodeFactory;
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    ProgramPtr ret = parser.parse(str);
    dumpCompilerResults(compilerResults);
    return ret;
}
Swift::ScopedProgramPtr analyzeStatement(Swift::SymbolRegistry& registry, Swift::CompilerResults& compilerResults, const char* func, const wchar_t* str)
{
    using namespace Swift;

    FunctionOverloadedSymbolPtr println(new FunctionOverloadedSymbol());
    {
        TypePtr t_int = registry.lookupType(L"Int");
        TypePtr t_Void = registry.lookupType(L"Void");
        std::vector<Type::Parameter> parameters = {Type::Parameter(t_int)};
        TypePtr type = Type::newFunction(parameters, t_Void, false);
        FunctionSymbolPtr println_int(new FunctionSymbol(L"println", type, nullptr));
        println->add(println_int);
    }
    registry.getCurrentScope()->addSymbol(L"println", SymbolPtr(println));
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
    using namespace Swift;
    Node::UnreleasedNodes.clear();
    Node::NodeCount = 0;
#endif
}
Tracer::~Tracer()
{
#ifdef TRACE_NODE
    using namespace Swift;
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
