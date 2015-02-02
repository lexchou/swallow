/* TestProperties.cpp --
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

TEST(TestProperties, StoredProperties)
{
    SEMANTIC_ANALYZE(L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"var rangeOfThreeItems = FixedLengthRange(firstValue: 0, length: 3)\n"
            L"rangeOfThreeItems.firstValue = 6");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestProperties, StoredProperties_ConstantStructure)
{
    SEMANTIC_ANALYZE(L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"let rangeOfFourItems = FixedLengthRange(firstValue: 0, length: 4)\n"
            L"rangeOfFourItems.firstValue = 6");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_ASSIGN_TO_A_IN_B_2, res.code);
}
TEST(TestProperties, StoredProperties_ConstantClass)
{
    SEMANTIC_ANALYZE(L"\n"
            L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"class Container\n"
            L"{\n"
            L"    let f = FixedLengthRange(firstValue: 0, length: 4)\n"
            L"}\n"
            L"var f = Container();\n"
            L"f.f.firstValue = 6");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_ASSIGN_TO_THE_RESULT_OF_THIS_EXPRESSION, res.code);
}


TEST(TestProperties, StoredProperties_ConstantClass2)
{
    SEMANTIC_ANALYZE(L"\n"
            L"struct FixedLengthRange {\n"
            L"    var firstValue: Int\n"
            L"    let length: Int\n"
            L"}\n"
            L"class Container\n"
            L"{\n"
            L"    var f = FixedLengthRange(firstValue: 0, length: 4)\n"
            L"}\n"
            L"let f = Container();\n"
            L"f.f.firstValue = 6");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestProperties, LazyProperty)
{
    SEMANTIC_ANALYZE(L"class C\n"
            L"{\n"
            L"    lazy var a = 3\n"
            L"}");
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr C;
    SymbolPtr a;
    ASSERT_NOT_NULL(C = dynamic_pointer_cast<Type>(scope->lookup(L"C")));
    ASSERT_NOT_NULL(a = C->getMember(L"a"));
    ASSERT_TRUE(a->hasFlags(SymbolFlagLazy));
}

TEST(TestProperties, LazyComputedProperty)
{
    SEMANTIC_ANALYZE(L"class C\n"
            L"{\n"
            L"    lazy var a : Int {return 3}"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_LAZY_MAY_NOT_BE_USED_ON_A_COMPUTED_PROPERTY, res.code);

}
TEST(TestProperties, LazyPropertyMustHaveInitializer)
{

    SEMANTIC_ANALYZE(L"class C\n"
            L"{\n"
            L"    lazy var a : Int\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_LAZY_PROPERTIES_MUST_HAVE_AN_INITIALIZER, res.code);
}
TEST(TestProperties, LazyPropertyOnLet)
{
    SEMANTIC_ANALYZE(L"class C\n"
            L"{\n"
            L"    lazy let a = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_LAZY_CANNOT_BE_USED_ON_A_LET, res.code);
}
TEST(TestProperties, LazyPropertyDestructure)
{

    SEMANTIC_ANALYZE(L"class C\n"
            L"{\n"
            L"    lazy var (a, b) = (3, 4)\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_LAZY_CANNOT_DESTRUCTURE_AN_INITIALIZER, res.code);
}

TEST(TestProperties, LazyProperty2)
{
    SEMANTIC_ANALYZE(L"class DataImporter {\n"
            L"            var fileName = \"data.txt\"\n"
            L"        }\n"
            L"\n"
            L"    class DataManager {\n"
            L"        lazy var importer = DataImporter()\n"
            L"        var data = [String]()\n"
            L"            }\n"
            L"\n"
            L"        let manager = DataManager()\n"
            L"        manager.data.append(\"Some data\")\n"
            L"        manager.data.append(\"Some more data\")\n"
            L"println(manager.importer.fileName)");
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestProperties, ComputedProperty)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"struct Size {\n"
            L"    var width = 0.0, height = 0.0\n"
            L"}\n"
            L"struct Rect {\n"
            L"    var origin = Point()\n"
            L"    var size = Size()\n"
            L"    var center: Point {\n"
            L"        get {\n"
            L"            let centerX = origin.x + (size.width / 2)\n"
            L"            let centerY = origin.y + (size.height / 2)\n"
            L"            return Point(x: centerX, y: centerY)\n"
            L"        }\n"
            L"        set(newCenter) {\n"
            L"            origin.x = newCenter.x - (size.width / 2)\n"
            L"            origin.y = newCenter.y - (size.height / 2)\n"
            L"        }\n"
            L"    }\n"
            L"}\n"
            L"var square = Rect(origin: Point(x: 0.0, y: 0.0),\n"
            L"    size: Size(width: 10.0, height: 10.0))\n"
            L"let initialSquareCenter = square.center\n"
            L"square.center = Point(x: 15.0, y: 15.0)\n"
            L"println(\"square.origin is now at (\\(square.origin.x), \\(square.origin.y))\")");
    ASSERT_NO_ERRORS();
}
TEST(TestProperties, ComputedProperty_ShorthandSetter)
{
    SEMANTIC_ANALYZE(L"struct Point {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"struct Size {\n"
            L"    var width = 0.0, height = 0.0\n"
            L"}\n"
            L"struct AlternativeRect {\n"
            L"    var origin = Point()\n"
            L"    var size = Size()\n"
            L"    var center: Point {\n"
            L"        get {\n"
            L"            let centerX = origin.x + (size.width / 2)\n"
            L"            let centerY = origin.y + (size.height / 2)\n"
            L"            return Point(x: centerX, y: centerY)\n"
            L"        }\n"
            L"        set {\n"
            L"            origin.x = newValue.x - (size.width / 2)\n"
            L"            origin.y = newValue.y - (size.height / 2)\n"
            L"        }\n"
            L"    }\n"
            L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestProperties, ReadonlyComputedProperty)
{
    SEMANTIC_ANALYZE(L"struct Cuboid {\n"
            L"    var width = 0.0, height = 0.0, depth = 0.0\n"
            L"    var volume: Double {\n"
            L"        return width * height * depth\n"
            L"    }\n"
            L"}\n"
            L"let fourByFiveByTwo = Cuboid(width: 4.0, height: 5.0, depth: 2.0)\n"
            L"println(\"the volume of fourByFiveByTwo is \\(fourByFiveByTwo.volume)\")");
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr Cuboid;
    SymbolPtr volume;
    ASSERT_NOT_NULL(Cuboid = dynamic_pointer_cast<Type>(scope->lookup(L"Cuboid")));
    ASSERT_NOT_NULL(volume = Cuboid->getMember(L"volume"));
    ASSERT_TRUE(volume->hasFlags(SymbolFlagReadable));
    ASSERT_TRUE(!volume->hasFlags(SymbolFlagWritable));
}

TEST(TestProperties, Observer)
{
    SEMANTIC_ANALYZE(L"class StepCounter {\n"
            L"    var totalSteps: Int = 0 {\n"
            L"        willSet(newTotalSteps) {\n"
            L"            println(\"About to set totalSteps to \\(newTotalSteps)\")\n"
            L"        }\n"
            L"        didSet {\n"
            L"            if totalSteps > oldValue  {\n"
            L"                println(\"Added \\(totalSteps - oldValue) steps\")\n"
            L"            }\n"
            L"        }\n"
            L"    }\n"
            L"}\n"
            L"let stepCounter = StepCounter()\n"
            L"stepCounter.totalSteps = 200\n"
            L"// About to set totalSteps to 200\n"
            L"// Added 200 steps\n"
            L"stepCounter.totalSteps = 360\n"
            L"// About to set totalSteps to 360\n"
            L"// Added 160 steps\n"
            L"stepCounter.totalSteps = 896");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestProperties, GlobalLazy)
{
    SEMANTIC_ANALYZE(L"lazy var a = 3");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_LAZY_IS_ONLY_VALID_FOR_MEMBERS_OF_A_STRUCT_OR_CLASS, res.code);
}

TEST(TestProperties, LocalLazy)
{
    SEMANTIC_ANALYZE(L"class CC\n"
            L"{\n"
            L"    func ff()\n"
            L"    {\n"
            L"        lazy var c = 3\n"
            L"    }\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_LAZY_IS_ONLY_VALID_FOR_MEMBERS_OF_A_STRUCT_OR_CLASS, res.code);
}

TEST(TestProperties, TypeProperty)
{
    SEMANTIC_ANALYZE(
            L"struct SomeStructure {\n"
            L"    static var storedTypeProperty = \"Some value.\"\n"
            L"    static var computedTypeProperty: Int {\n"
            L"        // return an Int value here\n"
            L"    }\n"
            L"}\n"
            L"enum SomeEnumeration {\n"
            L"    static var storedTypeProperty = \"Some value.\"\n"
            L"    static var computedTypeProperty: Int {\n"
            L"        // return an Int value here\n"
            L"    }\n"
            L"}\n"
            L"class SomeClass {\n"
            L"    class var computedTypeProperty: Int {\n"
            L"        // return an Int value here\n"
            L"    }\n"
            L"}\n"
            L"println(SomeClass.computedTypeProperty)\n"
            L"println(SomeStructure.storedTypeProperty)\n"
            L"SomeStructure.storedTypeProperty = \"Another value.\"\n"
            L"println(SomeStructure.storedTypeProperty)"
    );
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}
TEST(TestProperties, StaticVar_Global)
{
    SEMANTIC_ANALYZE(L"static var a = 3");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_STATIC_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE, res.code);
}
TEST(TestProperties, StaticVar_Func)
{
    SEMANTIC_ANALYZE(L"func foo(){\n"
            L"static var a = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_STATIC_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE, res.code);
}

TEST(TestProperties, StaticVar_Class)
{
    SEMANTIC_ANALYZE(L"class MyClass{\n"
            L"static var a = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_STATIC_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_STRUCTS_AND_ENUMS, res.code);
}
/*! TODO: Official swift compiler doesn't support declare class variable under protocol
TEST(TestProperties, StaticVar_Protocol)
{
    SEMANTIC_ANALYZE(L"protocol MyClass{\n"
            L"static var a = 3\n"
            L"}");
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_STATIC_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_STRUCTS_AND_ENUMS, res.code);
}
*/


TEST(TestProperties, ClassVar_Global)
{
    SEMANTIC_ANALYZE(L"class var a = 3");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CLASS_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE, res.code);
}
TEST(TestProperties, ClassVar_Func)
{
    SEMANTIC_ANALYZE(L"func foo(){\n"
            L"class var a = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CLASS_PROPERTIES_MAY_ONLY_BE_DECLARED_ON_A_TYPE, res.code);
}

TEST(TestProperties, ClassVar_Struct)
{
    SEMANTIC_ANALYZE(L"struct MyClass{\n"
            L"class var a = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CLASS_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_CLASSES_AND_PROTOCOLS, res.code);
}

TEST(TestProperties, ClassVar_Enum)
{
    SEMANTIC_ANALYZE(L"enum MyClass{\n"
            L"class var a = 3\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CLASS_PROPERTIES_ARE_ONLY_ALLOWED_WITHIN_CLASSES_AND_PROTOCOLS, res.code);
}


TEST(TestProperties, TypeProperty2)
{
    SEMANTIC_ANALYZE(
            L"struct AudioChannel {\n"
                    L"    static let thresholdLevel = 10\n"
                    L"    static var maxInputLevelForAllChannels = 0\n"
                    L"    var currentLevel: Int = 0 {\n"
                    L"        didSet {\n"
                    L"            if currentLevel > AudioChannel.thresholdLevel {\n"
                    L"                // cap the new audio level to the threshold level\n"
                    L"                currentLevel = AudioChannel.thresholdLevel\n"
                    L"            }\n"
                    L"            if currentLevel > AudioChannel.maxInputLevelForAllChannels {\n"
                    L"                // store this as the new overall maximum input level\n"
                    L"                AudioChannel.maxInputLevelForAllChannels = currentLevel\n"
                    L"            }\n"
                    L"        }\n"
                    L"    }\n"
                    L"}\n"
                    L"var leftChannel = AudioChannel()\n"
                    L"var rightChannel = AudioChannel()\n"
                    L"leftChannel.currentLevel = 7\n"
                    L"println(leftChannel.currentLevel)\n"
                    L"// prints \"7\"\n"
                    L"println(AudioChannel.maxInputLevelForAllChannels)\n"
                    L"rightChannel.currentLevel = 11\n"
                    L"println(rightChannel.currentLevel)\n"
                    L"// prints \"10\"\n"
                    L"println(AudioChannel.maxInputLevelForAllChannels)"
    );
    dumpCompilerResults(compilerResults, content);
    ASSERT_EQ(0, compilerResults.numResults());
}