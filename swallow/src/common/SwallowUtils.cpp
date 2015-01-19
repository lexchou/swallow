/* SwallowUtils.cpp --
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
#include "common/SwallowUtils.h"
#include <sstream>
#include <fstream>
#include <cassert>
#include <iostream>
#include "common/CompilerResults.h"
#include "common/Errors.h"
//#include <codecvt>

USE_SWALLOW_NS
using namespace std;


std::wstring SwallowUtils::readFile(const char* fileName)
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
void SwallowUtils::dumpCompilerResults(const std::wstring& src, const CompilerResults& compilerResults, std::wostream& out)
{

    using namespace Swallow;
    if(!compilerResults.numResults())
        return;

    //separate source code by lines
    vector<wstring> lines;
    wstringstream stream(src);
    wstring line;
    while(getline(stream, line))
    {
        lines.push_back(line);
    }

    for(int i = 0; i < compilerResults.numResults(); i++)
    {
        const CompilerResult &res = compilerResults.getResult(i);


        switch (res.level)
        {
            case ErrorLevel::Fatal:
                out << L"Fatal:";
                break;
            case ErrorLevel::Error:
                out << L"Error:";
                break;
            case ErrorLevel::Warning:
                out << L"Warning:";
                break;
            case ErrorLevel::Note:
                out << L"Note:";
                break;
        }
        out << L"(" << res.line << L", " << res.column << ") ";
        std::wstring msg = Errors::format(res.code, res.items);
        out << msg << std::endl;


        if(res.line > 0 && res.line <= (int)lines.size())
        {
            wstring line = lines[res.line - 1];
            out<<line<<endl;
            for(int i = 0; i < res.column - 1; i++)
            {
                out<<L' ';
            }
            out<<L'^'<<endl;
        }
    }


}
