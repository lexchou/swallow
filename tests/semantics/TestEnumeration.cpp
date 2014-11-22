/* TestEnumeration.cpp --
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

TEST(TestEnumeration, Declaration)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(Type::Enum, CompassPoint->getCategory());

}

TEST(TestEnumeration, CaseAccess)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}\n"
            L"var a = CompassPoint.East;");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a;
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(CompassPoint, a->getType());

}

TEST(TestEnumeration, CaseAccess_ShorterSyntax)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}\n"
            L"var a : CompassPoint;\n"
            L"a = .North");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a;
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(CompassPoint, a->getType());

}

TEST(TestEnumeration, CaseAccess_SwitchCase)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}\n"
            L"var directionToHead : CompassPoint = .North;\n"
            L"directionToHead = .North\n"
            L"switch directionToHead {\n"
            L"case .North:\n"
            L"        break\n"
            L"case .South:\n"
            L"        break\n"
            L"case .East:\n"
            L"        break\n"
            L"case .West:\n"
            L"        break\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr directionToHead;
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(directionToHead = scope->lookup(L"directionToHead"));
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(CompassPoint, directionToHead->getType());

}

TEST(TestEnumeration, AssociatedValues)
{
    SEMANTIC_ANALYZE(L"enum Barcode {\n"
            L"    case UPCA(Int, Int, Int, Int)\n"
            L"    case QRCode(String)\n"
            L"}\n"
            L"var a = Barcode.UPCA(3, 4, 5, 6)\n");
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPlaceHolderPtr a;
    TypePtr Barcode;

    ASSERT_NOT_NULL(Barcode = dynamic_pointer_cast<Type>(scope->lookup(L"Barcode")));
    ASSERT_NOT_NULL(a = dynamic_pointer_cast<SymbolPlaceHolder>(scope->lookup(L"a")));

    ASSERT_EQ(Barcode, a->getType());
}

TEST(TestEnumeration, PartialApplication)
{
    SEMANTIC_ANALYZE(L"enum Barcode {\n"
            L"    case UPCA(Int, Int, Int, Int)\n"
            L"    case QRCode(String)\n"
            L"}\n"
            L"var productBarcode = Barcode.UPCA\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_PARTIAL_APPLICATION_OF_ENUM_CONSTRUCTOR_IS_NOT_ALLOWED, res.code);
}

TEST(TestEnumeration, AssociatedValues_SwitchCase)
{
    SEMANTIC_ANALYZE(L"enum Barcode {\n"
            L"    case UPCA(Int, Int, Int, Int)\n"
            L"    case QRCode(String)\n"
            L"}\n"
            L"var productBarcode = Barcode.UPCA(3, 4, 5, 6)\n"
            L"switch productBarcode {\n"
            L"case .UPCA(let numberSystem, let manufacturer, let product, let check):\n"
            L"    println(\"UPC-A: \\(numberSystem), \\(manufacturer), \\(product), \\(check).\")\n"
            L"case .QRCode(let productCode):\n"
            L"    println(\"QR code: \\(productCode).\")\n"
            L"}\n"
            L"\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPlaceHolderPtr a;
    TypePtr Barcode;

    ASSERT_NOT_NULL(Barcode = dynamic_pointer_cast<Type>(scope->lookup(L"Barcode")));
    ASSERT_NOT_NULL(a = dynamic_pointer_cast<SymbolPlaceHolder>(scope->lookup(L"productBarcode")));

}

TEST(TestEnumeration, RawValues)
{
    SEMANTIC_ANALYZE(L"enum ASCIIControlCharacter: Character {\n"
            L"    case Tab = \"\\t\"\n"
            L"    case LineFeed = \"\\n\"\n"
            L"    case CarriageReturn = \"\\r\"\n"
            L"}");
    dumpCompilerResults(compilerResults);

    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestEnumeration, RawValueMustAppearFirst)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {}"
            L"enum ASCIIControlCharacter: MyProtocol, Character {\n"
            L"    case Tab = \"\\t\"\n"
            L"    case LineFeed = \"\\n\"\n"
            L"    case CarriageReturn = \"\\r\"\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_RAW_TYPE_A_MUST_APPEAR_FIRST_IN_THE_ENUM_INHERITANCE_CLAUSE_1, res.code);
}


TEST(TestEnumeration, MultipleEnumRawTypes)
{
    SEMANTIC_ANALYZE(L"enum ASCIIControlCharacter: String, Character {\n"
            L"    case Tab = \"\\t\"\n"
            L"    case LineFeed = \"\\n\"\n"
            L"    case CarriageReturn = \"\\r\"\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_MULTIPLE_ENUM_RAW_TYPES_A_AND_B_2, res.code);
}
TEST(TestEnumeration, RawTypeIsNotConvertibleFromLiteral)
{
    SEMANTIC_ANALYZE(L"class MyClass{}\n"
            L"enum Test : MyClass\n"
            L"{\n"
            L"case A = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_RAW_TYPE_A_IS_NOT_CONVERTIBLE_FROM_ANY_LITERAL_1, res.code);
}

TEST(TestEnumeration, RawTypeIsNotConvertibleFromLiteral2)
{
    SEMANTIC_ANALYZE(L"class MyClass : IntegerLiteralConvertible, Equatable{}\n"
            L"enum Test : MyClass\n"
            L"{\n"
            L"case A = 3\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestEnumeration, CannotBeSynthesizedBecauseNotEquatable)
{
    SEMANTIC_ANALYZE(L"class MyClass : IntegerLiteralConvertible{}\n"
            L"enum Test : MyClass\n"
            L"{\n"
            L"case A = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_RAWREPRESENTABLE_INIT_CANNOT_BE_SYNTHESIZED_BECAUSE_RAW_TYPE_A_IS_NOT_EQUATABLE_1, res.code);
}

TEST(TestEnumeration, RawValuesWithNoCases)
{
    SEMANTIC_ANALYZE(L"enum Test : Int\n"
            L"{\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_ENUM_WITH_NO_CASES_CANNOT_DECLARE_A_RAW_TYPE, res.code);
}

TEST(TestEnumeration, RawValueCaseMustBeLiteral_Int)
{
    SEMANTIC_ANALYZE(L"let one = 1;"
            L"enum Test : Int\n"
            L"{"
            L"    case A = one\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_RAW_VALUE_FOR_ENUM_CASE_MUST_BE_LITERAL, res.code);
}

TEST(TestEnumeration, RawValueCaseMustBeLiteral_String)
{
    SEMANTIC_ANALYZE(L"let one = \"one\";"
            L"enum Test : String\n"
            L"{"
            L"    case A = one\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_RAW_VALUE_FOR_ENUM_CASE_MUST_BE_LITERAL, res.code);
}


TEST(TestEnumeration, RawValueCaseMustBeLiteral_String2)
{
    SEMANTIC_ANALYZE(L"enum Test : String\n"
            L"{"
            L"    case A = \"one\"\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}



TEST(TestEnumeration, RawValueCannotConvertToRawType)
{
    SEMANTIC_ANALYZE(L"enum Test : Int\n"
            L"{"
            L"    case A = \"one\"\n"
            L"}");

    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_IS_NOT_CONVERTIBLE_TO_B_2, res.code);
}

TEST(TestEnumeration, EnumCaseRequireValueWhenRawTypeIsIntegerLiteralConvertible)
{
    SEMANTIC_ANALYZE(L"enum Test : String\n"
            L"{"
            L"    case A\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_ENUM_CASES_REQUIRE_EXPLICIT_RAW_VALUES_WHEN_THE_RAW_TYPE_IS_NOT_INTEGER_LITERAL_CONVERTIBLE, res.code);
}

TEST(TestEnumeration, EnumCaseRequireValueWhenRawTypeIsIntegerLiteralConvertible2)
{
    SEMANTIC_ANALYZE(L"enum Test : Int\n"
            L"{"
            L"    case A\n"
            L"}");
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestEnumeration, EnumCaseRequireValueWhenRawTypeIsIntegerLiteralConvertible3)
{
    SEMANTIC_ANALYZE(L"enum Test : Double\n"
            L"{"
            L"    case A\n"
            L"}");
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestEnumeration, ImplicitRawRepresentable)
{
    SEMANTIC_ANALYZE(L"enum Planet: Int {\n"
            L"    case Mercury = 1, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune\n"
            L"}\n"
            L"let earthsOrder = Planet.Earth.rawValue");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr earthsOrder;
    ASSERT_NOT_NULL(earthsOrder = scope->lookup(L"earthsOrder"));
    ASSERT_EQ(symbolRegistry.getGlobalScope()->Int, earthsOrder->getType());
}

TEST(TestEnumeration, ImplicitRawRepresentable2)
{
    SEMANTIC_ANALYZE(L"enum Planet {\n"
            L"    case Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune\n"
            L"}\n"
            L"let earthsOrder = Planet.Earth.rawValue");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_DOES_NOT_HAVE_A_MEMBER_2, res.code);
}