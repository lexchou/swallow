/* RequestHandler.cpp --
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
#include "RequestHandler.h"
#include "common/SwallowUtils.h"
#include <fcgio.h>
#include <iostream>
#include <sstream>
#include <common/Errors.h>
#include "parser/Parser.h"
#include "semantics/SemanticAnalyzer.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/ScopedNodeFactory.h"
#include "common/CompilerResults.h"
#include "semantics/OperatorResolver.h"
#include "semantics/ScopedNodes.h"
#include "JSONSerializer.h"

using namespace std;
using namespace Swallow;

RequestHandler::RequestHandler()
{
    this->handlers.insert(make_pair("/swift/compiler/ast", &RequestHandler::handleAST));
}
void RequestHandler::handle(FCGX_Request* request)
{
    string scriptName = FCGX_GetParam("SCRIPT_NAME", request->envp);
    auto iter = handlers.find(scriptName);
    if(iter == handlers.end())
    {
        handle404();
        return;
    }

    Handler handler = iter->second;
    (this->*handler)(request);
}
void RequestHandler::handle404()
{
    cout<<"Content-Type: text/plain\r\n"
            <<"\r\n"
            <<"Bad request, not found";
}
static string readAll(istream& in)
{
    string ret;
    char buf[1024];
    while(!in.eof())
    {
        cin.read(buf, sizeof(buf));
        long n = cin.gcount();
        if(n == 0)
            break;
        ret.append(buf, n);
    }
    return ret;
}

static ScopedProgramPtr compile(const wstring& code, CompilerResults* compilerResults)
{
    ScopedNodeFactory nodeFactory;
    SymbolRegistry registry;
    Parser parser(&nodeFactory, compilerResults);
    parser.setFileName(L"<file>");
    ScopedProgramPtr ret = std::dynamic_pointer_cast<ScopedProgram>(parser.parse(code.c_str()));
    if(!ret)
        return nullptr;
    try
    {
        OperatorResolver operatorResolver(&registry, compilerResults);
        SemanticAnalyzer analyzer(&registry, compilerResults);
        ret->accept(&operatorResolver);
        ret->accept(&analyzer);
        return ret;
    }
    catch(const Abort&)
    {
        return nullptr;
    }

}


void RequestHandler::handleAST(FCGX_Request* request)
{
    cout<<"Content-Type: text/json\r\n"
            <<"\r\n";
    wstring code = SwallowUtils::toWString(readAll(cin));

    CompilerResults results;
    ScopedProgramPtr program = compile(code, &results);
    cout<<"{\"errors\" : [";
    for(const CompilerResult& res : results)
    {
        std::wstring msg = Errors::format(res.code, res.items);
        cout<<"{\"code\" : " << res.code << ", ";
        cout<<"\"line\" : " << res.line << ", ";
        cout<<"\"column\" : " << res.column << ", ";
        cout<<"\"level\" : " << res.level << ", ";
        cout<<"\"msg\" : \"" << SwallowUtils::toString(msg) << "\"}";
    }
    cout<<"]";
    if(program != nullptr)
    {
        wstringstream out;
        JSONSerializer serializer(out);
        program->accept(&serializer);
        string ast = SwallowUtils::toString(out.str());
        cout << ", \"ast\" : ";
        cout << ast;
    }
    cout<<"}";
}

