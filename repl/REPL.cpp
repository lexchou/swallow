#include "REPL.h"
#include "ConsoleWriter.h"
#include <iostream>
#include <parser/Parser.h>
#include <common/Errors.h>
#include <semantics/SemanticAnalyzer.h>
#include <semantics/ScopedNodes.h>
#include <cassert>
#include <ast/ast.h>
#include <semantics/GlobalScope.h>

using namespace std;
using namespace Swallow;


REPL::REPL(const ConsoleWriterPtr& out, const char* path)
:out(out), canQuit(false), file(path)
{
    initCommands();
    resultId = 0;

}

void REPL::repl()
{
    wstring line;
    int id = 1;
    out->printf(L"Welcome to Swallow! Type :help for assistance.\n");
    program = nodeFactory.createProgram();
    while(!canQuit && file.good() ? !file.eof() : !wcin.eof())
    {
        out->printf(L"%3d> ", id);
        file.good() ? getline(file, line) : getline(wcin, line);
        if(line.empty())
            continue;
        if(line[0] == ':')
        {
            evalCommand(line.substr(1));
            continue;
        }
        CompilerResults compilerResults;
        eval(compilerResults, line);
        dumpCompilerResults(compilerResults, line);
        id++;
    }
}

void REPL::eval(CompilerResults& compilerResults, const wstring& line)
{
    Parser parser(&nodeFactory, &compilerResults);
    parser.setFileName(L"<file>");
    //remove parsed nodes in last eval
    program->clearStatements();

    bool successed = parser.parse(line.c_str(), program);
    if(!successed)
        return;
    try
    {
        SemanticAnalyzer analyzer(&registry, &compilerResults);
        program->accept(&analyzer);
        dumpProgram();
    }
    catch(const Abort&)
    {
//ignore this
    }

}
void REPL::dumpProgram()
{
    SymbolScope* scope = static_pointer_cast<ScopedProgram>(program)->getScope();
    assert(scope != nullptr);
    out->setForegroundColor(Cyan);
    for(const StatementPtr& st : *program)
    {
        SymbolPtr sym = nullptr;
        switch(st->getNodeType())
        {
            case NodeType::Identifier:
            {
                IdentifierPtr id = static_pointer_cast<Identifier>(st);
                sym = scope->lookup(id->getIdentifier());
                dumpSymbol(sym);
                break;
            }
            case NodeType::ValueBindings:
            {
                ValueBindingsPtr vars = static_pointer_cast<ValueBindings>(st);
                for(const ValueBindingPtr& var : *vars)
                {
                    if(IdentifierPtr id = dynamic_pointer_cast<Identifier>(var->getName()))
                    {
                        sym = scope->lookup(id->getIdentifier());
                        dumpSymbol(sym);
                    }
                }
                break;
            }
            default:
            {
                if(PatternPtr pat = dynamic_pointer_cast<Pattern>(st))
                {
                    if(pat->getType() && !Type::equals(registry.getGlobalScope()->t_Void, pat->getType()))
                    {
                        wstringstream s;
                        s<<L"$R"<<(resultId++);
                        SymbolPlaceHolderPtr sym(new SymbolPlaceHolder(s.str(), pat->getType(), SymbolPlaceHolder::R_LOCAL_VARIABLE, 0));
                        dumpSymbol(sym);
                    }
                }
                break;
            }
        }
    }
    out->reset();
}

void REPL::dumpSymbol(const SymbolPtr& sym)
{
    if(sym && sym->getType())
    {
        wstring type = sym->getType()->toString();
        out->printf(L"%ls : %ls\n", sym->getName().c_str(), type.c_str());
    }
}


void REPL::dumpCompilerResults(CompilerResults& compilerResults, const std::wstring& code)
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


void REPL::evalCommand(const wstring &command)
{
    wstring cmd = command;
    wstring args;
    wstring::size_type pos = command.find(L' ');
    if(pos != wstring::npos)
    {
        cmd = command.substr(0, pos);
        args = cmd.substr(pos + 1, command.size() - pos);
    }
    auto iter = methods.find(cmd);
    if(iter == methods.end())
    {
        out->printf(L"error: '%ls' is not a valid command.\n", cmd.c_str());
        return;
    }
    (this->*iter->second)(args);
}

void REPL::initCommands()
{
    methods.insert(make_pair(L"help", &REPL::commandHelp));
    methods.insert(make_pair(L"quit", &REPL::commandQuit));
    methods.insert(make_pair(L"exit", &REPL::commandQuit));
}
void REPL::commandHelp(const wstring& args)
{

    out->printf(L"The Swallow REPL (Read-Eval-Print-Loop) acts like an interpreter.  Valid statements, expressions, and declarations.\n");
    out->printf(L"Compiler and execution is not finished yet.");
    out->printf(L"\n");
    out->printf(L"The complete set of commands are also available as described below.  Commands must be prefixed with a colon at the REPL prompt (:quit for example.) \n\n\n");
    out->printf(L"REPL commands:\n");
    out->printf(L"  help              -- Show a list of all swallow commands, or give details about specific commands.\n");
    out->printf(L"  quit              -- Quit out of the Swallow REPL.\n\n");
}
void REPL::commandQuit(const wstring& args)
{
    canQuit = true;
}
