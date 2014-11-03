#include <parser/Parser.h>
#include <semantics/SymbolRegistry.h>
#include <semantics/ScopedNodeFactory.h>
#include <common/CompilerResults.h>
#include <semantics/SemanticAnalyzer.h>
#include <semantics/ScopedNodes.h>
#include <iostream>
#include "ConsoleWriter.h"

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
            wstring msg = compilerResults.format(res);
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