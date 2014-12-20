/* CompilerResultEmitter.h --
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
#ifndef COMPILER_RESULT_EMITTER_H
#define COMPILER_RESULT_EMITTER_H
#include "swallow_conf.h"
#include <vector>
#include <string>

SWALLOW_NS_BEGIN

class CompilerResults;
typedef std::shared_ptr<class Node> NodePtr;
class SWALLOW_EXPORT CompilerResultEmitter
{
public:
    CompilerResultEmitter(CompilerResults* compilerResults);
public:
    /*!
    * Abort the visitor
    */
    void abort();

    /*!
     * Outputs an compiler error
     */
    void error(const NodePtr& node, int code);
    void error(const NodePtr& node, int code, const std::vector<std::wstring>& items);
    void error(const NodePtr& node, int code, const std::wstring& item);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3);
    void error(const NodePtr& node, int code, const std::wstring& item1, const std::wstring& item2, const std::wstring& item3, const std::wstring& item4);


    /*!
     * Outputs an compiler error
     */
    void warning(const NodePtr& node, int code, const std::wstring& item = std::wstring());

protected:
    CompilerResults* compilerResults;
};

SWALLOW_NS_END




#endif//OPERATOR_RESOLVER_H