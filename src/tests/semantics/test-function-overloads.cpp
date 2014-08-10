#ifndef TEST_TYPE_INFERENCE_H
#define TEST_TYPE_INFERENCE_H

#include "tests/semantics/semantic-test.h"
#include "semantics/type.h"
#include "swift_errors.h"


using namespace Swift;

class TestFunctionOverloads : public SemanticTestCase
{
    CPPUNIT_TEST_SUITE(TestFunctionOverloads);
    CPPUNIT_TEST(testFunc);
    CPPUNIT_TEST(testOverloadedFunc);
    CPPUNIT_TEST(testNamedParameterOverload);
    CPPUNIT_TEST(testVariadicParametersOverload);
    CPPUNIT_TEST(testVariadicParametersOverload2);
    CPPUNIT_TEST(testCovarianceOverload);
    CPPUNIT_TEST(testCovarianceOverload2);
    CPPUNIT_TEST(testCovarianceOverload3);
    CPPUNIT_TEST_SUITE_END();
public:

    void testFunc()
    {
        SEMANTIC_ANALYZE(L"func test() -> String {}\n"
            L"let a = test()");
        IdentifierPtr a;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_String = symbolRegistry.lookupType(L"String");
        CPPUNIT_ASSERT(type == t_String);
    }

    void testOverloadedFunc()
    {
        SEMANTIC_ANALYZE(L"func test(a : Int) -> String {}\n"
                L"func test(a : String) -> Bool {}\n"
                L"let a = test(56), b = test(\"str\")");
        IdentifierPtr a, b;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_String = symbolRegistry.lookupType(L"String");
        CPPUNIT_ASSERT(type == t_String);


        CPPUNIT_ASSERT(b = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"b")));
        type = b->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
        CPPUNIT_ASSERT(type == t_Bool);
    }
    void testNamedParameterOverload()
    {
        SEMANTIC_ANALYZE(L"func test(a : Int) -> String {}\n"
                L"func test(# a : Int) -> Bool {}\n"
                L"func test(ExternalName a : String) -> Double {}\n"
                L"let a = test(56), b = test(a : 33), c = test(ExternalName : \"str\")");
        IdentifierPtr a, b, c;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_String = symbolRegistry.lookupType(L"String");
        CPPUNIT_ASSERT(type == t_String);


        CPPUNIT_ASSERT(b = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"b")));
        type = b->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
        CPPUNIT_ASSERT(type == t_Bool);


        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"c")));
        type = c->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_Double = symbolRegistry.lookupType(L"Double");
        CPPUNIT_ASSERT(type == t_Double);
    }
    void testVariadicParametersOverload()
    {
        SEMANTIC_ANALYZE(L"func test(a : Int, b : Int) -> String {}\n"
        L"func test(a : Int...) -> Int {}\n"
        L"let a = test(0, 1)");

        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());
        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_AMBIGUOUS_USE, r.code);
        ASSERT_EQUALS(L"test", r.item);
    }
    void testVariadicParametersOverload2()
    {
        SEMANTIC_ANALYZE(
                L"func foo(a : Int, b : Bool) -> Int {return 0;}\n"
                L"func foo(a : Int...) -> Bool{return false;}\n"
                L"let a = foo(0), b = foo(0, true), c = foo(0, 1), d = foo()\n")
        IdentifierPtr a, b, c, d;
        TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
        TypePtr t_Int = symbolRegistry.lookupType(L"Int");

        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        CPPUNIT_ASSERT(type == t_Bool);


        CPPUNIT_ASSERT(b = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"b")));
        type = b->getType();
        CPPUNIT_ASSERT(type);
        CPPUNIT_ASSERT(type == t_Int);


        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"c")));
        type = c->getType();
        CPPUNIT_ASSERT(type);
        CPPUNIT_ASSERT(type == t_Bool);


        CPPUNIT_ASSERT(d = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"d")));
        CPPUNIT_ASSERT(type = d->getType());
        CPPUNIT_ASSERT(type == t_Bool);
    }
    void testCovarianceOverload()
    {
        SEMANTIC_ANALYZE(
                L"func bar(a : Int)->Bool{return true}\n"
                        L"func bar(a : Float) -> Int {return 3}\n"
                        L"let a = bar(3), b = bar(3.4)");
        IdentifierPtr a, b;
        TypePtr type;
        CPPUNIT_ASSERT_EQUAL(0, compilerResults.numResults());

        TypePtr t_Bool = symbolRegistry.lookupType(L"Bool");
        TypePtr t_Int = symbolRegistry.lookupType(L"Int");

        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        CPPUNIT_ASSERT(type = a->getType());
        CPPUNIT_ASSERT(type == t_Bool);

        CPPUNIT_ASSERT(b = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"b")));
        CPPUNIT_ASSERT(type = b->getType());
        CPPUNIT_ASSERT(type == t_Int);
    }

    void testCovarianceOverload2()
    {
        SEMANTIC_ANALYZE(
                L"func bar(a : Double)->Bool{return true}\n"
                L"func bar(a : Float) -> Int {return 3}\n"
                L"let a = bar(3.4)");
        IdentifierPtr a, b;
        TypePtr type;
        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());

        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_AMBIGUOUS_USE, r.code);
        ASSERT_EQUALS(L"bar", r.item);
    }

    void testCovarianceOverload3()
    {
        SEMANTIC_ANALYZE(
                L"func bar(a : Double)->Bool{return true}\n"
                        L"func bar(a : Float) -> Int {return 3}\n"
                        L"let a = bar(3)");
        IdentifierPtr a, b;
        TypePtr type;
        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());

        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_AMBIGUOUS_USE, r.code);
        ASSERT_EQUALS(L"bar", r.item);
    }
    void testMixedVariableOverload()
    {

    }
    void testTrailingClosureOverload()
    {

    }
    void testCurryOverload()
    {

    }

};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestFunctionOverloads, "alltest");



#endif//TEST_TYPE_INFERENCE_H
