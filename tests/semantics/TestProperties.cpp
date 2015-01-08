/* TestProperties.cpp --
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

TEST(TestProperties, StoredProperties)
{
    SEMANTIC_ANALYZE(L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"var rangeOfThreeItems = FixedLengthRange(firstValue: 0, length: 3)\n"
            L"rangeOfThreeItems.firstValue = 6");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestProperties, StoredProperties_ConstantStructure)
{
    SEMANTIC_ANALYZE(L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"let rangeOfFourItems = FixedLengthRange(firstValue: 0, length: 4)\n"
            L"rangeOfFourItems.firstValue = 6");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2, res.code);
}
TEST(TestProperties, StoredProperties_ConstantClass)
{
    SEMANTIC_ANALYZE(L"\n"
            L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"class Container\n"
            L"{\n"
            L"    let f = FixedLengthRange(firstValue: 0, length: 4)\n"
            L"}\n"
            L"var f = Container();\n"
            L"f.f.firstValue = 6");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION, res.code);
}


TEST(TestProperties, StoredProperties_ConstantClass2)
{
    SEMANTIC_ANALYZE(L"\n"
            L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"class Container\n"
            L"{\n"
            L"    var f = FixedLengthRange(firstValue: 0, length: 4)\n"
            L"}\n"
            L"let f = Container();\n"
            L"f.f.firstValue = 6");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}