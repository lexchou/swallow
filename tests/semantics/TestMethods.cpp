/* TestMethods.cpp --
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

TEST(TestMethods, InstanceMethods)
{
    SEMANTIC_ANALYZE(L"class Counter {\n"
            L"    var count = 0\n"
            L"    func increment() {\n"
            L"        count++\n"
            L"    }\n"
            L"    func incrementBy(amount: Int) {\n"
            L"        count = count + amount\n"
            L"    }\n"
            L"    func reset() {\n"
            L"        count = 0\n"
            L"    }\n"
            L"}\n"
            L"let counter = Counter()\n"
            L"// the initial counter value is 0\n"
            L"counter.increment()\n"
            L"// the counter's value is now 1\n"
            L"counter.incrementBy(5)\n"
            L"// the counter's value is now 6\n"
            L"counter.reset()\n"
            L"// the counter's value is now 0");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestMethods, ChangeFieldWithoutMutating)
{
        SEMANTIC_ANALYZE(L"struct XX\n"
                L"{\n"
                L"    var A = 3\n"
                L"    func foo()\n"
                L"    {\n"
                L"        self.A = 2;\n"
                L"    }\n"
                L"}");
        ASSERT_EQ(1, compilerResults.numResults());
        auto res = compilerResults.getResult(0);
        ASSERT_EQ(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2, res.code);
}

TEST(TestMethods, ChangeFieldWithMutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
                L"{\n"
                L"    var A = 3\n"
                L"    mutating func foo()\n"
                L"    {\n"
                L"        self.A = 2;\n"
                L"    }\n"
                L"}");
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestMethods, ChangeFieldWithoutMutating_Enum)
{
    SEMANTIC_ANALYZE(L"enum XX\n"
        L"{\n"
        L"    case AA"
        L"    func foo()\n"
        L"    {\n"
        L"        self = XX.AA;\n"
        L"    }\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_ASSIGN_TO_A_IN_A_METHOD_1, res.code);
}

TEST(TestMethods, ChangeFieldWithMutating_Enum)
{
    SEMANTIC_ANALYZE(L"enum XX\n"
        L"{\n"
        L"    case AA"
        L"    mutating func foo()\n"
        L"    {\n"
        L"        self = XX.AA;\n"
        L"    }\n"
        L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestMethods, BothMutatingAndNonMutating)
{
    SEMANTIC_ANALYZE(L"struct XX\n"
        L"{\n"
        L"    mutating nonmutating func foo()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_METHOD_MAY_NOT_BE_DECLARED_BOTH_MUTATING_AND_NONMUTATING, res.code);


}

TEST(TestMethods, StaticNonmutating)
{
        SEMANTIC_ANALYZE(L"struct XX\n"
            L"{\n"
            L"    static nonmutating func foo()\n"
            L"    {\n"
            L"    }\n"
            L"}");
        ASSERT_EQ(1, compilerResults.numResults());
        auto res = compilerResults.getResult(0);
        ASSERT_EQ(Errors::E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1, res.code);
        ASSERT_EQ(L"nonmutating", res.items[0]);
}

TEST(TestMethods, StaticMutating)
{
        SEMANTIC_ANALYZE(L"struct XX\n"
            L"{\n"
            L"    static mutating func foo()\n"
            L"    {\n"
            L"    }\n"
            L"}");
        ASSERT_EQ(1, compilerResults.numResults());
        auto res = compilerResults.getResult(0);
        ASSERT_EQ(Errors::E_STATIC_FUNCTIONS_MAY_NOT_BE_DECLARED_A_1, res.code);
        ASSERT_EQ(L"mutating", res.items[0]);
}


TEST(TestMethods, MutatingVar)
{
        SEMANTIC_ANALYZE(L"mutating var a = 3");
        ASSERT_EQ(1, compilerResults.numResults());
        auto res = compilerResults.getResult(0);
        ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, res.code);
        ASSERT_EQ(L"mutating", res.items[0]);
        ASSERT_EQ(L"func", res.items[1]);
}

TEST(TestMethods, MutatingClass)
{
    SEMANTIC_ANALYZE(L"mutating class A{}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, res.code);
    ASSERT_EQ(L"mutating", res.items[0]);
    ASSERT_EQ(L"func", res.items[1]);
}

TEST(TestMethods, MutatingStruct)
{
    SEMANTIC_ANALYZE(L"mutating struct A{}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, res.code);
    ASSERT_EQ(L"mutating", res.items[0]);
    ASSERT_EQ(L"func", res.items[1]);
}


TEST(TestMethods, MutatingProtocol)
{
    SEMANTIC_ANALYZE(L"mutating protocol A{}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, res.code);
    ASSERT_EQ(L"mutating", res.items[0]);
    ASSERT_EQ(L"func", res.items[1]);
}

TEST(TestMethods, MutatingEnum)
{
    SEMANTIC_ANALYZE(L"mutating enum A{}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_USED_ON_B_DECLARATION_2, res.code);
    ASSERT_EQ(L"mutating", res.items[0]);
    ASSERT_EQ(L"func", res.items[1]);
}


TEST(TestMethods, MutatingFunc)
{
    SEMANTIC_ANALYZE(L"mutating func foo() {}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_IS_ONLY_VALID_ON_METHODS_1, res.code);
    ASSERT_EQ(L"mutating", res.items[0]);
}


TEST(TestMethods, NonMutatingFunc)
{
    SEMANTIC_ANALYZE(L"nonmutating func foo() {}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_IS_ONLY_VALID_ON_METHODS_1, res.code);
    ASSERT_EQ(L"nonmutating", res.items[0]);
}
TEST(TestMethods, MutatingClassFunc)
{
    SEMANTIC_ANALYZE(L"class Class\n"
        L"{\n"
        L"    mutating func foo()\n"
        L"    {\n"
        L"        \n"
        L"    }\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_ISNT_VALID_ON_METHODS_IN_CLASSES_OR_CLASS_BOUND_PROTOCOLS, res.code);
    ASSERT_EQ(L"mutating", res.items[0]);
}

TEST(TestMethods, ModifyValuesFromInstanceMethods)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
        L"    var x = 0.0, y = 0.0\n"
        L"    mutating func moveByX(deltaX: Double, y deltaY: Double) {\n"
        L"        x = x + deltaX\n"
        L"        y = y + deltaY\n"
        L"    }\n"
        L"}\n"
        L"var somePoint = Point(x: 1.0, y: 1.0)\n"
        L"somePoint.moveByX(2.0, y: 3.0)\n"
        L"println(\"The point is now at (\\(somePoint.x), \\(somePoint.y))\")");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());

}


TEST(TestMethods, ModifyValuesFromInstanceMethods_Let)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
        L"    var x = 0.0, y = 0.0\n"
        L"    mutating func moveByX(deltaX: Double, y deltaY: Double) {\n"
        L"        x = x + deltaX\n"
        L"        y = y + deltaY\n"
        L"    }\n"
        L"}\n"
        L"let fixedPoint = Point(x: 3.0, y: 3.0)\n"
        L"fixedPoint.moveByX(2.0, y: 3.0)\n");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2, res.code);
}
TEST(TestMethods, ModifyValuesFromInstanceMethods_NonmutatingSelf)
{
    SEMANTIC_ANALYZE(
        L"struct TTT\n"
        L"{\n"
        L"    var x = 3\n"
        L"    mutating func foo()\n"
        L"    {\n"
        L"    }\n"
        L"    func xxx()\n"
        L"    {\n"
        L"        foo()\n"
        L"        \n"
        L"    }\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_IMMUTABLE_VALUE_OF_TYPE_A_ONLY_HAS_MUTATING_MEMBERS_NAMED_B_2, res.code);
}



