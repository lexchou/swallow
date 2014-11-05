/* CompilerResults.h --
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
#ifndef ERROR_LIST_H
#define ERROR_LIST_H
#include "swallow_conf.h"
#include "swallow_types.h"
#include <string>
#include <vector>

SWALLOW_NS_BEGIN

struct ErrorLevel
{
    enum T
    {
        Fatal,
        Error,
        Warning,
        Note
    };
};
typedef std::vector<std::wstring> ResultItems;
struct CompilerResult : SourceInfo
{
    ErrorLevel::T level;
    int code;
    ResultItems items;

    
    CompilerResult(ErrorLevel::T level, const SourceInfo& sourceInfo, int code, const ResultItems& items)
    :level(level), code(code), items(items)
    {
        this->fileHash = sourceInfo.fileHash;
        this->line = sourceInfo.line;
        this->column = sourceInfo.column;
    }
};

class SWALLOW_EXPORT CompilerResults
{
public:
    void clear();
    int numResults();
    const CompilerResult& getResult(int i);
    void add(ErrorLevel::T level, const SourceInfo&, int code, const ResultItems& items);
    void add(ErrorLevel::T level, const SourceInfo&, int code, const std::wstring& item = std::wstring());

    std::vector<CompilerResult>::iterator begin() { return results.begin();}
    std::vector<CompilerResult>::iterator end() { return results.end();}
private:
    std::vector<CompilerResult> results;
};

SWALLOW_NS_END

#endif//ERROR_LIST_H
