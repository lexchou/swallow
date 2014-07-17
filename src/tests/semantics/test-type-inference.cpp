#ifndef TEST_TYPE_INFERENCE_H
#define TEST_TYPE_INFERENCE_H

#include "tests/semantics/semantic-test.h"


using namespace Swift;

class TestTypeInference : public SemanticTestCase
{
    CPPUNIT_TEST_SUITE(TestTypeInference);
    CPPUNIT_TEST(testLiteral);
    CPPUNIT_TEST_SUITE_END();
public:
    void testLiteral()
    {
        SEMANTIC_ANALYZE(L"let a = 34");
        Variable* a = dynamic_cast<Variable*>(scope->lookup(L"a"));

    }


};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestTypeInference, "alltest");



#endif//TEST_TYPE_INFERENCE_H
