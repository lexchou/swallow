#ifndef TEST_TYPE_INFERENCE_H
#define TEST_TYPE_INFERENCE_H

#include "tests/semantics/semantic-test.h"
#include "semantics/type.h"
#include "swift_errors.h"

using namespace Swift;

class TestSymbolResolve : public SemanticTestCase
{
    CPPUNIT_TEST_SUITE(TestSymbolResolve);
    CPPUNIT_TEST(testUndeclaredVars);
    CPPUNIT_TEST(testUseVariableBeforeDeclaration);
    CPPUNIT_TEST(testVariableUsedWithinItsOwnInitialization);
    CPPUNIT_TEST(testDuplicatedVars);
    CPPUNIT_TEST_SUITE_END();
public:
    void testUndeclaredVars()
    {
        SEMANTIC_ANALYZE(L"a = 34");
        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());
        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_USE_OF_UNRESOLVED_IDENTIFIER, r.code);
        ASSERT_EQUALS(L"a", r.item);
    }

    void testUseVariableBeforeDeclaration()
    {
        SEMANTIC_ANALYZE(L"let a = b, b = 1;");
        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());

        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_USE_OF_UNINITIALIZED_VARIABLE, r.code);
        ASSERT_EQUALS(L"b", r.item);

    }

    void testVariableUsedWithinItsOwnInitialization()
    {
        SEMANTIC_ANALYZE(L"let a = a + 1;");
        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());
        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_USE_OF_INITIALIZING_VARIABLE, r.code);
        ASSERT_EQUALS(L"a", r.item);

    }

    void testDuplicatedVars()
    {
        SEMANTIC_ANALYZE(L"let fa = 3;var fa = 3")

        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());
        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_DEFINITION_CONFLICT, r.code);
        ASSERT_EQUALS(L"fa", r.item);
    }

    void testLocalGlobal()
    {

    }

};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestSymbolResolve, "alltest");



#endif//TEST_TYPE_INFERENCE_H
