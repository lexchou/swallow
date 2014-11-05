/* utils.h --
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
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <gtest/gtest.h>
#include <iostream>
#include <cstdlib>
#include "parser/Parser.h"
#include "ast/ast.h"
#include "ast/NodeFactory.h"
#include "common/CompilerResults.h"
#include "semantics/semantic-types.h"


#define ASSERT_EQUALS(E, A) wcs_assertEquals((E), (A), __FILE__, __LINE__)
#define ASSERT_NOT_NULL(condition) GTEST_TEST_BOOLEAN_((condition) != NULL, #condition, false, true, GTEST_FATAL_FAILURE_)
#define ASSERT_NULL(condition) GTEST_TEST_BOOLEAN_((condition) == NULL, #condition, false, true, GTEST_FATAL_FAILURE_)

void dumpCompilerResults(Swallow::CompilerResults& compilerResults);
Swallow::NodePtr parseStatement(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str);
Swallow::ProgramPtr parseStatements(Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str);

Swallow::ScopedProgramPtr analyzeStatement(Swallow::SymbolRegistry& registry, Swallow::CompilerResults& compilerResults, const char* func, const wchar_t* str);
std::wstring readFile(const char* fileName);





struct Tracer
{
    Tracer(const char* file, int line, const char* func);
    ~Tracer();
    char file[1024];
    int line;
    char func[100];
    
};
#define PARSE_STATEMENT(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swallow::CompilerResults compilerResults; \
    NodePtr root = parseStatement(compilerResults, __FUNCTION__, (s));




#define SEMANTIC_ANALYZE(s) Tracer tracer(__FILE__, __LINE__, __FUNCTION__); \
    Swallow::SymbolRegistry symbolRegistry; \
    Swallow::CompilerResults compilerResults; \
    ScopedProgramPtr root = analyzeStatement(symbolRegistry, compilerResults, __FUNCTION__, (s)); \
    Swallow::SymbolScope* scope = root ? root->getScope() : nullptr; \
    (void)scope;
#define SEMANTIC_ANALYZE_F(fileName) std::wstring content = readFile(fileName); \
    SEMANTIC_ANALYZE(content.c_str());

#endif//TEST_UTILS_H



