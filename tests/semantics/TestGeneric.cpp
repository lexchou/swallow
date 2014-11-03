/* TestGeneric.cpp --
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericArgument.h"
#include "common/Errors.h"


using namespace Swift;

TEST(TestGeneric, Func_GenericParameter)
{
    SEMANTIC_ANALYZE(L"func swapTwoValues<T>(inout a: T, inout b: T) {\n"
            "    let temporaryA = a\n"
            "    a = b\n"
            "    b = temporaryA\n"
            "}\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


}
TEST(TestGeneric, Struct_GenericParameter)
{
    SEMANTIC_ANALYZE(L"struct Stack<T> {\n"
            "    var items : T\n"
            "    mutating func push(item: T) {\n"
            "        //items.append(item)\n"
            "    }\n"
            "    mutating func pop() -> T {\n"
            "        //return items.removeLast()\n"
            "    }\n"
            "}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestGeneric, Use_GenericType)
{
    SEMANTIC_ANALYZE(L"struct Stack<T> {}\n"
            "var stackOfInts : Stack<Int>");
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPlaceHolderPtr stackOfInts;
    TypePtr type, Stack, Int;
    ASSERT_NOT_NULL(stackOfInts = std::dynamic_pointer_cast<SymbolPlaceHolder>(scope->lookup(L"stackOfInts")));
    ASSERT_NOT_NULL(Stack = std::dynamic_pointer_cast<Type>(scope->lookup(L"Stack")));
    ASSERT_NOT_NULL(Int = symbolRegistry.lookupType(L"Int"));
    ASSERT_NOT_NULL(type = stackOfInts->getType());
    ASSERT_EQ(Type::Specialized, type->getCategory());
    ASSERT_EQ(Stack, type->getInnerType());
    ASSERT_EQ(1, type->getGenericArguments()->size());
    ASSERT_EQ(Int, type->getGenericArguments()->get(0));


}

TEST(TestGeneric, GenericConstraint)
{
    SEMANTIC_ANALYZE(L"func findIndex<T: Equatable>(array: Array<T>, valueToFind: T) -> Int? {\n"
            //"    for (index, value) in enumerate(array) {\n"
            //"        if value == valueToFind {\n"
            //"            return index\n"
            //"        }\n"
            //"    }\n"
            //"    return nil\n"
            "}\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestGeneric, GenericConstraint2)
{
    SEMANTIC_ANALYZE(L"class Base {}\n"
            "func test<T : Base>(a : T)\n"
            "{\n"
            "}\n"
            "var a : Int = 3\n"
            "test(\"\");");
//    dumpCompilerResults(compilerResults);
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_2_, res.code);
}


TEST(TestGeneric, GenericConstraint3)
{
    SEMANTIC_ANALYZE(L"func a<T where T.B : Int>(a : T)\n"
        "{\n"
        "    \n"
        "}");
//    dumpCompilerResults(compilerResults);
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_IS_NOT_A_MEMBER_OF_2, res.code);
}
TEST(TestGeneric, GenericConstraint4)
{
    SEMANTIC_ANALYZE(L"protocol DD\n"
        "{\n"
        "    typealias B\n"
        "}\n"
        "\n"
        "func a<T : DD where T.B == Int>(a : T)\n"
        "{\n"
        "    \n"
        "}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestGeneric, GenericConstraint5)
{
    SEMANTIC_ANALYZE(L"func a<T where T == Int>(a : T)\n"
        "{\n"
        "\n"
        "}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_SAME_TYPE_REQUIREMENTS_MAKES_GENERIC_PARAMETER_NON_GENERIC_1, res.code);

}

TEST(TestGeneric, GenericConstraint6)
{
    SEMANTIC_ANALYZE(L"protocol DD\n"
        "{\n"
        "    typealias F\n"
        "}\n"
        "\n"
        "func a<T : DD where T.F == DD>(a : T)\n"
        "{\n"
        "    \n"
        "}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_PROTOCOL_CAN_ONLY_BE_USED_AS_GENERIC_CONSTRAINT_1, res.code);

}
/*
TODO: check unused generic type
TEST(TestGeneric, GenericConstraint7)
{
    SEMANTIC_ANALYZE(L"func test<T1, T2>(a : T1) -> T1\n"
        "{\n"
        "return a;\n"
        "}\n"
        "test(5)");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(1, compilerResults.numResults());
}
*/

TEST(TestGeneric, GenericConstraint7)
{
    SEMANTIC_ANALYZE_F("TestGeneric_GenericConstraint7.swift");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestGeneric, GenericConstraint8)
{
    SEMANTIC_ANALYZE(L"func test<T>(a : Array<Foo<T>>) -> T\n"
        "{\n"
        "    return T();\n"
        "}");
}

/*

protocol DD
{
    typealias F
}
var a : DD
//Protocol '%' can only be used as a generic constraint because it has Self or associated type requirements

----

func test<T1, T2>(a : T1) -> T1
{
return a;
}
test(5)


----



func test<T>(a : Array<Foo<T>>) -> T
{
    return T();
}
T cannot be constructed because it has no accessible initializers
----

class Foo<T>
{
    var a : T;
    init()
    {
    }
}

func test<T>(a : Foo<Foo<T>>) -> T
{
    return a.a.a
}

----

*/
