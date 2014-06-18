#include "tokenizer/tokens.h"
#include "tokenizer/tokenizer.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>


class TestTokenizer : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTokenizer);
    CPPUNIT_TEST(testA);
    CPPUNIT_TEST(testString);
    CPPUNIT_TEST(testComment);
    CPPUNIT_TEST(testKeywords);
    CPPUNIT_TEST(testCustomizedOperators);
    CPPUNIT_TEST(testNumbers);
    CPPUNIT_TEST_SUITE_END();
public:
    void testA()
    {
        
        Tokenizer tokenizer(L"println(i++)");
        Token token;
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kIdentifier);
        CPPUNIT_ASSERT(!wcscmp(token.c_str(), L"println"));
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kOpenParen);
        CPPUNIT_ASSERT(!wcscmp(token.c_str(), L"("));
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kIdentifier);
        CPPUNIT_ASSERT(!wcscmp(token.c_str(), L"i"));
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kOperator);
        CPPUNIT_ASSERT(!wcscmp(token.c_str(), L"++"));
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kCloseParen);
        CPPUNIT_ASSERT(!wcscmp(token.c_str(), L")"));
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
    }
    
    void testString()
    {
        
    }
    
    void testComment()
    {
        
    }
    
    void testKeywords()
    {
        
    }
    
    void testCustomizedOperators()
    {
        
    }
    
    void testNumbers()
    {
        
    }
    
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestTokenizer, "alltest");
int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry("alltest");
    runner.addTest(registry.makeTest());
    runner.run();
}