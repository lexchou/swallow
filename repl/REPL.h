/* REPL.h --
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
#ifndef REPL_H
#define REPL_H
#include <string>
#include <map>
#include <memory>
#include "common/CompilerResults.h"
#include <semantics/SymbolRegistry.h>
#include <semantics/ScopedNodeFactory.h>
#include <ast/ast-decl.h>
using std::wstring;
class REPL;
typedef std::shared_ptr<class ConsoleWriter> ConsoleWriterPtr;
typedef void (REPL::*CommandMethod)(const wstring& args);

class REPL
{
public:
    REPL(const ConsoleWriterPtr& out);
public:
    void repl();
private:
    void evalCommand(const wstring& command);
    void eval(Swallow::CompilerResults& compilerResults, const wstring& line);
    void dumpCompilerResults(Swallow::CompilerResults& compilerResults, const std::wstring& code);
    void dumpProgram();
    void dumpSymbol(const Swallow::SymbolPtr& sym);

private://commands
    void initCommands();
    void commandHelp(const wstring& args);
    void commandQuit(const wstring& args);
private:
    Swallow::SymbolRegistry registry;
    Swallow::ScopedNodeFactory nodeFactory;
    Swallow::ProgramPtr program;
    std::map<std::wstring, CommandMethod> methods;
    ConsoleWriterPtr out;
    int resultId;
    bool canQuit;

};

#endif//REPL_H