#ifndef TEST_CONSTANT_FOLDING_H
#define TEST_CONSTANT_FOLDING_H

#include "tests/utils.h"


using namespace Swift;

class TestConstantFolding : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestConstantFolding);
        CPPUNIT_TEST(testArithmetic);
        CPPUNIT_TEST(testLogic);
    CPPUNIT_TEST_SUITE_END();
public:
    void testLogic()
    {

    }
    void testArithmetic()
    {

    }
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestConstantFolding, "alltest");



#endif//TEST_CONSTANT_FOLDING_H
