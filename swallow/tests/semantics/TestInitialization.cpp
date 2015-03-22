/* TestInitialization.cpp --
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

TEST(TestInitialization, Initializers)
{
    SEMANTIC_ANALYZE(L"struct Fahrenheit {\n"
        L"    var temperature: Double\n"
        L"    init() {\n"
        L"        temperature = 32.0\n"
        L"    }\n"
        L"}\n"
        L"var f = Fahrenheit()\n"
        L"println(\"The default temperature is \\(f.temperature)° Fahrenheit\")");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, DefaultInitializers)
{
    SEMANTIC_ANALYZE(L"struct Fahrenheit {\n"
        L"    var temperature: Double = 23\n"
        L"}\n"
        L"var f = Fahrenheit()\n");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, InitializationParameters)
{
    SEMANTIC_ANALYZE(L"struct Celsius {\n"
        L"    var temperatureInCelsius: Double\n"
        L"    init(fromFahrenheit fahrenheit: Double) {\n"
        L"        temperatureInCelsius = (fahrenheit - 32.0) / 1.8\n"
        L"    }\n"
        L"    init(fromKelvin kelvin: Double) {\n"
        L"        temperatureInCelsius = kelvin - 273.15\n"
        L"    }\n"
        L"}\n"
        L"let boilingPointOfWater = Celsius(fromFahrenheit: 212.0)\n"
        L"// boilingPointOfWater.temperatureInCelsius is 100.0\n"
        L"let freezingPointOfWater = Celsius(fromKelvin: 273.15)");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, LocalAndExternalParameterNames)
{
    SEMANTIC_ANALYZE(L"struct Color {\n"
        "    let red, green, blue: Double\n"
        "    init(red: Double, green: Double, blue: Double) {\n"
        "        self.red   = red\n"
        "        self.green = green\n"
        "        self.blue  = blue\n"
        "    }\n"
        "    init(white: Double) {\n"
        "        red   = white\n"
        "        green = white\n"
        "        blue  = white\n"
        "    }\n"
        "}"
        "let magenta = Color(red: 1.0, green: 0.0, blue: 1.0)\n"
        "let halfGray = Color(white: 0.5)");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, LocalAndExternalParameterNames2)
{
    SEMANTIC_ANALYZE(L"struct Color {\n"
        "    let red, green, blue: Double\n"
        "    init(red: Double, green: Double, blue: Double) {\n"
        "        self.red   = red\n"
        "        self.green = green\n"
        "        self.blue  = blue\n"
        "    }\n"
        "    init(white: Double) {\n"
        "        red   = white\n"
        "        green = white\n"
        "        blue  = white\n"
        "    }\n"
        "}"
        "let veryGreen = Color(0.0, 1.0, 0.0)\n");
    ASSERT_ERROR(Errors::E_MISSING_ARGUMENT_LABEL_IN_CALL_1);
}
TEST(TestInitialization, InitializerParametersWithoutExternalNames)
{
    SEMANTIC_ANALYZE(L"struct Celsius {\n"
        L"    var temperatureInCelsius: Double\n"
        L"    init(fromFahrenheit fahrenheit: Double) {\n"
        L"        temperatureInCelsius = (fahrenheit - 32.0) / 1.8\n"
        L"    }\n"
        L"    init(fromKelvin kelvin: Double) {\n"
        L"        temperatureInCelsius = kelvin - 273.15\n"
        L"    }\n"
        L"    init(_ celsius: Double) {\n"
        L"        temperatureInCelsius = celsius\n"
        L"    }\n"
        L"}\n"
        L"let bodyTemperature = Celsius(37.0)");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, OptionalPropertyType)
{
    SEMANTIC_ANALYZE(L"class SurveyQuestion {\n"
        L"    var text: String\n"
        L"    var response: String?\n"
        L"    init(text: String) {\n"
        L"        self.text = text\n"
        L"    }\n"
        L"    func ask() {\n"
        L"        println(text)\n"
        L"    }\n"
        L"}\n"
        L"let cheeseQuestion = SurveyQuestion(text: \"Do you like cheese?\")\n"
        L"cheeseQuestion.ask()\n"
        L"// prints \"Do you like cheese?\"\n"
        L"cheeseQuestion.response = \"Yes, I do like cheese.\"");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, ModifyConstantPropertiesDuringInitialization)
{
    SEMANTIC_ANALYZE(L"class SurveyQuestion {\n"
        L"    let text: String\n"
        L"    var response: String?\n"
        L"    init(text: String) {\n"
        L"        self.text = text\n"
        L"    }\n"
        L"    func ask() {\n"
        L"        println(text)\n"
        L"    }\n"
        L"}\n"
        L"let beetsQuestion = SurveyQuestion(text: \"How about beets?\")\n"
        L"beetsQuestion.ask()\n"
        L"// prints \"How about beets?\"\n"
        L"beetsQuestion.response = \"I also like beets. (But not with cheese.)\"");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, ModifyConstantPropertiesDuringSubClassInitialization)
{
    SEMANTIC_ANALYZE(L"class Base\n"
        L"{\n"
        L"    let a = 3\n"
        L"}\n"
        L"class Child : Base\n"
        L"{\n"
        L"    override init()\n"
        L"    {\n"
        L"         self.a = 4\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_USE_OF_PROPERTY_A_IN_BASE_OBJECT_BEFORE_SUPER_INIT_INITIALIZES_IT);
    ASSERT_EQ(L"a", error->items[0]);
}

TEST(TestInitialization, ModifyConstantPropertiesDuringSubClassInitialization2)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    let a = 3\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override init()\n"
            L"    {\n"
            L"         super.init()\n"
            L"         a = 4\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2);
    ASSERT_EQ(L"a", error->items[0]);
    ASSERT_EQ(L"self", error->items[1]);
}

TEST(TestInitialization, DefaultInitializer)
{
    SEMANTIC_ANALYZE(L"class ShoppingListItem {\n"
            L"    var name: String?\n"
            L"    var quantity = 1\n"
            L"    var purchased = false\n"
            L"}\n"
            L"var item = ShoppingListItem()");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, MemberwiseInitializer)
{
    SEMANTIC_ANALYZE(L"struct Size {\n"
            L"    var width = 0.0, height = 0.0\n"
            L"}\n"
            L"let twoByTwo = Size(width: 2.0, height: 2.0)");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, InitializerDelegation)
{
    SEMANTIC_ANALYZE(L"struct Size {\n"
            "    var width = 0.0, height = 0.0\n"
            "}\n"
            "struct Point {\n"
            "    var x = 0.0, y = 0.0\n"
            "}\n"
            "struct Rect {\n"
            "    var origin = Point()\n"
            "    var size = Size()\n"
            "    init() {}\n"
            "    init(origin: Point, size: Size) {\n"
            "        self.origin = origin\n"
            "        self.size = size\n"
            "    }\n"
            "    init(center: Point, size: Size) {\n"
            "        let originX = center.x - (size.width / 2)\n"
            "        let originY = center.y - (size.height / 2)\n"
            "        self.init(origin: Point(x: originX, y: originY), size: size)\n"
            "    }\n"
            "}\n"
            "let basicRect = Rect()\n"
            "let originRect = Rect(origin: Point(x: 2.0, y: 2.0),\n"
            "    size: Size(width: 5.0, height: 5.0))\n"
            "let centerRect = Rect(center: Point(x: 4.0, y: 4.0),\n"
            "    size: Size(width: 3.0, height: 3.0))");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, OverrideInit)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
            L"    var numberOfWheels = 0\n"
            L"    init() {\n"
            L"    }\n"
            L"    var description: String {\n"
            L"        return \"\\(numberOfWheels) wheel(s)\"\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"class Bicycle: Vehicle {\n"
            L"    override init() {\n"
            L"        super.init()\n"
            L"        numberOfWheels = 2\n"
            L"    }\n"
            L"}\n"
            L"let bicycle = Bicycle()\n"
            L"println(\"Bicycle: \\(bicycle.description)\")");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, InitializerDelegationOutsideInitializer)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
            L"}\n"
            L"\n"
            L"class Bicycle: Vehicle {\n"
            L"    func foo()\n"
            L"    {\n"
            L"        self.init();\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_INITIALIZER_DELEGATION_CAN_ONLY_OCCUR_WITHIN_AN_INITIALIZER);
}
TEST(TestInitialization, SuperInitOutsideInitializer)
{
    SEMANTIC_ANALYZE(L"class Vehicle {\n"
            L"}\n"
            L"\n"
            L"class Bicycle: Vehicle {\n"
            L"    func foo()\n"
            L"    {\n"
            L"        super.init();\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUPER_INIT_CANNOT_BE_CALLED_OUTSIDE_OF_AN_INITIALIZER);
}

TEST(TestInitialization, ConvenienceInitializer)
{
    SEMANTIC_ANALYZE(L"class Food {\n"
            L"    var name: String\n"
            L"    init(name: String) {\n"
            L"        self.name = name\n"
            L"    }\n"
            L"    convenience init() {\n"
            L"        self.init(name: \"[Unnamed]\")\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, OverrideConvenienceInitializer)
{
    SEMANTIC_ANALYZE(L"class Food {\n"
            L"    var name: String\n"
            L"    init(name: String) {\n"
            L"        self.name = name\n"
            L"    }\n"
            L"    convenience init() {\n"
            L"        self.init(name: \"[Unnamed]\")\n"
            L"    }\n"
            L"}\n"
            L"class RecipeIngredient: Food {\n"
            L"    var quantity: Int\n"
            L"    init(name: String, quantity: Int) {\n"
            L"        self.quantity = quantity\n"
            L"        super.init(name: name)\n"
            L"    }\n"
            L"    override convenience init(name: String) {\n"
            L"        self.init(name: name, quantity: 1)\n"
            L"    }\n"
            L"}\n"
            L"let oneMysteryItem = RecipeIngredient()\n"
            L"let oneBacon = RecipeIngredient(name: \"Bacon\")\n"
            L"let sixEggs = RecipeIngredient(name: \"Eggs\", quantity: 6)");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, SelfInitIsntCalledOnAllPaths)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            self.init(a : 3);\n"
            L"        }\n"
            L"        else\n"
            L"        {\n"
            L"            \n"
            L"        }\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SELF_INIT_ISNT_CALLED_ON_ALL_PATHS_IN_DELEGATING_INITIALIZER);
}
TEST(TestInitialization, SelfInitIsntCalledOnAllPaths2)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            self.init(a : 3);\n"
            L"        }\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SELF_INIT_ISNT_CALLED_ON_ALL_PATHS_IN_DELEGATING_INITIALIZER);
}
TEST(TestInitialization, SelfInitIsntCalledOnAllPaths3)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SELF_INIT_ISNT_CALLED_ON_ALL_PATHS_IN_DELEGATING_INITIALIZER);
}
TEST(TestInitialization, SelfInitIsntCalledOnAllPaths4)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"        self.init();\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, SelfInitIsntCalledOnAllPaths5)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            self.init(a : 3);\n"
            L"        }\n"
            L"        self.init(a : 3);\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SELF_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER);
}

TEST(TestInitialization, SelfInitIsntCalledOnAllPaths6)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            self.init(a : 3);\n"
            L"        }\n"
            L"        else\n"
            L"        {\n"
            L"            self.init(a : 3);\n"
            L"        }\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
/*
TEST(TestInitialization, InvalidRedeclarationOfInit)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    convenience init()\n"
            L"    {\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_INVALID_REDECLARATION_1);
}
*/
TEST(TestInitialization, SuperInitIsntCalled)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUPER_INIT_ISNT_CALLED_BEFORE_RETURNING_FROM_INITIALIZER);
}

TEST(TestInitialization, SuperInitIsntCalledOnAllPaths)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            super.init(a : 5);\n"
            L"        }\n"
            L"        else\n"
            L"        {\n"
            L"            super.init(a : 0);\n"
            L"        }\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, SuperInitIsCalledOnAllPaths)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            super.init(a : 5);\n"
            L"        }\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUPER_INIT_ISNT_CALLED_BEFORE_RETURNING_FROM_INITIALIZER);
}
TEST(TestInitialization, SuperInitCalledMultipleTimes)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"        if(a)\n"
            L"        {\n"
            L"            super.init(a : 5);\n"
            L"        }\n"
            L"        super.init(a : 5);\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SUPER_INIT_CALLED_MULTIPLE_TIMES_IN_INITIALIZER);
}

TEST(TestInitialization, MustCallDesignatedInitializer)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init()\n"
            L"    {\n"
            L"    }\n"
            L"    convenience init(a : Int)\n"
            L"    {\n"
            L"        self.init()\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"        super.init(a: 3);\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_MUST_CALL_A_DESIGNATED_INITIALIZER_OF_THE_SUPER_CLASS_A_1);
    ASSERT_EQ(L"Base", error->items[0]);
}

TEST(TestInitialization, DesignatedInitializerCannotDelegateSelfInit)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init()\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"        self.init();\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_DESIGNATED_INITIALIZER_FOR_A_CANNOT_DELEGATE_1);
    ASSERT_EQ(L"Base", error->items[0]);
}
TEST(TestInitialization, ConvenienceInitializerMustDelegateWithSelfInit)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init(a : Int)\n"
            L"    {\n"
            L"    }\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    \n"
            L"    convenience init(a : Bool)\n"
            L"    {\n"
            L"        super.init(a: 3);\n"
            L"    }\n"
            L"}")
    ASSERT_ERROR(Errors::E_CONVENIENCE_INITIALIZER_FOR_A_MUST_DELEGATE_WITH_SELF_INIT_1);
    ASSERT_EQ(L"Child", error->items[0]);
}
TEST(TestInitialization, DelegateAfterFieldInitialized)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"}\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"  var a : Int\n"
            L"  init(a : Int)\n"
            L"  {\n"
            L"    super.init();\n"
            L"  }\n"
            L"}");
    ASSERT_ERROR(Errors::E_PROPERTY_A_NOT_INITIALIZED_AT_SUPER_INIT_CALL_1);
    ASSERT_EQ(L"self.a", error->items[0]);
}
TEST(TestInitialization, DelegateAfterFieldInitialized2)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"}\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"  var a : Int\n"
            L"  init(a : Int)\n"
            L"  {\n"
            L"    self.a = 3;\n"
            L"    super.init();\n"
            L"  }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, DelegateAfterFieldInitialized3)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"}\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"  var a : Int = 3\n"
            L"  init(a : Int)\n"
            L"  {\n"
            L"    super.init();\n"
            L"  }\n"
            L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, VariableInitializedInInitializer)
{
   SEMANTIC_ANALYZE(L"class Test\n"
           L"{\n"
           L"    var a : String\n"
           L"    init()\n"
           L"    {\n"
           L"        a = \"\"\n"
           L"    }\n"
           L"    func foo()\n"
           L"    {\n"
           L"        println(a);\n"
           L"    }\n"
           L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, ChangeParentPropertyBeforeSuperInit)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"}\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init(b : Int)\n"
            L"    {\n"
            L"        a = 3\n"
            L"        super.init();\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_USE_OF_PROPERTY_A_IN_BASE_OBJECT_BEFORE_SUPER_INIT_INITIALIZES_IT);
}
TEST(TestInitialization, ChangeParentPropertyBeforeSuperInit2)
{
   SEMANTIC_ANALYZE(L"class Base\n"
           L"{\n"
           L"    var a : Int = 5\n"
           L"}\n"
           L"\n"
           L"\n"
           L"class Child : Base\n"
           L"{\n"
           L"    \n"
           L"    init(b : Int)\n"
           L"    {\n"
           L"        if(b == 0)\n"
           L"        {\n"
           L"          super.init()\n"
           L"        }\n"
           L"        \n"
           L"        a = 5\n"
           L"    }\n"
           L"}");
    ASSERT_ERROR(Errors::E_USE_OF_PROPERTY_A_IN_BASE_OBJECT_BEFORE_SUPER_INIT_INITIALIZES_IT);
}
TEST(TestInitialization, ChangeParentPropertyBeforeSuperInit3)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    \n"
            L"    init(b : Int)\n"
            L"    {\n"
            L"        if(b == 0)\n"
            L"        {\n"
            L"          super.init()\n"
            L"        }\n"
            L"        else\n"
            L"        {\n"
            L"          super.init()\n"
            L"        }\n"
            L"        \n"
            L"        a = 5\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, ChangeParentPropertyBeforeSuperInit4)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"}\n"
            L"\n"
            L"\n"
            L"class Child : Base\n"
            L"{\n"
            L"    \n"
            L"    init(b : Int)\n"
            L"    {\n"
            L"        super.init()\n"
            L"        a = 5\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}


TEST(TestInitialization, UseOfSelfBeforeSelfInitCalled)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"    init()\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"    convenience init(b : Int)\n"
            L"    {\n"
            L"        a = 3\n"
            L"        self.init()\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED);
}
TEST(TestInitialization, UseOfSelfBeforeSelfInitCalled2)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"    init()\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"    convenience init(b : Int)\n"
            L"    {\n"
            L"        self.init()\n"
            L"        a = 3\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, UseOfSelfBeforeSelfInitCalled3)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override init()\n"
            L"    {\n"
            L"        super.init()\n"
            L"    }\n"
            L"    convenience init(b : Int)\n"
            L"    {\n"
            L"        a = 3\n"
            L"        self.init()\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED);
}
TEST(TestInitialization, UseOfSelfBeforeSelfInitCalled4)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    override init()\n"
            L"    {\n"
            L"        super.init()\n"
            L"    }\n"
            L"    convenience init(b : Int)\n"
            L"    {\n"
            L"        self.init()\n"
            L"        a = 3\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, UseOfSelfBeforeFirstPhraseInitialization)
{
    SEMANTIC_ANALYZE(L"class T\n"
            L"{\n"
            L"}\n"
            L"\n"
            L"class Base : T\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"    override init()\n"
            L"    {\n"
            L"        self.foo()\n"
            L"        super.init()\n"
            L"    }\n"
            L"    convenience init(b : Int)\n"
            L"    {\n"
            L"    }\n"
            L"    \n"
            L"    func foo()\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_SELF_USED_BEFORE_SUPER_INIT_CALL);
}
TEST(TestInitialization, UseOfSelfBeforeFirstPhraseInitialization2)
{
    SEMANTIC_ANALYZE(L"class T\n"
            L"{\n"
            L"}\n"
            L"\n"
            L"class Base : T\n"
            L"{\n"
            L"    var a : Int = 5\n"
            L"    override init()\n"
            L"    {\n"
            L"        super.init()\n"
            L"    }\n"
            L"    convenience init(b : Int)\n"
            L"    {\n"
            L"        self.foo()\n"
            L"        self.init()\n"
            L"    }\n"
            L"    \n"
            L"    func foo()\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_USE_OF_SELF_IN_DELEGATING_INITIALIZER_BEFORE_SELF_INIT_IS_CALLED);
}

TEST(TestInitialization, MultipleInitializers)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    var a : Int\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"        self.a = 3;\n"
            L"    }\n"
            L"    \n"
            L"    init(a : String)\n"
            L"    {\n"
            L"    }\n"
            L"\n"
            L"    func foo()\n"
            L"    {\n"
            L"        println(a);\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_PROPERTY_A_NOT_INITIALIZED)
}
TEST(TestInitialization, MultipleInitializers2)
{
    SEMANTIC_ANALYZE(L"\n"
            L"class Base\n"
            L"{\n"
            L"    var a : Int\n"
            L"    init(a : Bool)\n"
            L"    {\n"
            L"        self.a = 3;\n"
            L"    }\n"
            L"    \n"
            L"    init(a : String)\n"
            L"    {\n"
            L"        self.a = 4;\n"
            L"    }\n"
            L"\n"
            L"    func foo()\n"
            L"    {\n"
            L"        println(a);\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestInitialization, FailableInitializer)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    var a : Int\n"
            L"    init?()\n"
            L"    {\n"
            L"        a = 4\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, FailableInitializer2)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init?()\n"
            L"    {\n"
            L"        return nil\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestInitialization, FailableInitializer3)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init()\n"
            L"    {\n"
            L"        return nil\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_ONLY_A_FAILABLE_INITIALIZER_CAN_RETURN_NIL);
}
TEST(TestInitialization, FailableInitializer4)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init?()\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_NIL_IS_THE_ONLY_RETURN_VALUE_PERMITTED_IN_AN_INITIALIZER);
}
TEST(TestInitialization, FailableInitializer5)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    init()\n"
            L"    {\n"
            L"        return 3\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_NIL_IS_THE_ONLY_RETURN_VALUE_PERMITTED_IN_AN_INITIALIZER);
}
