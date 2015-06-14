/* TestCollection.cpp --
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

TEST(TestCollection, ArrayLiteral)
{
    SEMANTIC_ANALYZE(L"var shoppingList: [String] = [\"Eggs\", \"Milk\"]");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr shoppingList;
    ASSERT_NOT_NULL(shoppingList = scope->lookup(L"shoppingList"));
    ASSERT_EQ(L"Array<String>", shoppingList->getType()->toString());
}


TEST(TestCollection, ArrayLiteral2)
{
    SEMANTIC_ANALYZE(L"var shoppingList: Array<String> = [\"Eggs\", \"Milk\"]");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr shoppingList;
    ASSERT_NOT_NULL(shoppingList = scope->lookup(L"shoppingList"));
    ASSERT_EQ(L"Array<String>", shoppingList->getType()->toString());
}


TEST(TestCollection, ArrayLiteral3)
{
    SEMANTIC_ANALYZE(L"var shoppingList = [\"Eggs\", \"Milk\"]");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr shoppingList;
    ASSERT_NOT_NULL(shoppingList = scope->lookup(L"shoppingList"));
    ASSERT_EQ(L"Array<String>", shoppingList->getType()->toString());
}

TEST(TestCollection, InvalidArray)
{
    SEMANTIC_ANALYZE(L"var a = [\"\", true]");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES, res.code);
}

TEST(TestCollection, InvalidArray2)
{
    SEMANTIC_ANALYZE(L"var b = 3;"
            L"var a = [3.4, 3.4, b, 4.5]");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_ARRAY_CONTAINS_DIFFERENT_TYPES, res.code);
}

TEST(TestCollection, InvalidArray3)
{
    SEMANTIC_ANALYZE(L"var a = []");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_DEFINE_AN_EMPTY_ARRAY_WITHOUT_CONTEXTUAL_TYPE, res.code);
}

TEST(TestCollection, ValidArray)
{
    SEMANTIC_ANALYZE(L"var a = [3.4, 3]");
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_EQ(L"Array<Double>", a->getType()->toString());
}

TEST(TestCollection, ValidArray2)
{
    SEMANTIC_ANALYZE(L"var a : [Int] = []");
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_EQ(L"Array<Int>", a->getType()->toString());
}

TEST(TestCollection, ValidArray3)
{
    SEMANTIC_ANALYZE(L"class Base{}; class Child1 : Base{}; class Child2 : Base{}\n"
            L"var a = [Child1(), Child2()]");
    ASSERT_NO_ERRORS();
    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_EQ(L"Array<Base>", a->getType()->toString());
}

TEST(TestCollection, ValidArray4)
{
    SEMANTIC_ANALYZE(L"class Base{}; class Child1 : Base{}; class Child2 : Base{}\n"
            L"var a : [Child1] = [Child1(), Child2()]");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, res.code);
}


TEST(TestCollection, InitArray)
{
    SEMANTIC_ANALYZE(L"var someInts = [Int]()\n"
            L"someInts = []");
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr someInts;
    ASSERT_NOT_NULL(someInts = scope->lookup(L"someInts"));
    ASSERT_EQ(L"Array<Int>", someInts->getType()->toString());
}


TEST(TestCollection, DictionaryLiteral)
{
    SEMANTIC_ANALYZE(L"var airports: [String: String] = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr airports;
    ASSERT_NOT_NULL(airports = scope->lookup(L"airports"));
    ASSERT_EQ(L"Dictionary<String, String>", airports->getType()->toString());
}


TEST(TestCollection, DictionaryLiteral2)
{
    SEMANTIC_ANALYZE(L"var airports: [String: String?] = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr airports;
    ASSERT_NOT_NULL(airports = scope->lookup(L"airports"));
    ASSERT_EQ(L"Dictionary<String, Optional<String>>", airports->getType()->toString());
}

TEST(TestCollection, DictionaryLiteral3)
{
    SEMANTIC_ANALYZE(L"var airports = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr airports;
    ASSERT_NOT_NULL(airports = scope->lookup(L"airports"));
    ASSERT_EQ(L"Dictionary<String, String>", airports->getType()->toString());
}


TEST(TestCollection, DictionaryLiteral4)
{
    SEMANTIC_ANALYZE(L"class Base{}; class Child1 : Base{}; class Child2 : Base{}\n"
            L"var airports = [\"YYZ\": Child1(), \"DUB\": Child2()]");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr airports;
    ASSERT_NOT_NULL(airports = scope->lookup(L"airports"));
    ASSERT_EQ(L"Dictionary<String, Base>", airports->getType()->toString());
}


TEST(TestCollection, InvalidDictionary)
{
    SEMANTIC_ANALYZE(L"var a = [:]");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_DEFINE_AN_EMPTY_DICTIONARY_WITHOUT_CONTEXTUAL_TYPE, res.code);
}


TEST(TestCollection, InvalidDictionary2)
{
    SEMANTIC_ANALYZE(L"var a = [3 : \"\", 3: true]");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_DICTIONARY_VALUE_CONTAINS_DIFFERENT_TYPES, res.code);
}


TEST(TestCollection, InvalidDictionary3)
{
    SEMANTIC_ANALYZE(L"var a = [true : \"\", 3: \"\"]");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_DICTIONARY_KEY_CONTAINS_DIFFERENT_TYPES, res.code);
}

TEST(TestCollection, DictionaryCount)
{
    SEMANTIC_ANALYZE(L"var airports = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]\n"
            L"println(\"The airports dictionary contains \\(airports.count) items.\")");

    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestCollection, DictionaryEmpty)
{
    SEMANTIC_ANALYZE(L"var airports = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]\n"
            L"if airports.isEmpty {\n"
            L"    println(\"The airports dictionary is empty.\")\n"
            L"} else {\n"
            L"    println(\"The airports dictionary is not empty.\")\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestCollection, DictionarySubscriptSet)
{
    SEMANTIC_ANALYZE(L"var airports = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]\n"
            L"airports[\"LHR\"] = \"London\"\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestCollection, DictionarySubscriptSet2)
{
    SEMANTIC_ANALYZE(L"var airports = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]\n"
            L"airports[\"LHR\"] = nil");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestCollection, DictionarySubscriptGet)
{
    SEMANTIC_ANALYZE(L"var airports = [\"YYZ\": \"Toronto Pearson\", \"DUB\": \"Dublin\"]\n"
            L"if let airportName = airports[\"DUB\"] {\n"
            L"    println(\"The name of the airport is \\(airportName).\")\n"
            L"} else {\n"
            L"    println(\"That airport is not in the airports dictionary.\")\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestCollection, DictionaryInit)
{
    SEMANTIC_ANALYZE(L"var namesOfIntegers = [Int: String]()\n"
            L"namesOfIntegers[16] = \"sixteen\"\n"
            L"namesOfIntegers = [:]");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}
