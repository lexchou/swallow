/* TestBasic.cpp --
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
#include "SwallowCompiler.h"

using namespace Swallow;
using namespace std;

TEST(TestBasic, UseModule)
{
    //SEMANTIC_ANALYZE(L"var a = Builtin");
    SwallowCompiler compiler(L"test");
    compiler.addSource(L"test", L"var a = Builtin");
    compiler.compile();
    const CompilerResult* error = nullptr;
    Swallow::CompilerResults& compilerResults = *compiler.getCompilerResults();
    ASSERT_ERROR(Errors::E_EXPECT_MODULE_MEMBER_NAME_AFTER_MODULE_NAME);
}
TEST(TestBasic, VariableUseBeforeInitialized)
{
    SEMANTIC_ANALYZE(L"var a : String\n"
            L"println(a)");
    ASSERT_ERROR(Errors::E_VARIABLE_A_USED_BEFORE_BEING_INITIALIZED_1);
}
TEST(TestBasic, VariableUseBeforeInitialized2)
{
    SEMANTIC_ANALYZE(L"var a : String\n"
            L"a = \"test\"\n"
            L"println(a)");
    ASSERT_NO_ERRORS();
}
TEST(TestBasic, VariableUseBeforeInitialized3)
{
    SEMANTIC_ANALYZE(L"var a : Int\n"
            L"if(true)\n"
            L"{\n"
            L"    a = 3\n"
            L"}\n"
            L"println(a)");
    ASSERT_ERROR(Errors::E_VARIABLE_A_USED_BEFORE_BEING_INITIALIZED_1);
}
TEST(TestBasic, VariableUseBeforeInitialized4)
{
    SEMANTIC_ANALYZE(L"var a : Int\n"
            L"if(true)\n"
            L"{\n"
            L"    a = 3\n"
            L"}\n"
            L"else\n"
            L"{\n"
            L"    a = 3\n"
            L"}\n"
            L"println(a)");
    ASSERT_NO_ERRORS();
}
TEST(TestBasic, VariableUseBeforeInitialized5)
{
    SEMANTIC_ANALYZE(L"var a : Int\n"
            L"if(true)\n"
            L"{\n"
            L"    if(true)\n"
            L"    {\n"
            L"        a = 3\n"
            L"    }\n"
            L"}\n"
            L"else\n"
            L"{\n"
            L"    a = 5;\n"
            L"}\n"
            L"println(a)");
    ASSERT_ERROR(Errors::E_VARIABLE_A_USED_BEFORE_BEING_INITIALIZED_1);
}
TEST(TestBasic, VariableUseBeforeInitialized6)
{
    SEMANTIC_ANALYZE(L"var cond1 = true;\n"
            L"var cond2 = false;\n"
            L"var cond3 = 4;\n"
            L"\n"
            L"var a : Int\n"
            L"if(cond1)\n"
            L"{\n"
            L"    if(cond2)\n"
            L"    {\n"
            L"        a = 3\n"
            L"    }\n"
            L"    else\n"
            L"    {\n"
            L"        switch(cond3)\n"
            L"        {\n"
            L"        case 1:\n"
            L"            a=1\n"
            L"        case 2:\n"
            L"            a=2\n"
            L"        case 3:\n"
            L"            a=3\n"
            L"        case 4:\n"
            L"            a=4\n"
            L"        default:\n"
            L"            break;\n"
            L"        }\n"
            L"    }\n"
            L"}\n"
            L"else\n"
            L"{\n"
            L"    a = 5;\n"
            L"}\n"
            L"println(a)");
    ASSERT_ERROR(Errors::E_VARIABLE_A_USED_BEFORE_BEING_INITIALIZED_1);
}
