/* TestLoop.cpp --
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
#include "../utils.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GlobalScope.h"
#include "semantics/GenericArgument.h"

using namespace Swallow;
using namespace std;

/*
TODO: Fix the ForIn test case after migrated standard library to external file and implemented Range/SequenceType/...


TEST(TestLoop, ForIn)
{
    SEMANTIC_ANALYZE(L"for index in 1...5 {\n"
            L"    println(\"\\(index) times 5 is \\(index * 5)\")\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
*/


/*
TEST(TestLoop, ForIn_NotSequenceType)
{

    SEMANTIC_ANALYZE(L"for index in 3 {\n"
            L"    println(\"\\(index) times 5 is \\(index * 5)\")\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);

}
*/

TEST(TestLoop, For)
{
    SEMANTIC_ANALYZE(L"for var index = 0; index < 3; index++ {\n"
            L"    println(\"index is \\(index)\")\n"
            L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestLoop, For_InnerScope)
{
    SEMANTIC_ANALYZE(L"var index = 0\n"
            L"for var index = 0; index < 3; index++ {\n"
            L"    println(\"index is \\(index)\")\n"
            L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestLoop, For_NonBooleanCondition)
{
    SEMANTIC_ANALYZE(L"for var index = 0; index; index++ {\n"
            L"    println(\"index is \\(index)\")\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);
}


TEST(TestLoop, For_Undeclared)
{
    SEMANTIC_ANALYZE(L"for index = 0; index < 3; ++index {\n"
            L"    println(\"index is \\(index)\")\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, res.code);
    ASSERT_EQ(L"index", res.items[0]);
}


TEST(TestLoop, While)
{
    SEMANTIC_ANALYZE(L"var i = 0, sum = 0\n"
            L"while i < 10 {\n"
            L"  sum = sum + i\n"
            L"  i++\n"
            L"}");
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestLoop, While_NonBooleanCondition)
{
    SEMANTIC_ANALYZE(L"var i = 0, sum = 0\n"
            L"while i {\n"
            L"  sum = sum + i\n"
            L"  i++\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);
}



TEST(TestLoop, Do)
{
    SEMANTIC_ANALYZE(L"var i = 0, sum = 0\n"
            L"do {\n"
            L"  sum = sum + i\n"
            L"  i++\n"
            L"} while i < 20");
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestLoop, Do_NonBooleanCondition)
{
    SEMANTIC_ANALYZE(L"var i = 0, sum = 0\n"
            L"do {\n"
            L"  sum = sum + i\n"
            L"  i++\n"
            L"} while i");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);
}


