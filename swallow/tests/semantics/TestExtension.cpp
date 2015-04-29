/* TestExtension.cpp --
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

TEST(TestExtension, ExtensionWithIncorrectType)
{
    SEMANTIC_ANALYZE(L"extension SomeType { \n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_USE_OF_UNDECLARED_TYPE_1, res.code);
}


TEST(TestExtension, ExtensionUnderBadScope)
{
    SEMANTIC_ANALYZE(L"class SomeType { \n"
            L"  extension SomeType {}\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, res.code);
}


TEST(TestExtension, ExtensionOfProtocol)
{
    SEMANTIC_ANALYZE(L"protocol SomeProtocol {} \n"
            L"  extension SomeProtocol {}\n");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_PROTOCOL_A_CANNOT_BE_EXTENDED_1, res.code);
}


TEST(TestExtension, ExtensionCannotContainStoredProperties)
{
    SEMANTIC_ANALYZE(L"  extension Int {var a : Int }\n");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_EXTENSIONS_MAY_NOT_CONTAIN_STORED_PROPERTIES, res.code);
}


TEST(TestExtension, ExtensionOfDouble)
{
    SEMANTIC_ANALYZE(L"protocol SomeProtocol {} \n"
            L"  extension Double : SomeProtocol {}\n");
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestExtension, ExtensionOfDouble2)
{
    SEMANTIC_ANALYZE(L"extension Double {\n"
            L"    var km: Double { return self * 1_000.0 }\n"
            L"    var m: Double { return self }\n"
            L"    var cm: Double { return self / 100.0 }\n"
            L"    var mm: Double { return self / 1_000.0 }\n"
            L"    var ft: Double { return self / 3.28084 }\n"
            L"}\n"
            L"let oneInch = 25.4.mm\n"
            L"println(\"One inch is \\(oneInch) meters\")\n"
            L"// prints \"One inch is 0.0254 meters\"\n"
            L"let threeFeet = 3.ft\n"
            L"println(\"Three feet is \\(threeFeet) meters\")");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestExtension, Initializers)
{
    SEMANTIC_ANALYZE(L"struct Size {\n"
            L"    var width = 0.0, height = 0.0\n"
            L"}\n"
            L"struct Point {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"struct Rect {\n"
            L"    var origin = Point()\n"
            L"    var size = Size()\n"
            L"}\n"
            L"let defaultRect = Rect()\n"
            L"let memberwiseRect = Rect(origin: Point(x: 2.0, y: 2.0),\n"
            L"    size: Size(width: 5.0, height: 5.0))\n"
            L"extension Rect {\n"
            L"    init(center: Point, size: Size) {\n"
            L"        let originX = center.x - (size.width / 2)\n"
            L"        let originY = center.y - (size.height / 2)\n"
            L"        self.init(origin: Point(x: originX, y: originY), size: size)\n"
            L"    }\n"
            L"}\n"
            L"let centerRect = Rect(center: Point(x: 4.0, y: 4.0),\n"
            L"    size: Size(width: 3.0, height: 3.0))");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestExtension, Methods)
{
    SEMANTIC_ANALYZE(L"extension Int {\n"
            L"    func repetitions(task: () -> ()) {\n"
            L"        var i : Int\n"
            L"        for i = 0; i < self; i++ {\n"
            L"            task()\n"
            L"        }\n"
            L"    }\n"
            L"}\n"
            L"3.repetitions({\n"
            L"    println(\"Hello!\")\n"
            L"})\n"
            L"3.repetitions {\n"
            L"    println(\"Goodbye!\")\n"
            L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestExtension, MutaingMethods)
{
    SEMANTIC_ANALYZE(L"extension Int {\n"
            L"    mutating func square() {\n"
            L"        self = self * self\n"
            L"    }\n"
            L"}\n"
            L"var someInt = 3\n"
            L"someInt.square()");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestExtension, Subscript)
{
    //TODO use *= and --  ref: https://developer.apple.com/library/ios/documentation/Swift/Conceptual/Swift_Programming_Language/Extensions.html#//apple_ref/doc/uid/TP40014097-CH24-XID_229
    SEMANTIC_ANALYZE(L"extension Int {\n"
            L"    subscript(var digitIndex: Int) -> Int {\n"
            L"        var decimalBase = 1\n"
            L"        while digitIndex > 0 {\n"
            L"            decimalBase = decimalBase * 10\n"
            L"            digitIndex = digitIndex - 1\n"
            L"        }\n"
            L"        return (self / decimalBase) % 10\n"
            L"    }\n"
            L"}\n"
            "var a = 343434[2]");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestExtension, NestedType)
{
    SEMANTIC_ANALYZE_F("semantics/TestExtension_NestedTypes.swift");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestExtension, Redeclaration_Property)
{
    SEMANTIC_ANALYZE(L"class TestClass\n"
            L"{\n"
            L"    var a : Int { return 3}\n"
            L"}\n"
            L"extension TestClass\n"
            L"{\n"
            L"    var a : Int { return 3}\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_INVALID_REDECLARATION_1, res.code);
}

TEST(TestExtension, Redeclaration_Property2)
{
    SEMANTIC_ANALYZE(L"class TestClass\n"
            L"{\n"
            L"    func a() {}\n"
            L"}\n"
            L"extension TestClass\n"
            L"{\n"
            L"    var a : Int { return 3}\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_INVALID_REDECLARATION_1, res.code);
}



TEST(TestExtension, Redeclaration_Func)
{
    SEMANTIC_ANALYZE(L"class TestClass\n"
            L"{\n"
            L"    var a : Int { return 3}\n"
            L"}\n"
            L"extension TestClass\n"
            L"{\n"
            L"    func a() {}\n"
            L"}");
    ASSERT_ERROR(Errors::E_INVALID_REDECLARATION_1);
}

TEST(TestExtension, Redeclaration_Func2)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class TestClass\n"
            L"{\n"
            L"    func a() -> Bool{ return true}\n"
            L"}\n"
            L"extension TestClass\n"
            L"{\n"
            L"    func a(){}\n"
            L"}");
    ASSERT_EQ(0, compilerResults.numResults());
}



TEST(TestExtension, Redeclaration_Subscription)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class TestClass\n"
            L"{\n"
            L"    subscript(a : Int) -> Int { return 3 }\n"
            L"}\n"
            L"extension TestClass\n"
            L"{\n"
            L"    subscript(a : Int) -> Bool {return true;}\n"
            L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestExtension, Redeclaration_Init)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class TestClass\n"
            L"{\n"
            L"    init(a : Int) {}\n"
            L"}\n"
            L"extension TestClass\n"
            L"{\n"
            L"    init(a : Int) {}\n"
            L"}");
    ASSERT_ERROR(Errors::E_INVALID_REDECLARATION_1);
}

TEST(TestExtension, ProtocolImplmentedByExtension)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol\n"
            L"{\n"
            L"    func test()\n"
            L"}\n"
            L"struct Test : MyProtocol\n"
            L"{\n"
            L"\n"
            L"}\n"
            L"extension Test :MyProtocol\n"
            L"{\n"
            L"    func test()\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}\n"
            L"var a = Test()\n"
            L"a.test()");
    ASSERT_NO_ERRORS();
}