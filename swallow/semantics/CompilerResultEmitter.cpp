/* CompilerResultEmitter.cpp --
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
#include "CompilerResultEmitter.h"
#include <cassert>
#include "ast/Node.h"
#include "common/Errors.h"
#include "common/CompilerResults.h"

USE_SWALLOW_NS
using namespace std;


CompilerResultEmitter::CompilerResultEmitter(CompilerResults* compilerResults)
        :compilerResults(compilerResults)
{
}



/*!
 * Abort the visitor
 */
void CompilerResultEmitter::abort()
{
    throw Abort();
}
void CompilerResultEmitter::error(const NodePtr& node, int code, const std::vector<std::wstring>& items)
{
    assert(node != nullptr);
    compilerResults->add(ErrorLevel::Error, *node->getSourceInfo(), code, items);
    abort();
}
/*!
 * Outputs an compiler error
 */
void CompilerResultEmitter::error(const NodePtr& node, int code, const std::wstring& item)
{
    ResultItems items = {item};
    error(node, code, items);
}
void CompilerResultEmitter::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2)
{
    ResultItems items = {item1, item2};
    error(node, code, items);
}
void CompilerResultEmitter::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3)
{
    ResultItems items = {item1, item2, item3};
    error(node, code, items);
}
void CompilerResultEmitter::error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3, const std::wstring& item4)
{
    ResultItems items = {item1, item2, item3, item4};
    error(node, code, items);
}

void CompilerResultEmitter::error(const NodePtr& node, int code)
{
    error(node, code, L"");
}
void CompilerResultEmitter::warning(const NodePtr& node, int code, const std::wstring& item)
{
    compilerResults->add(ErrorLevel::Warning, *node->getSourceInfo(), code, item);
}

