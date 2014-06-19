#include "tokenizer/tokens.h"
#include "tokenizer/tokenizer.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include <cstdlib>

#define ASSERT_EQUALS(E, A) wcs_assertEquals((E), (A), __FILE__, __LINE__)
void wcs_assertEquals(const wchar_t* expected, const wchar_t* actual, const char* file, int line)
{
    if(!wcscmp(expected, actual))
        return;
    char expected2[1024];
    char actual2[1024];
    wcstombs(expected2, expected, sizeof(expected2));
    wcstombs(actual2, actual, sizeof(actual2));
    
    
    CPPUNIT_NS::Asserter::failNotEqual( expected2, actual2, CPPUNIT_NS::SourceLine(file, line), "");
}


class TestTokenizer : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestTokenizer);
    CPPUNIT_TEST(testExpression);
    CPPUNIT_TEST(testString);
    CPPUNIT_TEST(testComment);
    CPPUNIT_TEST(testKeywords);
    CPPUNIT_TEST(testCustomizedOperators);
    CPPUNIT_TEST_SUITE_END();
public:
    void testExpression()
    {
        Tokenizer tokenizer(L"println(i++)");
        Token token;
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kIdentifier);
        ASSERT_EQUALS(L"println", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kOpenParen);
        ASSERT_EQUALS(L"(", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kIdentifier);
        ASSERT_EQUALS(L"i", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kOperator);
        ASSERT_EQUALS(L"++", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kCloseParen);
        ASSERT_EQUALS(L")", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
    }
    
    void testString()
    {
        Tokenizer tokenizer(L"");
        Token token;
        
        //test empty string
        tokenizer.set(L"\"\"");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        ASSERT_EQUALS(L"", token.c_str());

        //test ordinal string
        tokenizer.set(L"\"1 2 3\"");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        ASSERT_EQUALS(L"1 2 3", token.c_str());
        
        //test escapes
        tokenizer.set(L"\"\\\\ \\t \\n \\r \\\" \\' \\0\"");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        ASSERT_EQUALS(L"\\ \t \n \r \" ' \0", token.c_str());
        
        //test \x
        tokenizer.set(L"\"\\x34\\xf3\\x40\"");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        ASSERT_EQUALS(L"\x34\xf3\x40", token.c_str());
        CPPUNIT_ASSERT(!tokenizer.next(token));
        //test \u
        tokenizer.set(L"\"\\u34f3\ufa40\"");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        ASSERT_EQUALS(L"\u34f3\ufa40", token.c_str());
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        
        
        //test expression
        tokenizer.set(L"\"1 2 \\(3) 4 \\(5) 6\"");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        CPPUNIT_ASSERT(token.string.expressionFollowed);
        ASSERT_EQUALS(L"1 2 ", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kInteger);
        ASSERT_EQUALS(L"3", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        CPPUNIT_ASSERT(token.string.expressionFollowed);
        ASSERT_EQUALS(L" 4 ", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kInteger);
        ASSERT_EQUALS(L"5", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        CPPUNIT_ASSERT(!token.string.expressionFollowed);
        ASSERT_EQUALS(L" 6", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        
        
        //TODO: test \U
        tokenizer.set(L"\"\\U34f3fa40\"");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(token.type, kString);
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        //TODO: test bad strings
        
    }
    
    void testComment()
    {
        Tokenizer tokenizer(NULL);
        Token token;
        
        tokenizer.set(L"a//hello\nb");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT(!token.comment.multiline);
        CPPUNIT_ASSERT_EQUAL(kComment, token.type);

        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());

        //test mutiline comment
        
        tokenizer.set(L"a/*hello*/\nb");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT(token.comment.multiline);
        CPPUNIT_ASSERT_EQUAL(0, token.comment.nestedLevels);
        ASSERT_EQUALS(L"hello", token.c_str());
        CPPUNIT_ASSERT_EQUAL(kComment, token.type);
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
        
        tokenizer.set(L"a/*he/*dd*/llo*/\nb");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT(token.comment.multiline);
        CPPUNIT_ASSERT_EQUAL(1, token.comment.nestedLevels);
        ASSERT_EQUALS(L"he/*dd*/llo", token.c_str());
        CPPUNIT_ASSERT_EQUAL(kComment, token.type);
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
    }
    
    void testKeywords()
    {
        
    }
    
    void testCustomizedOperators()
    {
        Tokenizer tokenizer(L"");
        Token token;
        
        tokenizer.set(L"a++.b");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kPostfixUnary, token.operators.type);
        ASSERT_EQUALS(L"++", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kInfixBinary, token.operators.type);
        ASSERT_EQUALS(L".", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        //
        tokenizer.set(L"a+b");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kInfixBinary, token.operators.type);
        ASSERT_EQUALS(L"+", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));

        
        //
        tokenizer.set(L"a ++b");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kPrefixUnary, token.operators.type);
        ASSERT_EQUALS(L"++", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        //
        tokenizer.set(L"a++ b");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kPostfixUnary, token.operators.type);
        ASSERT_EQUALS(L"++", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        //
        tokenizer.set(L"a!+b");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"a", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kPostfixUnary, token.operators.type);
        ASSERT_EQUALS(L"!", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOperator, token.type);
        CPPUNIT_ASSERT_EQUAL(kInfixBinary, token.operators.type);
        ASSERT_EQUALS(L"+", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"b", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        //
        tokenizer.set(L"true ? 1 : 0");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kIdentifier, token.type);
        ASSERT_EQUALS(L"true", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kOptional, token.type);
        ASSERT_EQUALS(L"?", token.c_str());

        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        ASSERT_EQUALS(L"1", token.c_str());

        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kColon, token.type);
        ASSERT_EQUALS(L":", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        ASSERT_EQUALS(L"0", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
        
    }
    
};
class TestNumberToken : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestNumberToken);
    CPPUNIT_TEST(testDecimal);
    CPPUNIT_TEST(testBinary);
    CPPUNIT_TEST(testHexadecimal);
    CPPUNIT_TEST(testOctal);
    CPPUNIT_TEST(testDecimal);
    CPPUNIT_TEST(testHexadecimalFloat);
    CPPUNIT_TEST_SUITE_END();
public:
    void testDecimal()
    {
        
        Tokenizer tokenizer(L"3 345 0345 34_333_444 +43 -03_4");
        Token token;
        
        //test decimal
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"3", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"345", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"0345", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"34_333_444", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        ASSERT_EQUALS(L"+43", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"-03_4", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));

    }
    void testBinary()
    {
        Token token;
        Tokenizer tokenizer(L"0b100_010_000 0b0 -0b1 +0b0");
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(2, token.number.base);
        ASSERT_EQUALS(L"0b100_010_000", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(2, token.number.base);
        ASSERT_EQUALS(L"0b0", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(2, token.number.base);
        ASSERT_EQUALS(L"-0b1", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(2, token.number.base);
        ASSERT_EQUALS(L"+0b0", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
        
    }
    void testHexadecimal()
    {
        
        Token token;
        Tokenizer tokenizer(L"0xffe 0xfep2 0xaa -0xacd +0xfe");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"0xffe", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"0xfep2", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"0xaa", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"-0xacd", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"+0xfe", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
    }
    void testOctal()
    {
        Token token;
        Tokenizer tokenizer(L"0o444_555_777 0o340 -0o176 +0o340");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(8, token.number.base);
        ASSERT_EQUALS(L"0o444_555_777", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(8, token.number.base);
        ASSERT_EQUALS(L"0o340", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(8, token.number.base);
        ASSERT_EQUALS(L"-0o176", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(8, token.number.base);
        ASSERT_EQUALS(L"+0o340", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
    }
    void testDecimalFloat()
    {
        Token token;
        Tokenizer tokenizer(L"345 345.543 -345.6e78 +3e4");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"345", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kFloat, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"345.543", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kFloat, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"-345.6e78", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(10, token.number.base);
        ASSERT_EQUALS(L"+3e4", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
    }
    
    void testHexadecimalFloat()
    {
        Token token;
        Tokenizer tokenizer(L"0x345 0x345.543 -0x345.6p78 +0x3p4");
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"0x345", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kFloat, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"0x345.543", token.c_str());
        
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kFloat, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"-0x345.6p78", token.c_str());
        
        CPPUNIT_ASSERT(tokenizer.next(token));
        CPPUNIT_ASSERT_EQUAL(kInteger, token.type);
        CPPUNIT_ASSERT_EQUAL(16, token.number.base);
        ASSERT_EQUALS(L"+0x3p4", token.c_str());
        
        CPPUNIT_ASSERT(!tokenizer.next(token));
    }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestTokenizer, "alltest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestNumberToken, "alltest");
int main(int argc, char** argv)
{
    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &alltest = CppUnit::TestFactoryRegistry::getRegistry("alltest");
    runner.addTest(alltest.makeTest());
    runner.run();
}
