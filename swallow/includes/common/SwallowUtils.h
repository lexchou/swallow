/* SwallowUtils.h --
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
#ifndef SWALLOW_UTILS_H
#define SWALLOW_UTILS_H
#include "swallow_conf.h"
#include <string>
#include <ostream>
#include <vector>

SWALLOW_NS_BEGIN

class CompilerResults;

typedef std::shared_ptr<class Node> NodePtr;

struct SwallowUtils
{
    static void dumpHex(const char* s);
    static std::wstring readFile(const char* fileName);
    static std::vector<std::string> readDirectory(const char* path);
    static void dumpCompilerResults(const std::wstring& src, const CompilerResults& compilerResults, std::wostream& out);
    static std::wstring toString(const NodePtr& node);
    static std::wstring toString(int i);

    /*!
     * A simplified approach to convert std::string to std::wstring
     */
    static std::wstring toWString(const std::string& str);
    /*!
     * A simplified approach to convert std::wstring to std::string
     */
    static std::string toString(const std::wstring& str);
};
SWALLOW_NS_END

#endif//SWALLOW_UTILS_H
