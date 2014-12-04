/* TestOptional.cpp --
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
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/GlobalScope.h"
#include "semantics/ScopedNodes.h"
#include "semantics/FunctionSymbol.h"
#include "semantics/GenericArgument.h"
#include "common/Errors.h"


using namespace Swallow;

TEST(TestOptional, Type)
{
    SEMANTIC_ANALYZE(L"var a : Int? = 3")
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a = scope->lookup(L"a");
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(L"Optional<Int>", a->getType()->toString());

}
TEST(TestOptional, Type2)
{
    SEMANTIC_ANALYZE(L"var a : Int?? = 3");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a = scope->lookup(L"a");
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(L"Optional<Optional<Int>>", a->getType()->toString());
}

TEST(TestOptional, Type3)
{
    SEMANTIC_ANALYZE(L"var a : (Int??, Int??\?) = (3, 4)");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a = scope->lookup(L"a");
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(L"(Optional<Optional<Int>>, Optional<Optional<Optional<Int>>>)", a->getType()->toString());
}


TEST(TestOptional, Type4)
{
    SEMANTIC_ANALYZE(L"func test(a : Int??\?){}\n"
            L"test(3)");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestOptional, Type5)
{
    SEMANTIC_ANALYZE(L"func test(a : Int??\?){}\n"
            L"test(nil)");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestOptional, NilComparison)
{
    SEMANTIC_ANALYZE(L"var a : Int? = 3;\n"
            L"var b = a == nil;");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr b = scope->lookup(L"b");
    ASSERT_NOT_NULL(b);
    ASSERT_EQ(symbolRegistry.getGlobalScope()->Bool, b->getType());
}


TEST(TestOptional, ForcedValue)
{
    SEMANTIC_ANALYZE(
            L"class Residence {\n"
                    L"    var numberOfRooms = 1\n"
                    L"}\n"
            L"class Person {\n"
            L"    var residence: Residence?\n"
            L"}\n"
            L"\n"
            L"let john = Person()\n"
            L"let roomCount = john.residence!.numberOfRooms");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr roomCount = scope->lookup(L"roomCount");
    ASSERT_NOT_NULL(roomCount);
    ASSERT_EQ(symbolRegistry.getGlobalScope()->Int, roomCount->getType());
}

TEST(TestOptional, ForcedValue2)
{
    SEMANTIC_ANALYZE(
            L"var a : Int??? = 3\n"
                    L"var b = a!");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr b = scope->lookup(L"b");
    ASSERT_NOT_NULL(b);
    ASSERT_EQ(L"Optional<Optional<Int>>", b->getType()->toString());
}
TEST(TestOptional, OptionalChaining)
{
    SEMANTIC_ANALYZE(
            L"class Residence {\n"
                    L"    var numberOfRooms = 1\n"
                    L"}\n"
                    L"class Person {\n"
                    L"    var residence: Residence?\n"
                    L"}\n"
                    L"\n"
                    L"let john = Person()\n"
                    L"let roomCount = john.residence?.numberOfRooms");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr roomCount = scope->lookup(L"roomCount");
    ASSERT_NOT_NULL(roomCount);
    ASSERT_EQ(L"Optional<Int>", roomCount->getType()->toString());
}

TEST(TestOptional, OptionalChaining2)
{
    SEMANTIC_ANALYZE(
            L"var a : Int??? = 3\n"
                    L"var b = a?");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr b = scope->lookup(L"b");
    ASSERT_NOT_NULL(b);
    ASSERT_EQ(L"Optional<Optional<Int>>", b->getType()->toString());
}



TEST(TestOptional, OptionalChaining3)
{
    SEMANTIC_ANALYZE(
            L"class Residence {\n"
                    L"    var numberOfRooms = 1\n"
                    L"}\n"
                    L"class Person {\n"
                    L"    var residence: Residence?\n"
                    L"}\n"
                    L"\n"
                    L"let john = Person()\n"
                    L"if let roomCount = john.residence?.numberOfRooms {\n"
                    "    println(\"John's residence has \\(roomCount) room(s).\")\n"
                    "} else {\n"
                    "    println(\"Unable to retrieve the number of rooms.\")\n"
                    "}\n");


    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());

    IfStatementPtr _if = std::dynamic_pointer_cast<IfStatement>(root->getStatement(3));
    ASSERT_NOT_NULL(_if);
    ScopedCodeBlockPtr then = std::dynamic_pointer_cast<ScopedCodeBlock>(_if->getThen());
    ASSERT_NOT_NULL(then);
    SymbolScope* thenScope = then->getScope();
    ASSERT_NOT_NULL(thenScope);
    SymbolPtr roomCount = thenScope->lookup(L"roomCount");
    ASSERT_NOT_NULL(roomCount);
    ASSERT_EQ(L"Int", roomCount->getType()->toString());
}

TEST(TestOptional, OptionalChaining_Property)
{
    SEMANTIC_ANALYZE_F("semantics/TestOptional_OptionalChaining_Property.swift");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestOptional, OptionalChaining_Method)
{
    SEMANTIC_ANALYZE_F("semantics/TestOptional_OptionalChaining_Method.swift");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestOptional, OptionalChaining_Subscript)
{
    SEMANTIC_ANALYZE_F("semantics/TestOptional_OptionalChaining_Subscript.swift");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
