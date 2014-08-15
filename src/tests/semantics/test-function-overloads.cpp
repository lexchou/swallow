#include "tests/utils.h"
#include "semantics/symbol-registry.h"
#include "semantics/symbol.h"
#include "semantics/scoped-nodes.h"
#include "semantics/type.h"
#include "swift_errors.h"


using namespace Swift;


TEST(TestFunctionOverloads, testFunc)
{
    SEMANTIC_ANALYZE(L"func test() -> String {}\n"
        L"let a = test()");
    SymbolPtr a;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    TypePtr type = a->getType();
    ASSERT_NOT_NULL(type);
    TypePtr t_String = symbolRegistry.lookupType(L"String");
    ASSERT_TRUE(type == t_String);
}

TEST(TestFunctionOverloads, testOverloadedFunc)
{
    SEMANTIC_ANALYZE(L"func test(a : Int) -> String {}\n"
            L"func test(a : String) -> Bool {}\n"
            L"let a = test(56), b = test(\"str\")");
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
    SEMANTIC_ANALYZE(L"func test(a : Int) -> String {}\n"
            L"func test(# a : Int) -> Bool {}\n"
            L"func test(ExternalName a : String) -> Double {}\n"
            L"let a = test(56), b = test(a : 33), c = test(ExternalName : \"str\")");
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
    SEMANTIC_ANALYZE(L"func test(a : Int, b : Int) -> String {}\n"
    L"func test(a : Int...) -> Int {}\n"
    L"let a = test(0, 1)");

    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_AMBIGUOUS_USE, r.code);
    ASSERT_EQ(L"test", r.item);
}
TEST(TestFunctionOverloads, testVariadicParametersOverload2)
{
    SEMANTIC_ANALYZE(
            L"func foo(a : Int, b : Bool) -> Int {return 0;}\n"
            L"func foo(a : Int...) -> Bool{return false;}\n"
            L"let a = foo(0), b = foo(0, true), c = foo(0, 1), d = foo()\n")
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
            L"func bar(a : Double)->Bool{return true}\n"
            L"func bar(a : Float) -> Int {return 3}\n"
            L"let a = bar(3.4)");
    SymbolPtr a, b;
    TypePtr type;
    ASSERT_EQ(1, compilerResults.numResults());

    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_AMBIGUOUS_USE, r.code);
    ASSERT_EQ(L"bar", r.item);
}

TEST(TestFunctionOverloads, testCovarianceOverload3)
{
    SEMANTIC_ANALYZE(
            L"func bar(a : Double)->Bool{return true}\n"
                    L"func bar(a : Float) -> Int {return 3}\n"
                    L"let a = bar(3)");
    SymbolPtr a, b;
    TypePtr type;
    ASSERT_EQ(1, compilerResults.numResults());

    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_AMBIGUOUS_USE, r.code);
    ASSERT_EQ(L"bar", r.item);
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


