#ifndef TEST_TYPE_INFERENCE_H
#define TEST_TYPE_INFERENCE_H

#include "tests/semantics/semantic-test.h"
#include "semantics/type.h"


using namespace Swift;

class TestTypeInference : public SemanticTestCase
{
    CPPUNIT_TEST_SUITE(TestTypeInference);
    //CPPUNIT_TEST(testIntLiteral);
    //CPPUNIT_TEST(testStringLiteral);
    //CPPUNIT_TEST(testTuple);
    //CPPUNIT_TEST(testExpression);
    //CPPUNIT_TEST(testClassInstance);
    CPPUNIT_TEST(testFunc);
    CPPUNIT_TEST(testOverloadedFunc);
    CPPUNIT_TEST_SUITE_END();
public:
    void testIntLiteral()
    {
        SEMANTIC_ANALYZE(L"let a = 34");
        IdentifierPtr a = NULL;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr Int = symbolRegistry.lookupType(L"Int32");
        CPPUNIT_ASSERT(type == Int);

    }
    void testStringLiteral()
    {
        SEMANTIC_ANALYZE(L"let a = \"345\"");
        IdentifierPtr a = NULL;

        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr String = symbolRegistry.lookupType(L"String");
        CPPUNIT_ASSERT(type == String);
    }
    void testTuple()
    {
        /*
        SEMANTIC_ANALYZE(L"let (a, b) : Int = (1, 0.3)");
        IdentifierPtr a = NULL;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr String = symbolRegistry.lookupType(L"Int");
        CPPUNIT_ASSERT(type == String);
        */
    }
    void testExpression()
    {
        SEMANTIC_ANALYZE(L"let a = 3 * 4 + 3");
        IdentifierPtr a;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_int = symbolRegistry.lookupType(L"Int32");
        CPPUNIT_ASSERT(type == t_int);
    }

    void testClassInstance()
    {
        SEMANTIC_ANALYZE(L"class Test {}\n"
        L"let a = Test()"
        );
        IdentifierPtr a;
        CPPUNIT_ASSERT(a = std::dynamic_pointer_cast<Identifier>(scope->lookup(L"a")));
        TypePtr type = a->getType();
        CPPUNIT_ASSERT(type);
        TypePtr t_Test;
        CPPUNIT_ASSERT(t_Test = std::dynamic_pointer_cast<Type>(scope->lookup(L"Test")));
        CPPUNIT_ASSERT(type == t_Test);
    }

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

};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestTypeInference, "alltest");



#endif//TEST_TYPE_INFERENCE_H
