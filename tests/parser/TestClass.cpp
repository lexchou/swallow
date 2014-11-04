/* TestClass.cpp --
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
#include "common/Errors.h"

using namespace Swallow;

TEST(TestClass, testStruct)
{
    PARSE_STATEMENT(L"struct Resolution {"
                       L"var width = 0"
                       L"var height = 0"
                       L"}");
    StructDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(L"Resolution", std::static_pointer_cast<TypeIdentifier>(s->getIdentifier())->getName());
    ASSERT_EQ(2, s->numDeclarations());
}
TEST(TestClass, StructWithEof)
{
    PARSE_STATEMENT(L"struct Resolution ");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_EXPECT_1, res.code);
}
TEST(TestClass, testClass)
{
    PARSE_STATEMENT(L"class VideoMode { "
                       L"var resolution = Resolution() "
                       L"var interlaced = false "
                       L"var frameRate = 0.0 "
                       L"var name: String? "
                       L"}");
    ClassDefPtr c;
    ValueBindingsPtr vars;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(L"VideoMode", std::static_pointer_cast<TypeIdentifier>(c->getIdentifier())->getName());
    ASSERT_EQ(4, c->numDeclarations());

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(c->getDeclaration(0)));
    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(c->getDeclaration(1)));
    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(c->getDeclaration(2)));
    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(c->getDeclaration(3)));



}
TEST(TestClass, testComputedProperty)
{
    PARSE_STATEMENT(L"struct Rect {"
                       L"var origin = Point()"
                       L"var center: Point {"
                       L"    get {"
                       L"        return origin"
                       L"    }"
                       L"    set(newCenter) {"
                       L"        origin = newCenter"
                       L"    }"
                       L"}"
                       L"}");
    StructDefPtr s;
    IdentifierPtr id;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());
    {
        ValueBindingsPtr vars;
        ValueBindingPtr var;
        FunctionCallPtr call;
        ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
        ASSERT_EQ(1, vars->numBindings());
        ASSERT_NOT_NULL(var = vars->get(0));
        ASSERT_EQ(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        ASSERT_EQ(0, call->getArguments()->numExpressions());
        ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQ(L"Point", id->getIdentifier());
    }
    {
        ComputedPropertyPtr prop;
        CodeBlockPtr cb;
        ASSERT_NOT_NULL(prop = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
        ASSERT_EQ(L"center", prop->getName());
        ASSERT_NULL(prop->getInitializer());
        ASSERT_NULL(prop->getDidSet());
        ASSERT_NULL(prop->getWillSet());
        ASSERT_NOT_NULL(cb = prop->getGetter());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));


        ASSERT_NOT_NULL(cb = prop->getSetter());
        ASSERT_EQ(L"newCenter", prop->getSetterName());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<Assignment>(cb->getStatement(0)));
    }

}


TEST(TestClass, testShorthandSetter)
{
    PARSE_STATEMENT(L"struct Rect {"
                       L"var origin = Point()"
                       L"var center: Point {"
                       L"    get {"
                       L"        return origin"
                       L"    }"
                       L"    set {"
                       L"        origin = newCenter"
                       L"    }"
                       L"}"
                       L"}");
    StructDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());
    {
        IdentifierPtr id;
        ValueBindingsPtr vars;
        ValueBindingPtr var;
        FunctionCallPtr call;
        ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
        ASSERT_EQ(1, vars->numBindings());
        ASSERT_NOT_NULL(var = vars->get(0));
        ASSERT_EQ(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        ASSERT_EQ(0, call->getArguments()->numExpressions());
        ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQ(L"Point", id->getIdentifier());
    }
    {
        ComputedPropertyPtr prop;
        CodeBlockPtr cb;
        ASSERT_NOT_NULL(prop = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
        ASSERT_EQ(L"center", prop->getName());
        ASSERT_NULL(prop->getInitializer());
        ASSERT_NULL(prop->getDidSet());
        ASSERT_NULL(prop->getWillSet());
        ASSERT_NOT_NULL(cb = prop->getGetter());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));


        ASSERT_NOT_NULL(cb = prop->getSetter());
        ASSERT_EQ(L"", prop->getSetterName());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<Assignment>(cb->getStatement(0)));
    }

}

TEST(TestClass, testShorthandSetter2)
{
    PARSE_STATEMENT(L"struct Rect {"
                       L"var origin = Point()"
                       L"var center: Point {"
                       L"    set {"
                       L"        origin = newCenter"
                       L"    }"
                       L"    get {"
                       L"        return origin"
                       L"    }"
                       L"}"
                       L"}");
    StructDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());
    {
        ValueBindingsPtr vars;
        ValueBindingPtr var;
        IdentifierPtr id;
        FunctionCallPtr call;
        ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
        ASSERT_EQ(1, vars->numBindings());
        ASSERT_NOT_NULL(var = vars->get(0));
        ASSERT_EQ(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        ASSERT_EQ(0, call->getArguments()->numExpressions());
        ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQ(L"Point", id->getIdentifier());
    }

    {
        ComputedPropertyPtr prop;
        CodeBlockPtr cb;
        ASSERT_NOT_NULL(prop = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
        ASSERT_EQ(L"center", prop->getName());
        ASSERT_NULL(prop->getInitializer());
        ASSERT_NULL(prop->getDidSet());
        ASSERT_NULL(prop->getWillSet());
        ASSERT_NOT_NULL(cb = prop->getGetter());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));


        ASSERT_NOT_NULL(cb = prop->getSetter());
        ASSERT_EQ(L"", prop->getSetterName());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<Assignment>(cb->getStatement(0)));

    }
}

TEST(TestClass, testShorthandSetter3)
{
    PARSE_STATEMENT(L"struct Rect {"
                       L"var origin = Point()"
                       L"var center: Point {"
                       L"    get {"
                       L"        return origin"
                       L"    }"
                       L"}"
                       L"}");
    StructDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());
    {
        ValueBindingsPtr vars;
        ValueBindingPtr var;
        IdentifierPtr id;
        FunctionCallPtr call;
        ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
        ASSERT_EQ(1, vars->numBindings());
        ASSERT_NOT_NULL(var = vars->get(0));
        ASSERT_EQ(L"origin", std::dynamic_pointer_cast<Identifier>(var->getName())->getIdentifier());
        ASSERT_NOT_NULL(call = std::dynamic_pointer_cast<FunctionCall>(var->getInitializer()));
        ASSERT_EQ(0, call->getArguments()->numExpressions());
        ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(call->getFunction()));
        ASSERT_EQ(L"Point", id->getIdentifier());
    }
    {
        ComputedPropertyPtr prop;
        CodeBlockPtr cb;
        ASSERT_NOT_NULL(prop = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
        ASSERT_EQ(L"center", prop->getName());
        ASSERT_NULL(prop->getInitializer());
        ASSERT_NULL(prop->getDidSet());
        ASSERT_NULL(prop->getWillSet());
        ASSERT_NOT_NULL(cb = prop->getGetter());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));


        ASSERT_NULL(prop->getSetter());
    }
}

TEST(TestClass, testReadonlyComputedProperty)
{
    PARSE_STATEMENT(L"struct Cuboid { "
                       L"var width = 0.0, height = 0.0, depth = 0.0 "
                       L"var volume: Double { "
                       L"    return width * height * depth "
                       L"} "
                       L"}");
    StructDefPtr s;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());
    {
        ComputedPropertyPtr prop;
        CodeBlockPtr cb;
        ASSERT_NOT_NULL(prop = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
        ASSERT_EQ(L"volume", prop->getName());
        ASSERT_NULL(prop->getInitializer());
        ASSERT_NULL(prop->getDidSet());
        ASSERT_NULL(prop->getWillSet());
        ASSERT_NULL(prop->getSetter());
        ASSERT_NOT_NULL(cb = prop->getGetter());
        ASSERT_EQ(1, cb->numStatements());
        ASSERT_NOT_NULL(std::dynamic_pointer_cast<ReturnStatement>(cb->getStatement(0)));
    }
}
TEST(TestClass, testPropertyObservers)
{
    PARSE_STATEMENT(L"class StepCounter { "
                       L"var totalSteps: Int = 0 { "
                       L"    willSet(newTotalSteps) { "
                       L"        println(\"About to set totalSteps to \(newTotalSteps)\") "
                       L"    } "
                       L"    didSet { "
                       L"        if totalSteps > oldValue  { "
                       L"            println(\"Added \(totalSteps - oldValue) steps\") "
                       L"        } "
                       L"    } "
                       L"} "
                       L"}");

    ClassDefPtr s;
    ComputedPropertyPtr var;
    CodeBlockPtr cb;
    IntegerLiteralPtr i;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(0)));
    ASSERT_EQ(L"totalSteps", var->getName());
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
    ASSERT_NULL(var->getSetter());
    ASSERT_NULL(var->getGetter());


    ASSERT_NOT_NULL(cb = var->getWillSet());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(cb->getStatement(0)));

    ASSERT_NOT_NULL(cb = var->getDidSet());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<IfStatement>(cb->getStatement(0)));


}

TEST(TestClass, testPropertyObservers2)
{
    PARSE_STATEMENT(L"class StepCounter { "
                       L"var totalSteps: Int = 0 { "
                       L"    didSet { "
                       L"        if totalSteps > oldValue  { "
                       L"            println(\"Added \(totalSteps - oldValue) steps\") "
                       L"        } "
                       L"    } "
                       L"    willSet(newTotalSteps) { "
                       L"        println(\"About to set totalSteps to \(newTotalSteps)\") "
                       L"    } "
                       L"} "
                       L"}");

    ClassDefPtr s;
    ComputedPropertyPtr prop;
    CodeBlockPtr cb;
    IntegerLiteralPtr i;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(prop = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(0)));
    ASSERT_EQ(L"totalSteps", prop->getName());
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(prop->getInitializer()));
    ASSERT_NULL(prop->getSetter());
    ASSERT_NULL(prop->getGetter());


    ASSERT_NOT_NULL(cb = prop->getWillSet());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(cb->getStatement(0)));

    ASSERT_NOT_NULL(cb = prop->getDidSet());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<IfStatement>(cb->getStatement(0)));


}

TEST(TestClass, testPropertyObservers3)
{
    PARSE_STATEMENT(L"class StepCounter { "
                       L"var totalSteps: Int = 0 { "
                       L"    willSet(newTotalSteps) { "
                       L"        println(\"About to set totalSteps to \(newTotalSteps)\") "
                       L"    } "
                       L"} "
                       L"}");

    ClassDefPtr s;
    ComputedPropertyPtr var;
    CodeBlockPtr cb;
    IntegerLiteralPtr i;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(0)));
    ASSERT_EQ(L"totalSteps", var->getName());
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(var->getInitializer()));
    ASSERT_NULL(var->getSetter());
    ASSERT_NULL(var->getGetter());


    ASSERT_NOT_NULL(cb = var->getWillSet());
    ASSERT_EQ(1, cb->numStatements());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<FunctionCall>(cb->getStatement(0)));

    ASSERT_NULL(var->getDidSet());

}

void testTypeProperty_Struct()
{
    PARSE_STATEMENT(L"struct SomeStructure {"
                       L"static var storedTypeProperty = \"Some value.\""
                       L"static var computedTypeProperty: Int {"
                       L"    // return an Int value here\n"
                       L"}"
                       L"}");
    StructDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());


    ValueBindingsPtr storedTypeProperty;
    ASSERT_NOT_NULL(storedTypeProperty = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Static, storedTypeProperty->getSpecifiers());

    ComputedPropertyPtr computedTypeProperty;
    ASSERT_NOT_NULL(computedTypeProperty = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
    ASSERT_EQ((int)TypeSpecifier::Static, computedTypeProperty->getSpecifiers());

}

void testTypeProperty_Class()
{
    PARSE_STATEMENT(L"class SomeStructure {"
                       L"static var storedTypeProperty = \"Some value.\""
                       L"static var computedTypeProperty: Int {"
                       L"    // return an Int value here\n"
                       L"}"
                       L"}");
    ClassDefPtr s;

    ValueBindingsPtr vars;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(2, s->numDeclarations());

    ValueBindingsPtr storedTypeProperty;
    ASSERT_NOT_NULL(storedTypeProperty = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Static, storedTypeProperty->getSpecifiers());

    ComputedPropertyPtr computedTypeProperty;
    ASSERT_NOT_NULL(computedTypeProperty = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
    ASSERT_EQ((int)TypeSpecifier::Static, computedTypeProperty->getSpecifiers());


}

void testTypeProperty_Enum()
{
    PARSE_STATEMENT(L"enum SomeStructure {"
                       L"static var storedTypeProperty = \"Some value.\""
                       L"static var computedTypeProperty: Int {"
                       L"    // return an Int value here\n"
                       L"}"
                       L"}");
    EnumDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(2, s->numDeclarations());

    ValueBindingsPtr storedTypeProperty;
    ASSERT_NOT_NULL(storedTypeProperty = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Static, storedTypeProperty->getSpecifiers());

    ComputedPropertyPtr computedTypeProperty;
    ASSERT_NOT_NULL(computedTypeProperty = std::dynamic_pointer_cast<ComputedProperty>(s->getDeclaration(1)));
    ASSERT_EQ((int)TypeSpecifier::Static, computedTypeProperty->getSpecifiers());

}

TEST(TestClass, testMethod)
{
    PARSE_STATEMENT(L"struct Point { "
                       L"var x = 0.0, y = 0.0 "
                       L"mutating func moveByX(deltaX: Double, y deltaY: Double) { "
                       L"    x += deltaX "
                       L"    y += deltaY "
                       L"} "
                       L"}");

    StructDefPtr s;
    ValueBindingsPtr vars;
    FunctionDefPtr f;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
    ASSERT_EQ(0, vars->getSpecifiers());

    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(1)));
    ASSERT_EQ((int)TypeSpecifier::Mutating, f->getSpecifiers());

}

void testMethod_Enum()
{
    PARSE_STATEMENT(L"enum TriStateSwitch { "
                       L"case Off, Low, High "
                       L"mutating func next() { "
                       L"    switch self { "
                       L"    case Off: "
                       L"        self = Low "
                       L"    case Low: "
                       L"        self = High "
                       L"    case High: "
                       L"        self = Off "
                       L"    } "
                       L"} "
                       L"}");

    EnumDefPtr s;
    FunctionDefPtr f;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Mutating, f->getSpecifiers());


}

void testTypeMethod_Struct()
{
    PARSE_STATEMENT(L"struct SomeClass {\n"
                       L"static func someTypeMethod() {\n"
                       L"    // type method implementation goes here\n"
                       L"}\n"
                       L"}");

    StructDefPtr s;
    FunctionDefPtr f;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Static, f->getSpecifiers());

}

void testTypeMethod_Enum()
{
    PARSE_STATEMENT(L"enum SomeClass {\n"
                       L"static func someTypeMethod() {\n"
                       L"    // type method implementation goes here\n"
                       L"}\n"
                       L"}");

    EnumDefPtr s;
    FunctionDefPtr f;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<EnumDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Static, f->getSpecifiers());

}

void testTypeMethod_Class()
{
    PARSE_STATEMENT(L"class SomeClass {\n"
                       L"class func someTypeMethod() {\n"
                       L"    // type method implementation goes here\n"
                       L"}\n"
                       L"}");

    ClassDefPtr s;
    FunctionDefPtr f;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, s->numDeclarations());


    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(s->getDeclaration(0)));
    ASSERT_EQ((int)TypeSpecifier::Class, f->getSpecifiers());

}

void testSubscript_Struct()
{
    PARSE_STATEMENT(L"struct TimesTable {\n"
                       L"let multiplier: Int\n"
                       L"subscript(index: Int) -> Int {\n"
                       L"    return multiplier * index\n"
                       L"}\n"
                       L"}");

    StructDefPtr s;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());
    SubscriptDefPtr sub;

    ASSERT_NOT_NULL(sub = std::dynamic_pointer_cast<SubscriptDef>(s->getDeclaration(1)));

}
TEST(TestClass, testSubscriptOptions)
{
    PARSE_STATEMENT(
                       L"struct Matrix {\n"
                       L"    subscript(row: Int, column: Int) -> Double {\n"
                       L"        get {\n"
                       L"            return grid[(row * columns) + column]\n"
                       L"        }\n"
                       L"        set {\n"
                       L"            grid[(row * columns) + column] = newValue\n"
                       L"        }\n"
                       L"    }\n"
                       L"}\n");

    StructDefPtr s;
    ParameterPtr p;
    ParametersPtr args;
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(1, s->numDeclarations());
    SubscriptDefPtr sub;

    ASSERT_NOT_NULL(sub = std::dynamic_pointer_cast<SubscriptDef>(s->getDeclaration(0)));
    ASSERT_NOT_NULL(args = sub->getParameters());
    ASSERT_EQ(2, args->numParameters());
    ASSERT_NOT_NULL(p =  args->getParameter(0));
    ASSERT_EQ(L"row", p->getLocalName());
    ASSERT_NOT_NULL(p =  args->getParameter(1));
    ASSERT_EQ(L"column", p->getLocalName());

}

TEST(TestClass, testSubclass)
{
    PARSE_STATEMENT(L"class Bicycle: Vehicle {"
                       L"init() {"
                       L"    super.init()"
                       L"    numberOfWheels = 2"
                       L"}"
                       L"}");
    ClassDefPtr c;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, c->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
    ASSERT_EQ(L"Vehicle", t->getName());


    InitializerDefPtr init;
    ASSERT_EQ(1, c->numDeclarations());
    ASSERT_NOT_NULL(init = std::dynamic_pointer_cast<InitializerDef>(c->getDeclaration(0)));



}

void testOverride_Func()
{
    PARSE_STATEMENT(L"class Car: Vehicle {\n"
                    L"var speed: Double = 0.0\n"
                    L"init() {\n"
                    L"    super.init()\n"
                    L"    maxPassengers = 5\n"
                    L"    numberOfWheels = 4\n"
                    L"}\n"
                    L"override func description() -> String {\n"
                    L"    return super.description() + \"; \"\n"
                    L"    + \"traveling at \(speed) mph\"\n"
                    L"}\n"
                    L"}");

    ClassDefPtr c;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, c->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
    ASSERT_EQ(L"Vehicle", t->getName());


    FunctionDefPtr f;
    ASSERT_EQ(3, c->numDeclarations());
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionDef>(c->getDeclaration(2)));
    ASSERT_EQ(L"description", f->getName());
    ASSERT_EQ((int)TypeSpecifier::Override, f->getSpecifiers());



}
void testOverride_Var()
{
    PARSE_STATEMENT(L"class SpeedLimitedCar: Car {\n"
                    L"override var speed: Double  {\n"
                    L"    get {\n"
                    L"        return super.speed\n"
                    L"    }\n"
                    L"    set {\n"
                    L"        super.speed = min(newValue, 40.0)\n"
                    L"    }\n"
                    L"}\n"
                    L"}");


    ClassDefPtr c;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, c->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
    ASSERT_EQ(L"Car", t->getName());


    ValueBindingsPtr vars;
    ASSERT_EQ(1, c->numDeclarations());
    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(c->getDeclaration(0)));
    ASSERT_EQ(1, vars->numBindings());
    ComputedPropertyPtr var;
    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(vars->get(0)));
    ASSERT_EQ(L"speed", var->getName());
    ASSERT_EQ((int)TypeSpecifier::Override, var->getSpecifiers());


}

void testOverride_PropertyObservers()
{
    PARSE_STATEMENT(L"class AutomaticCar: Car {\n"
                    L"var gear = 1\n"
                    L"override var speed: Double {\n"
                    L"    didSet {\n"
                    L"        gear = Int(speed / 10.0) + 1\n"
                    L"    }\n"
                    L"}\n"
                    L"override func description() -> String {\n"
                    L"    return super.description() + \" in gear \(gear)\"\n"
                    L"}\n"
                    L"}");


    ClassDefPtr c;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, c->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
    ASSERT_EQ(L"Car", t->getName());


    ComputedPropertyPtr var;
    ASSERT_EQ(3, c->numDeclarations());
    ASSERT_NOT_NULL(var = std::dynamic_pointer_cast<ComputedProperty>(c->getDeclaration(1)));
    ASSERT_EQ(L"speed", var->getName());
    ASSERT_EQ((int)TypeSpecifier::Override, var->getSpecifiers());

}


TEST(TestClass, testFinalAttribute)
{
    PARSE_STATEMENT(L"@final class AutomaticCar: Car {\n"
                    L"@final var gear = 1\n"
                    L"@final func description() -> String {\n"
                    L"    return super.description() + \" in gear \(gear)\"\n"
                    L"}\n"
                    L"@final subscript(index: Int) -> Int {\n"
                    L"    return multiplier * index\n"
                    L"}\n"
                    L"}");


    ClassDefPtr c;
    TypeIdentifierPtr t;
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));

    ASSERT_EQ(1, (int)c->getAttributes().size());
    ASSERT_EQ(L"final", c->getAttributes().front()->getName());


    ASSERT_EQ(1, c->numParents());
    ASSERT_NOT_NULL(t = std::dynamic_pointer_cast<TypeIdentifier>(c->getParent(0)));
    ASSERT_EQ(L"Car", t->getName());

    ASSERT_EQ(3, c->numDeclarations());
    ValueBindingsPtr vars;
    FunctionDefPtr func;
    SubscriptDefPtr sub;

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(c->getDeclaration(0)));
    ASSERT_EQ(1, (int)vars->getAttributes().size());
    ASSERT_EQ(L"final", vars->getAttributes().front()->getName());


    ASSERT_NOT_NULL(func = std::dynamic_pointer_cast<FunctionDef>(c->getDeclaration(1)));
    ASSERT_EQ(1, (int)func->getAttributes().size());
    ASSERT_EQ(L"final", func->getAttributes().front()->getName());


    ASSERT_NOT_NULL(sub = std::dynamic_pointer_cast<SubscriptDef>(c->getDeclaration(2)));
    ASSERT_EQ(1, (int)sub->getAttributes().size());
    ASSERT_EQ(L"final", sub->getAttributes().front()->getName());


}
TEST(TestClass, testInit)
{
    PARSE_STATEMENT(L"struct Color {\n"
                    L"let red = 0.0, green = 0.0, blue = 0.0\n"
                    L"init(red: Double, green: Double, blue: Double) {\n"
                    L"  self.red   = red\n"
                    L"  self.green = green\n"
                    L"  self.blue  = blue\n"
                    L"}\n"
                    L"}\n");
    StructDefPtr s;
    ValueBindingsPtr let;
    InitializerDefPtr init;
    ParametersPtr params;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(2, s->numDeclarations());

    ASSERT_NOT_NULL(let = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(0)));
    ASSERT_NOT_NULL(init = std::dynamic_pointer_cast<InitializerDef>(s->getDeclaration(1)));
    ASSERT_FALSE(init->isConvenience());
    ASSERT_NOT_NULL(params = std::dynamic_pointer_cast<Parameters>(init->getParameters()));
    ASSERT_EQ(3, params->numParameters());


}



TEST(TestClass, testConvenienceInitializer)
{
    PARSE_STATEMENT(L"class Food {\n"
                    L"  var name: String\n"
                    L"  init(name: String) {\n"
                    L"      self.name = name\n"
                    L"  }\n"
                    L"  convenience init() {\n"
                    L"      self.init(name: \"[Unnamed]\")\n"
                    L"  }\n"
                    L"}");

    ClassDefPtr s;

    InitializerDefPtr init;
    ParametersPtr params;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(3, s->numDeclarations());

    ASSERT_NOT_NULL(init = std::dynamic_pointer_cast<InitializerDef>(s->getDeclaration(2)));
    ASSERT_TRUE(init->isConvenience());
    ASSERT_NOT_NULL(params = std::dynamic_pointer_cast<Parameters>(init->getParameters()));
    ASSERT_EQ(0, params->numParameters());
}


TEST(TestClass, testDefaultValueWithClosure)
{

    PARSE_STATEMENT(L"class SomeClass {\n"
                    L"let someProperty: SomeType = {\n"
                    L"    return someValue\n"
                    L"}()\n"
                    L"}");


}





TEST(TestClass, testDeinit)
{
    PARSE_STATEMENT(L"class Player {\n"
                    L"var coinsInPurse: Int\n"
                    L"init(coins: Int) {\n"
                    L"    coinsInPurse = Bank.vendCoins(coins)\n"
                    L"}\n"
                    L"func winCoins(coins: Int) {\n"
                    L"    coinsInPurse += Bank.vendCoins(coins)\n"
                    L"}\n"
                    L"deinit {\n"
                    L"    Bank.receiveCoins(coinsInPurse)\n"
                    L"}\n"
                    L"}");

    ClassDefPtr s;
    DeinitializerDefPtr deinit;
    CodeBlockPtr body;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(4, s->numDeclarations());

    ASSERT_NOT_NULL(deinit = std::dynamic_pointer_cast<DeinitializerDef>(s->getDeclaration(3)));
    ASSERT_NOT_NULL(body = deinit->getBody());
    ASSERT_EQ(1, body->numStatements());


}


TEST(TestClass, testWeakReference)
{
    PARSE_STATEMENT(L"class Apartment {\n"
                    L"let number: Int\n"
                    L"init(number: Int) { self.number = number }\n"
                    L"weak var tenant: Person?\n"
                    L"deinit { println(\"Apartment #\(number) is being deinitialized\") }\n"
                    L"}");

    ClassDefPtr s;
    ValueBindingsPtr vars;


    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(4, s->numDeclarations());

    ASSERT_NOT_NULL(vars = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(2)));
    ASSERT_TRUE(vars->getSpecifiers() && TypeSpecifier::Weak);

}

TEST(TestClass, testUnownedReference)
{
    PARSE_STATEMENT(L"class CreditCard {\n"
                    L"let number: Int\n"
                    L"unowned let customer: Customer\n"
                    L"init(number: Int, customer: Customer) {\n"
                    L"    self.number = number\n"
                    L"    self.customer = customer\n"
                    L"}\n"
                    L"deinit { println(\"Card #\(number) is being deinitialized\") }\n"
                    L"}");

    ClassDefPtr s;
    ValueBindingsPtr let;


    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(4, s->numDeclarations());

    ASSERT_NOT_NULL(let = std::dynamic_pointer_cast<ValueBindings>(s->getDeclaration(1)));
    ASSERT_TRUE(let->getSpecifiers() && TypeSpecifier::Unowned);

}

void testNestedType_Struct()
{
    PARSE_STATEMENT(L"class Foo\n"
                    L"{\n"
                    L"  struct Bar\n"
                    L"  {\n"
                    L"  }\n"
                    L"}");
    ClassDefPtr c;
    StructDefPtr s;

    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(root));
    ASSERT_EQ(1, c->numDeclarations());
    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(c->getDeclaration(0)));

}

void testNestedType_Class()
{
    PARSE_STATEMENT(L"struct Foo\n"
                    L"{\n"
                    L"  class Bar\n"
                    L"  {\n"
                    L"  }\n"
                    L"}");
    ClassDefPtr c;
    StructDefPtr s;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(1, s->numDeclarations());
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<ClassDef>(s->getDeclaration(0)));

}


void testNestedType_Enum()
{
    PARSE_STATEMENT(L"struct Foo\n"
                    L"{\n"
                    L"  enum Bar\n"
                    L"  {\n"
                    L"  }\n"
                    L"}");
    EnumDefPtr c;
    StructDefPtr s;

    ASSERT_NOT_NULL(s = std::dynamic_pointer_cast<StructDef>(root));
    ASSERT_EQ(1, s->numDeclarations());
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<EnumDef>(s->getDeclaration(0)));

}

