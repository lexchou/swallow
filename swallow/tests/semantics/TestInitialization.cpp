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
        L"    init()\n"
        L"    {\n"
        L"         self.a = 4\n"
        L"    }\n"
        L"}");
    ASSERT_ERROR(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2);
    ASSERT_EQ(L"a", error->items[0]);
    ASSERT_EQ(L"self", error->items[1]);
}

TEST(TestInitialization, ModifyConstantPropertiesDuringSubClassInitialization2)
{
    SEMANTIC_ANALYZE(L"class Base\n"
            L"{\n"
            L"    let a = 3\n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    init()\n"
            L"    {\n"
            L"         a = 4\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2);
    ASSERT_EQ(L"a", error->items[0]);
    ASSERT_EQ(L"self", error->items[1]);
}
