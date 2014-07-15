#ifndef TEST_TYPE_INFERENCE_H
#define TEST_TYPE_INFERENCE_H

#include "tests/utils.h"


using namespace Swift;

class TestTypeInference : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestTypeInference);
    CPPUNIT_TEST(testLiteral);
    CPPUNIT_TEST_SUITE_END();
public:
    void testLiteral()
    {
        SEMANTIC_ANALYZE(L"let a = 34");

    }


};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestTypeInference, "alltest");



#endif//TEST_TYPE_INFERENCE_H
