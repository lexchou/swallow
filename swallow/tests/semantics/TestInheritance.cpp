/* TestInheritance.cpp --
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
#include "semantics/Symbol.h"
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GenericArgument.h"

using namespace Swallow;
using namespace std;

TEST(TestInheritance, BaseClass)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
        L"    var currentSpeed = 0.0\n"
        L"    var description: String {\n"
        L"        return \"traveling at \\(currentSpeed) miles per hour\"\n"
        L"    }\n"
        L"    func makeNoise() {\n"
        L"        // do nothing - an arbitrary vehicle doesn't necessarily make a noise\n"
        L"    }\n"
        L"}\n"
        L"let someVehicle = Vehicle()\n"
        L"\n"
        L"println(\"Vehicle: \\(someVehicle.description)\")");
    ASSERT_NO_ERRORS();
}


TEST(TestInheritance, Subclassing)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
        L"    var currentSpeed = 0.0\n"
        L"    var description: String {\n"
        L"        return \"traveling at \\(currentSpeed) miles per hour\"\n"
        L"    }\n"
        L"    func makeNoise() {\n"
        L"        // do nothing - an arbitrary vehicle doesn't necessarily make a noise\n"
        L"    }\n"
        L"}\n"
        L"class Bicycle: Vehicle {\n"
        L"    var hasBasket = false\n"
        L"}\n"
        L"let bicycle = Bicycle()\n"
        L"bicycle.hasBasket = true\n"
        L"bicycle.currentSpeed = 15.0\n"
        L"println(\"Bicycle: \\(bicycle.description)\")\n"
        L"class Tandem: Bicycle {\n"
        L"    var currentNumberOfPassengers = 0\n"
        L"}\n"
        L"let tandem = Tandem()\n"
        L"tandem.hasBasket = true\n"
        L"tandem.currentNumberOfPassengers = 2\n"
        L"tandem.currentSpeed = 22.0\n"
        L"println(\"Tandem: \\(tandem.description)\")");
    ASSERT_NO_ERRORS();
}

TEST(TestInheritance, Overriding_GlobalFunc)
{
    SEMANTIC_ANALYZE(L"override func test()\n"
        L"{\n"
        L"    \n"
        L"}");
    ASSERT_ERROR(Errors::E_A_CAN_ONLY_BE_SPECIFIED_ON_CLASS_MEMBERS);
    ASSERT_EQ(L"override", error->items[0]);
}


TEST(TestInheritance, Overriding_StructFunc)
{
    SEMANTIC_ANALYZE(L"struct Test {\n"
        L"    override func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_A_CAN_ONLY_BE_SPECIFIED_ON_CLASS_MEMBERS);
    ASSERT_EQ(L"override", error->items[0]);
}

TEST(TestInheritance, Override_NoMatchedFunc)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    func test(a : Int)\n"
        L"    {\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_METHOD_DOES_NOT_OVERRIDE_ANY_METHOD_FROM_ITS_SUPERCLASS);
}


TEST(TestInheritance, Override_MatchedFunc)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}



TEST(TestInheritance, NoOverride_MatchedFunc)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
}


TEST(TestInheritance, ExtensionOverride)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"}\n"
        L"extension Child\n"
        L"{\n"
        L"    override func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_DECLARATIONS_IN_EXTENSIONS_CANNOT_OVERRIDE_YET);
}

TEST(TestInheritance, ExtensionOverrideWithoutKeyword)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"}\n"
        L"extension Child\n"
        L"{\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_DECLARATIONS_IN_EXTENSIONS_CANNOT_OVERRIDE_YET);
}

TEST(TestInheritance, Override)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
        L"    var currentSpeed = 0.0\n"
        L"    var description: String {\n"
        L"        return \"traveling at \\(currentSpeed) miles per hour\"\n"
        L"    }\n"
        L"    func makeNoise() {\n"
        L"        // do nothing - an arbitrary vehicle doesn't necessarily make a noise\n"
        L"    }\n"
        L"}\n"
        L"class Train: Vehicle {\n"
        L"    override func makeNoise() {\n"
        L"        println(\"Choo Choo\")\n"
        L"    }\n"
        L"}\n"
        L"let train = Train()\n"
        L"train.makeNoise()");
    ASSERT_NO_ERRORS();
}

TEST(TestInheritance, Override_Property)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
        L"    var currentSpeed = 0.0\n"
        L"    var description: String {\n"
        L"        return \"traveling at \\(currentSpeed) miles per hour\"\n"
        L"    }\n"
        L"    func makeNoise() {\n"
        L"        // do nothing - an arbitrary vehicle doesn't necessarily make a noise\n"
        L"    }\n"
        L"}\n"
        L"class Car: Vehicle {\n"
        L"    var gear = 1\n"
        L"    override var description: String {\n"
        L"        return \"\\(super.description) in gear \\(gear)\"\n"
        L"    }\n"
        L"}\n"
        L"let car = Car()\n"
        L"car.currentSpeed = 25.0\n"
        L"car.gear = 3\n"
        L"println(\"Car: \\(car.description)\")");
    ASSERT_NO_ERRORS();
}
TEST(TestInheritance, OverridePropertyWithoutKeyword)
{
    SEMANTIC_ANALYZE(L"class TT\n"
        L"{\n"
        L"    var p : String { return \"\"}\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    var p : String { return \"\"}\n"
        L"}");
    ASSERT_ERROR(Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
}

TEST(TestInheritance, OverrideUnmatchedProperty)
{
    SEMANTIC_ANALYZE(L"class TT\n"
        L"{\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    override var p : String { return \"\"}\n"
        L"}");
    ASSERT_ERROR(Errors::E_PROPERTY_DOES_NOT_OVERRIDE_ANY_PROPERTY_FROM_ITS_SUPERCLASS);
}

TEST(TestInheritance, OverrideUnmatchedPropertyDifferentType)
{
    SEMANTIC_ANALYZE(L"class TT\n"
        L"{\n"
        L"    var p : Int { return 3}\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    override var p : String { return \"\"}\n"
        L"}");
    ASSERT_ERROR(Errors::E_PROPERTY_A_WITH_TYPE_B_CANNOT_OVERRIDE_A_PROPERTY_WITH_TYPE_C_3);
    ASSERT_EQ(L"p", error->items[0]);
    ASSERT_EQ(L"String", error->items[1]);
    ASSERT_EQ(L"Int", error->items[2]);
}


TEST(TestInheritance, OverrideNonmutatingPropertyByMutating)
{
    SEMANTIC_ANALYZE(L"\n"
        L"class TT\n"
        L"{\n"
        L"    var p : String { return \"\"}\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    override var p : String {\n"
        L"        get{\n"
        L"            return \"\"\n"
        L"        }\n"
        L"        set{\n"
        L"            \n"
        L"        }\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInheritance, OverrideMutatingPropertyByNonmutating)
{
    SEMANTIC_ANALYZE(L"class TT\n"
        "{\n"
        "    var p : String {\n"
        "        get{return \"\"}\n"
        "        set{}\n"
        "    }\n"
        "}\n"
        "class TTT : TT\n"
        "{\n"
        "    override var p : String {\n"
        "        get{\n"
        "            return \"\"\n"
        "        }\n"
        "\n"
        "    }\n"
        "}");
    ASSERT_ERROR(Errors::E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1);
    ASSERT_EQ(L"p", error->items[0]);
}

TEST(TestInheritance, OverrideMutatingPropertyWithDidSet)
{
    SEMANTIC_ANALYZE(L"\n"
        L"class TT\n"
        L"{\n"
        L"    var p : String {\n"
        L"        get{return \"\"}\n"
        L"        set{}\n"
        L"    }\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    override var p : String {\n"
        L"        didSet\n"
        L"        {\n"
        L"            \n"
        L"        }\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInheritance, OverrideMutatingPropertyWithWillSet)
{
    SEMANTIC_ANALYZE(L"\n"
        L"class TT\n"
        L"{\n"
        L"    var p : String {\n"
        L"        get{return \"\"}\n"
        L"        set{}\n"
        L"    }\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    override var p : String {\n"
        L"        willSet\n"
        L"        {\n"
        L"            \n"
        L"        }\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInheritance, OverrideObserverWithGet)
{
    SEMANTIC_ANALYZE(L"class TT\n"
        L"{\n"
        L"    var p : String = \"\" {\n"
        L"        didSet{}\n"
        L"        willSet{}\n"
        L"    }\n"
        L"}\n"
        L"class TTT : TT\n"
        L"{\n"
        L"    override var p : String {\n"
        L"        get\n"
        L"        {\n"
        L"            return \"\"\n"
        L"        }\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1);
    ASSERT_EQ(L"p", error->items[0]);
}

TEST(TestInheritance, OverrideSubscript)
{
    SEMANTIC_ANALYZE(L"\n"
        L"class Base\n"
        L"{\n"
        L"    subscript(idx : Int) -> Int\n"
        L"    {\n"
        L"        return 3\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override subscript(idx : Int) -> Int\n"
        L"    {\n"
        L"        return 4\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}


TEST(TestInheritance, OverrideSubscript_WithoutOverride)
{
    SEMANTIC_ANALYZE(L"\n"
        L"class Base\n"
        L"{\n"
        L"    subscript(idx : Int) -> Int\n"
        L"    {\n"
        L"        return 3\n"
        L"    }\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"    subscript(idx : Int) -> Int\n"
        L"    {\n"
        L"        return 4\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_OVERRIDING_DECLARATION_REQUIRES_AN_OVERRIDE_KEYWORD);
}
TEST(TestInheritance, OverloadingSubscript)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> String\n"
            L"    {\n"
            L"        return \"\"\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInheritance, OverloadingSubscript2)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    subscript(idx : Int, idx2 : Int) -> Int\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInheritance, OverloadingSubscript3)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    subscript(# idx : Int) -> Int\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInheritance, OverrideUnmatchedSubscript)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override subscript(idx : Int) -> String\n"
            L"    {\n"
            L"        return \"\"\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUBSCRIPT_DOES_NOT_OVERRIDE_ANY_SUBSCRIPT_FROM_ITS_SUPERCLASS);
}
TEST(TestInheritance, OverrideUnmatchedSubscript2)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    override subscript(idx : Int) -> String\n"
            L"    {\n"
            L"        return \"\"\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUBSCRIPT_DOES_NOT_OVERRIDE_ANY_SUBSCRIPT_FROM_ITS_SUPERCLASS);
}
TEST(TestInheritance, OverrideUnmatchedSubscript3)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> String\n"
            L"    {\n"
            L"        return \"\"\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override subscript(#idx : Int) -> String\n"
            L"    {\n"
            L"        return \"\"\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUBSCRIPT_DOES_NOT_OVERRIDE_ANY_SUBSCRIPT_FROM_ITS_SUPERCLASS);
}

TEST(TestInheritance, OverrideMutatingSubscriptWithUnmutating)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        get { return 3}\n"
            L"        set{}\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_CANNOT_OVERRIDE_MUTABLE_PROPERTY_WITH_READONLY_PROPERTY_A_1);
    ASSERT_EQ(L"subscript", error->items[0]);
}
TEST(TestInheritance, OverrideMUnmtatingSubscriptWithMutating)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        get { return 3}\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override subscript(idx : Int) -> Int\n"
            L"    {\n"
            L"        get { return 3}\n"
            L"        set{}\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInheritance, FinalClass)
{
    SEMANTIC_ANALYZE(L"final class Base\n"
       L"{\n"
       L"}\n"
       L"class Child : Base\n"
       L"{\n"
       L"}");
    ASSERT_ERROR(Errors::E_INHERITANCE_FROM_A_FINAL_CLASS_A_1);
    ASSERT_EQ(L"Base", error->items[0]);
}
TEST(TestInheritance, FinalStruct)
{
    SEMANTIC_ANALYZE(L"final struct BB\n"
        L"{\n"
        L"    \n"
        L"}");
    ASSERT_ERROR(Errors::E_A_MODIFIER_CANNOT_BE_APPLIED_TO_THIS_DECLARATION_1);
    ASSERT_EQ(L"final", error->items[0]);
}
TEST(TestInheritance, FinalEnum)
{
    SEMANTIC_ANALYZE(L"final enum BB\n"
        L"{\n"
        L"    \n"
        L"}");
    ASSERT_ERROR(Errors::E_A_MODIFIER_CANNOT_BE_APPLIED_TO_THIS_DECLARATION_1);
    ASSERT_EQ(L"final", error->items[0]);
}
TEST(TestInheritance, FinalProtocol)
{
    SEMANTIC_ANALYZE(L"final protocol BB\n"
        L"{\n"
        L"    \n"
        L"}");
    ASSERT_ERROR(Errors::E_A_MODIFIER_CANNOT_BE_APPLIED_TO_THIS_DECLARATION_1);
    ASSERT_EQ(L"final", error->items[0]);
}
TEST(TestInheritance, FinalTypeAlias)
{
    SEMANTIC_ANALYZE(L"final typealias BB = Int");
    ASSERT_ERROR(Errors::E_A_MODIFIER_CANNOT_BE_APPLIED_TO_THIS_DECLARATION_1);
    ASSERT_EQ(L"final", error->items[0]);
}
TEST(TestInheritance, FinalGlobalLet)
{
    SEMANTIC_ANALYZE(L"final let BB = 3");
    ASSERT_ERROR(Errors::E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL);
}
TEST(TestInheritance, FinalGlobalVar)
{
    SEMANTIC_ANALYZE(L"final var BB = 3");
    ASSERT_ERROR(Errors::E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL);
}
TEST(TestInheritance, FinalLocalVar)
{
    SEMANTIC_ANALYZE(L"func test() {\n"
        L"final var BB = 3\n"
        L"}");
    ASSERT_ERROR(Errors::E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL);
}
TEST(TestInheritance, FinalGlobalProp)
{
    SEMANTIC_ANALYZE(L"final var BB : Int {return 3}");
    ASSERT_ERROR(Errors::E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL);
}
TEST(TestInheritance, FinalGlobalFunc)
{
    SEMANTIC_ANALYZE(L"final func foo() {}");
    ASSERT_ERROR(Errors::E_ONLY_CLASSES_AND_CLASS_MEMBERS_MAY_BE_MARKED_WITH_FINAL);
}

TEST(TestInheritance, OverrideStoredProperty)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        "{\n"
        "    let a = 3\n"
        "}\n"
        "\n"
        "class Child : Base\n"
        "{\n"
        "    override let a = 3\n"
        "}");
    ASSERT_ERROR(Errors::E_CANNOT_OVERRIDE_WITH_A_STORED_PROPERTY_A_1);
    ASSERT_EQ(L"a", error->items[0]);
}



TEST(TestInheritance, OverrideFinalVar)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    final var a : Int {return 3}\n"
        L"    final func test() {}\n"
        L"    final subscript (a : Int) -> Int{ return 3}\n"
        L"}\n"
        L"\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override var a : Int {return 5}\n"
        L"//    override func test(){}\n"
        L"//    override subscript(a : Int) -> Int { return 3}\n"
        L"}");
    ASSERT_ERROR(Errors::E_VAR_OVERRIDES_A_FINAL_VAR);
}
TEST(TestInheritance, OverrideFinalFunc)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    final func test() {}\n"
        L"}\n"
        L"\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override func test(){}\n"
        L"}");
    ASSERT_ERROR(Errors::E_INSTANCE_METHOD_OVERRIDES_A_FINAL_INSTANCE_METHOD);
}
TEST(TestInheritance, OverrideFinalSubscript)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    final subscript (a : Int) -> Int{ return 3}\n"
        L"}\n"
        L"\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override subscript(a : Int) -> Int { return 3}\n"
        L"}");
    ASSERT_ERROR(Errors::E_SUBSCRIPT_OVERRIDES_A_FINAL_SUBSCRIPT);
}

TEST(TestInheritance, ReverseOrderInheritance)
{
    SEMANTIC_ANALYZE(L"typealias A = FFF \n"
                     L"struct FFF : MyProtocol \n"
                     L"{ \n"
                     L"    func f(){} \n"
                     L"} \n"
                     L"protocol MyProtocol \n"
                     L"{ \n"
                     L"    func f() \n"
                     L"} \n"
                     L"var a : MyProtocol = A() \n"
                     L"a.f()");
    ASSERT_NO_ERRORS();
}
