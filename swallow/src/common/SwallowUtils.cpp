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
#include "ast/utils/NodeSerializer.h"
#include "common/Errors.h"
#include "ast/Node.h"
#include <cstdlib>
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <dirent.h>
#endif
//#include <codecvt>

USE_SWALLOW_NS
using namespace std;



void SwallowUtils::dumpHex(const char* s)
{
    char hex[100];
    char raw[20];
    int addr = 0;
    while(s[addr])
    {
        int p = sprintf(hex, "0x%08x ", addr);
        int i = 0;
        for(; i < 16 && s[addr]; i++, addr++)
        {
            p += sprintf(hex + p, "%02x ", s[addr]);
            raw[i] = s[addr];
        }
        raw[i] = 0;
        for(; i < 16; i++)
        {
            p += sprintf(hex + p, "   ");
        }
        printf("%s %s\n", hex, raw);
    }
}
std::vector<std::string> SwallowUtils::readDirectory(const char* path)
{
	vector<string> ret;
#if defined(_WIN32) || defined(_WIN64)
	assert(0 && "Not supported");
#else
    DIR *dir;
    dir = opendir(path);
    if (dir)
    {
        struct dirent* ent;
        while((ent = readdir(dir)) != NULL)
        {
            ret.push_back(ent->d_name);
        }
        closedir(dir);
    }
#endif
	return ret;
}


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
void SwallowUtils::dumpCompilerResults(const CompilerResults& compilerResults, std::wostream& out)
{

    using namespace Swallow;
    if(!compilerResults.numResults())
        return;


    for(int i = 0; i < compilerResults.numResults(); i++)
    {
        CompilerResult res = compilerResults.getResult(i);

        //separate source code by lines
        vector<wstring> lines;
        wstringstream stream(res.sourceFile->code);
        wstring line;
        while(getline(stream, line))
        {
            lines.push_back(line);
        }

        switch (res.level)
        {
            case ErrorLevel::Fatal:
                out << L"Fatal";
                break;
            case ErrorLevel::Error:
                out << L"Error";
                break;
            case ErrorLevel::Warning:
                out << L"Warning";
                break;
            case ErrorLevel::Note:
                out << L"Note";
                break;
        }
        out << L" " << res.code << L" ";
        if(res.sourceFile)
            out<< res.sourceFile->fileName;
        out<<L":";
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
std::wstring SwallowUtils::toString(const NodePtr& node)
{
    std::wstringstream out;
    NodeSerializerW serializer(out);
    node->accept(&serializer);
    return out.str();
}
std::wstring SwallowUtils::toString(int i)
{
    std::wstringstream s;
    s<<i;
    return s.str();
}
/*!
 * A simplified approach to convert std::string to std::wstring
 */
std::wstring SwallowUtils::toWString(const std::string& str)
{
    wstring ret;
    ret.append(str.begin(), str.end());
    return ret;

}
/*!
 * A simplified approach to convert std::wstring to std::string
 */
std::string SwallowUtils::toString(const std::wstring& str)
{
    string ret;
    ret.append(str.begin(), str.end());
    return ret;
}
