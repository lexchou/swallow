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
    CPPUNIT_TEST(testLocalGlobal);
    CPPUNIT_TEST(testLocalTypeUsesUpperLocalSymbol);
    CPPUNIT_TEST(testClassVariable1);
    CPPUNIT_TEST(testClassVariable2);
    CPPUNIT_TEST(testClassVariable3);
    CPPUNIT_TEST_SUITE_END();
public:
    void testUndeclaredVars()
    {
        SEMANTIC_ANALYZE(L"a = 34");
        dumpCompilerResults(compilerResults);
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
        SEMANTIC_ANALYZE(L"let global = 0; func test(){let local = 1;}");
        FunctionDefPtr test = NULL;

        CPPUNIT_ASSERT(test = std::dynamic_pointer_cast<FunctionDef>(scope->lookup(L"test")));
        ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(test->getBody());
        SymbolPtr local = cb->getScope()->lookup(L"local");
        SymbolPtr global = root->getScope()->lookup(L"global");
        CPPUNIT_ASSERT(local != NULL);
        CPPUNIT_ASSERT(global != NULL);
    }
    void testLocalTypeUsesUpperLocalSymbol()
    {
        const wchar_t* code = L"func test() -> Int\n"
        L"{\n"
        L"    let b = 11\n"
        L"    struct TTT\n"
        L"    {\n"
        L"        let i = b\n"
        L"    }\n"
        L"    let i = TTT()\n"
        L"    return i.i;\n"
        L"\n"
        L"}\n";
        SEMANTIC_ANALYZE(code);
        CPPUNIT_ASSERT_EQUAL(1, compilerResults.numResults());
        const CompilerResult& r = compilerResults.getResult(0);
        CPPUNIT_ASSERT_EQUAL((int)Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, r.code);
        ASSERT_EQUALS(L"b", r.item);
    }
    void testClassVariable1()
    {
        const wchar_t* code =
                L"class Class\n"
                L"{\n"
                L"    var _val : Int = 0\n"
                L"    var val : Int = 0 {\n"
                L"        willSet(newVal) {\n"
                L"            _val = newVal;\n"
                L"        }\n"
                L"        didSet {\n"
                L"            println(oldValue);\n"
                L"        }\n"
                L"    }\n"
                L"}";
        SEMANTIC_ANALYZE(code);
        CPPUNIT_ASSERT_EQUAL(0, compilerResults.numResults());
    }

    void testClassVariable2()
    {
        const wchar_t* code =
                L"class Class\n"
        L"{\n"
        L"    var _val : Int = 0\n"
        L"    var val : Int = 0 {\n"
        L"        willSet {\n"
        L"            _val = newValue;\n"
        L"        }\n"
        L"        didSet(old) {\n"
        L"            println(old);\n"
        L"        }\n"
        L"    }\n"
        L"}";
        SEMANTIC_ANALYZE(code);
        CPPUNIT_ASSERT_EQUAL(0, compilerResults.numResults());
    }

    void testClassVariable3()
    {
        const wchar_t* code =
                L"class Class\n"
        L"{\n"
        L"    var _val : Int = 0\n"
        L"    var val : Int = 0 {\n"
        L"        get{\n"
        L"            return _val;\n"
        L"        }\n"
        L"        set {\n"
        L"            _val = newValue;\n"
        L"        }\n"
        L"    }\n"
        L"}";
        SEMANTIC_ANALYZE(code);
        CPPUNIT_ASSERT_EQUAL(0, compilerResults.numResults());
    }

};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestSymbolResolve, "alltest");



#endif//TEST_TYPE_INFERENCE_H
