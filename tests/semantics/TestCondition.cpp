/* TestCondition.cpp --
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

TEST(TestCondition, SwitchCase_DuplicatedCase)
{
    //TODO neither the official compiler implemented this checking
}

TEST(TestCondition, SwitchCase_NoStatement)
{
    SEMANTIC_ANALYZE(L"var a = 3;\n"
            L"switch a { \n"
            L"case 0:\n"
            L"\n"
            L"}\n");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, res.code);
    ASSERT_EQ(L"case", res.items[0]);
}

TEST(TestCondition, SwitchCase_DefaultNoStatement)
{
    SEMANTIC_ANALYZE(L"var a = 3;\n"
            L"switch a { \n"
            L"case 0:\n"
            L"break\n"
            L"default:"
            L"}\n");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_LABEL_IN_SWITCH_SHOULD_HAVE_AT_LEAST_ONE_STATEMENT_0, res.code);
    ASSERT_EQ(L"default", res.items[0]);
}
TEST(TestCondition, SwitchCase_EnumNotExhausive)
{
    SEMANTIC_ANALYZE(L"enum MyEnum{case A, B, C, D}\n"
            L"var a = MyEnum.A\n"
            L"switch a {\n"
            L"case .A:break\n"
            L"case .B:break\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE, res.code);
}

TEST(TestCondition, SwitchCase_BoolNotExhausive)
{
    SEMANTIC_ANALYZE(L"var a = true\n"
            L"switch a {\n"
            L"case true:break\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_SWITCH_MUST_BE_EXHAUSIVE_CONSIDER_ADDING_A_DEFAULT_CLAUSE, res.code);
}

TEST(TestCondition, SwitchCase_NotExhausiveWithDefaultCase)
{
    SEMANTIC_ANALYZE(L"enum MyEnum{case A, B, C, D}\n"
            L"var a = MyEnum.A\n"
            L"switch a {\n"
            L"case .A:break\n"
            L"default:break\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestCondition, SwitchCase_NonBoolGuard)
{
    SEMANTIC_ANALYZE(L"var a = true\n"
            L"switch a {\n"
            L"case true where 3:break\n"
            L"default:break\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);

    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);

}


TEST(TestCondition, If)
{
    SEMANTIC_ANALYZE(L"var a = 3\n"
            L"if a = 3\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);
}


TEST(TestCondition, If_Unpack_Optional)
{
    SEMANTIC_ANALYZE(L"if var a = 3\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_BOUND_VALUE_IN_A_CONDITIONAL_BINDING_MUST_BE_OF_OPTIONAL_TYPE, res.code);
}




TEST(TestCondition, If_Unpack_Optional2)
{
    SEMANTIC_ANALYZE(L"var b : Int? = 3\n"
            L"if var a = b\n"
            L"{\n"
            L"    \n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    ASSERT_EQ(2, root->numStatements());
    IfStatementPtr _if = dynamic_pointer_cast<IfStatement>(root->getStatement(1));
    ASSERT_NOT_NULL(_if);
    ScopedCodeBlockPtr then = dynamic_pointer_cast<ScopedCodeBlock>(_if->getThen());
    ASSERT_NOT_NULL(then);
    SymbolScope* thenScope = then->getScope();
    ASSERT_NOT_NULL(thenScope);
    SymbolPtr a = thenScope->lookup(L"a");
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(symbolRegistry.getGlobalScope()->Int, a->getType());
}


TEST(TestCondition, If_Unpack_Optional3)
{
    SEMANTIC_ANALYZE(L"if var a\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_VARIABLE_BINDING_IN_A_CONDITION_REQUIRES_AN_INITIALIZER, res.code);
}


TEST(TestCondition, If_Unpack_Optional4)
{
    SEMANTIC_ANALYZE(L"var b : Int? = 3\n"
            L"if var a : Float = b\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_IS_NOT_IDENTICIAL_TO_B_2, res.code);
}