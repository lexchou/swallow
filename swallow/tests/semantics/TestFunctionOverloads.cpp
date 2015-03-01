/* TestFunctionOverloads.cpp --
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


using namespace Swallow;


TEST(TestFunctionOverloads, testFunc)
{
    SEMANTIC_ANALYZE(L"func test() -> String {return \"\"}\n"
        L"let a = test()");
    dumpCompilerResults(compilerResults);
    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_String = symbolRegistry.lookupType(L"String");
    ASSERT_TRUE(type == t_String);
}

TEST(TestFunctionOverloads, testOverloadedFunc)
{
    SEMANTIC_ANALYZE(L"func test(a : Int) -> String {return \"\"}\n"
            L"func test(a : String) -> Bool {return true}\n"
            L"let a = test(56), b = test(\"str\")");

    dumpCompilerResults(compilerResults);
    SymbolPtr a, b;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_String = symbolRegistry.lookupType(L"String");
    ASSERT_TRUE(type == t_String);


    ASSERT_NOT_NULL(b = scope->lookup(L"b"));
    type = b->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
    ASSERT_TRUE(type == t_Bool);
}
TEST(TestFunctionOverloads, testNamedParameterOverload)
{
    SEMANTIC_ANALYZE(L"func test(a : Int) -> String {return \"\"}\n"
            L"func test(# a : Int) -> Bool {return true}\n"
            L"func test(ExternalName a : String) -> Double {return 3.0}\n"
            L"let a = test(56), b = test(a : 33), c = test(ExternalName : \"str\")");
    dumpCompilerResults(compilerResults);
    SymbolPtr a, b, c;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_String = symbolRegistry.lookupType(L"String");
    ASSERT_TRUE(type == t_String);


    ASSERT_NOT_NULL(b = scope->lookup(L"b"));
    type = b->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
    ASSERT_TRUE(type == t_Bool);


    ASSERT_NOT_NULL(c = scope->lookup(L"c"));
    type = c->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Double = symbolRegistry.lookupType(L"Double");
    ASSERT_TRUE(type == t_Double);
}
TEST(TestFunctionOverloads, testVariadicParametersOverload)
{
    SEMANTIC_ANALYZE(L"func test(a : Int, b : Int) -> String {return \"\"}\n"
    L"func test(a : Int...) -> Int {return 3}\n"
    L"let a = test(0, 1)");

    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_AMBIGUOUS_USE_1, r.code);
    ASSERT_EQ(L"test", r.items[0]);
}
TEST(TestFunctionOverloads, testVariadicParametersOverload2)
{
    SEMANTIC_ANALYZE(
            L"func foo(a : Int, b : Bool) -> Int {return 0;}\n"
            L"func foo(a : Int...) -> Bool{return false;}\n"
            L"let a = foo(0), b = foo(0, true), c = foo(0, 1), d = foo()\n")

    dumpCompilerResults(compilerResults);

    SymbolPtr a, b, c, d;
    TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
    TypePtr t_Int = symbolRegistry.lookupType(L"Int");

    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    ASSERT_TRUE(type == t_Bool);


    ASSERT_NOT_NULL(b = scope->lookup(L"b"));
    type = b->getType();
    ASSERT_NOT_NULL(type);
    ASSERT_TRUE(type == t_Int);


    ASSERT_NOT_NULL(c = scope->lookup(L"c"));
    type = c->getType();
    ASSERT_NOT_NULL(type);
    ASSERT_TRUE(type == t_Bool);


    ASSERT_NOT_NULL(d = scope->lookup(L"d"));
    ASSERT_NOT_NULL(type = d->getType());
    ASSERT_TRUE(type == t_Bool);
}
TEST(TestFunctionOverloads, testCovarianceOverload)
{
    SEMANTIC_ANALYZE(
            L"func bar(a : Int)->Bool{return true}\n"
                    L"func bar(a : Float) -> Int {return 3}\n"
                    L"let a = bar(3), b = bar(3.4)");
    SymbolPtr a, b;
    TypePtr type;
    ASSERT_EQ(0, compilerResults.numResults());

    TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
    TypePtr t_Int = symbolRegistry.lookupType(L"Int");

    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(type = a->getType());
    ASSERT_TRUE(type == t_Bool);

    ASSERT_NOT_NULL(b = scope->lookup(L"b"));
    ASSERT_NOT_NULL(type = b->getType());
    ASSERT_TRUE(type == t_Int);
}


TEST(TestFunctionOverloads, testCovarianceOverload2)
{
    SEMANTIC_ANALYZE(
        L"func bar(a : Bool)->Bool{return true}\n"
            L"func bar(a : Float) -> Int {return 3}\n"
            L"let a = bar(true), b = bar(3)");
    SymbolPtr a, b;
    TypePtr type;
    ASSERT_EQ(0, compilerResults.numResults());

    TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
    TypePtr t_Int = symbolRegistry.lookupType(L"Int");

    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(type = a->getType());
    ASSERT_TRUE(type == t_Bool);

    ASSERT_NOT_NULL(b = scope->lookup(L"b"));
    ASSERT_NOT_NULL(type = b->getType());
    ASSERT_TRUE(type == t_Int);
}

TEST(TestFunctionOverloads, testCovarianceOverload3)
{
    SEMANTIC_ANALYZE(
            L"func bar(a : Double)->Bool{return true}\n"
            L"func bar(a : Float) -> Int {return 3}\n"
            L"let a = bar(3.4)");
    SymbolPtr a, b;
    TypePtr type;
    ASSERT_EQ(0, compilerResults.numResults());
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_EQ(global->Bool(), a->getType());
}

TEST(TestFunctionOverloads, testCovarianceOverload4)
{
    SEMANTIC_ANALYZE(
            L"func bar(a : Double)->Bool{return true}\n"
                    L"func bar(a : Float) -> Int {return 3}\n"
                    L"let a = bar(3)");
    SymbolPtr a, b;
    TypePtr type;
    ASSERT_EQ(1, compilerResults.numResults());

    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_AMBIGUOUS_USE_1, r.code);
    ASSERT_EQ(L"bar", r.items[0]);
}
TEST(TestFunctionOverloads, testMixedVariableOverload)
{

}
TEST(TestFunctionOverloads, testTrailingClosureOverload)
{

}
TEST(TestFunctionOverloads, testCurryOverload)
{

}

TEST(TestFunctionOverloads, testStructFuncOverload)
{

    SEMANTIC_ANALYZE(L"struct Test {"
            L"func a(a : Int) -> String {return \"\"}\n"
            L"func a(a : Bool) -> Float {return 3}\n"
            L"}\n"
            L"let t = Test()\n"
            L"let a = t.a(3)"
    );
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


    SymbolPtr t, a;
    ASSERT_NOT_NULL(t = scope->lookup(L"t"));
    TypePtr type = t->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Test;
    ASSERT_NOT_NULL(t_Test = std::dynamic_pointer_cast<Type>(scope->lookup(L"Test")));
    ASSERT_TRUE(type == t_Test);

    ASSERT_NOT_NULL(t_Test->getReference());
    ScopedStructPtr def;
    ASSERT_NOT_NULL(def = std::dynamic_pointer_cast<ScopedStruct>(t_Test->getReference()));


    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(type = a->getType());
    TypePtr t_String;
    ASSERT_NOT_NULL(t_String = std::dynamic_pointer_cast<Type>(symbolRegistry.lookupType(L"String")));
    ASSERT_TRUE(type == t_String);



}

TEST(TestFunctionOverloads, testClosureLiteral)
{
    SEMANTIC_ANALYZE(L"let a = {(c : Int, b : Int)->Int in return c + b}(1, 2)");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Int = symbolRegistry.lookupType(L"Int");
    ASSERT_TRUE(type == t_Int);


}

TEST(TestFunctionOverloads, CallVariable)
{
    SEMANTIC_ANALYZE(L"var a = {(a : Int) -> Bool in return true}\n"
            L"var b = a(3)");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());


    SymbolPtr b;
    ASSERT_NOT_NULL(b = scope->lookup(L"b"));
    TypePtr type = b->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
    ASSERT_TRUE(type == t_Bool);
}

TEST(TestFunctionOverloads, UnmatchedArgumentType)
{

    SEMANTIC_ANALYZE(L"var a = {(a : Int) -> Bool in return true}\n"
            L"var b = a(true)");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_CANNOT_CONVERT_EXPRESSION_TYPE_2, res.code);
}
