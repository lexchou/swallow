/* main.cpp --
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
#include <parser/Parser.h>
#include <semantics/SymbolRegistry.h>
#include <semantics/ScopedNodeFactory.h>
#include <common/CompilerResults.h>
#include <semantics/SemanticAnalyzer.h>
#include <semantics/ScopedNodes.h>
#include <iostream>
#include "ConsoleWriter.h"
#include "common/Errors.h"

using namespace std;
using namespace Swift;
typedef std::shared_ptr<ConsoleWriter> ConsoleWriterPtr;
class Repl
{
public:
    Repl(const ConsoleWriterPtr& out)
    :out(out)
    {
    }

    void eval(CompilerResults& compilerResults, const wstring& line)
    {
        Parser parser(&nodeFactory, &compilerResults);
        parser.setFileName(L"<file>");
        ScopedProgramPtr ret = std::dynamic_pointer_cast<ScopedProgram>(parser.parse(line.c_str()));
        if(!ret)
            return;
        try
        {
            SemanticAnalyzer analyzer(&registry, &compilerResults);
            ret->accept(&analyzer);
            SymbolScope* scope = ret->getScope();
            for(auto entry : scope->getSymbols())
            {
                registry.getCurrentScope()->addSymbol(entry.first, entry.second);
            }
            dumpScope(scope);
        }
        catch(const Abort&)
        {
//ignore this
        }

    }
    void dumpScope(SymbolScope* scope)
    {
        out->setForegroundColor(Cyan);
        for(auto entry : scope->getSymbols())
        {
            if(SymbolPlaceHolderPtr s = std::dynamic_pointer_cast<SymbolPlaceHolder>(entry.second))
            {
                wstring type = s->getType()->toString();
                out->printf(L"%ls : %ls\n", s->getName().c_str(), type.c_str());
            }
        }
        out->reset();
    }



    void dumpCompilerResults(CompilerResults& compilerResults, const std::wstring& code)
    {
        for(auto res : compilerResults)
        {
            out->setForegroundColor(White);
            out->printf(L"%d:%d: ", res.line, res.column);
            switch(res.level)
            {
                case ErrorLevel::Fatal:
                    out->setForegroundColor(Red);
                    out->printf(L"fatal");
                    break;
                case ErrorLevel::Error:
                    out->setForegroundColor(Red, Bright);
                    out->printf(L"error");
                    break;
                case ErrorLevel::Note:
                    out->setForegroundColor(White);
                    out->printf(L"note");
                    break;
                case ErrorLevel::Warning:
                    out->setForegroundColor(Yellow);
                    out->printf(L"warning");
                    break;
            }
            out->setForegroundColor(White, Bright);
            out->printf(L": ");
            wstring msg = Errors::format(res.code, res.items);
            out->printf(L"%ls\n", msg.c_str());
            out->reset();
            out->printf(L"%ls\n", code.c_str());
            for(int i = 1; i < res.column; i++)
            {
                out->printf(L" ");
            }
            out->setForegroundColor(Green);
            out->printf(L"^\n");
            out->reset();
        }
    }
private:
    SymbolRegistry registry;
    ScopedNodeFactory nodeFactory;
    ConsoleWriterPtr out;
};



int main(int argc, char** argv)
{
    ConsoleWriterPtr out(ConsoleWriter::create());
    int id = 1;
    bool canQuit = false;
    wstring line;
    Repl repl(out);
    out->printf(L"Welcome to Swallow! Type :help for assistance.\n");
    while(!canQuit && !wcin.eof())
    {
        out->printf(L"%3d> ", id++);
        getline(wcin, line);
        if(line.empty())
            continue;
        CompilerResults compilerResults;
        repl.eval(compilerResults, line);
        repl.dumpCompilerResults(compilerResults, line);
    }

    return 0;
}
