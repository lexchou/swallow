#include "tokenizer/Token.h"
#include "tokenizer/Tokenizer.h"
#include "tests/utils.h"
using namespace Swift;





TEST(TestTokenizer, testExpression)
{
    Tokenizer tokenizer(L"println(i++)");
    Token token;
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::Identifier);
    ASSERT_EQ(L"println", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::OpenParen);
    ASSERT_EQ(L"(", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::Identifier);
    ASSERT_EQ(L"i", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::Operator);
    ASSERT_EQ(L"++", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::CloseParen);
    ASSERT_EQ(L")", token.token);

    ASSERT_TRUE(!tokenizer.next(token));
}

TEST(TestTokenizer, testString)
{
    Tokenizer tokenizer(L"");
    Token token;

    //test empty string
    tokenizer.set(L"\"\"");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_EQ(L"", token.token);

    //test ordinal string
    tokenizer.set(L"\"1 2 3\"");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_EQ(L"1 2 3", token.token);

    //test escapes
    tokenizer.set(L"\"\\\\ \\t \\n \\r \\\" \\' \"");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_EQ(L"\\ \t \n \r \" ' ", token.token);

    //test \x
    tokenizer.set(L"\"\\x34\\xf3\\x40\"");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_EQ(L"\x34\xf3\x40", token.token);
    ASSERT_TRUE(!tokenizer.next(token));
    //test \u
    tokenizer.set(L"\"\\u34f3\ufa40\"");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_EQ(L"\u34f3\ufa40", token.token);
    ASSERT_TRUE(!tokenizer.next(token));



    //test expression
    tokenizer.set(L"\"1 2 \\(3) 4 \\(5) 6\"");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_TRUE(token.string.expressionFollowed);
    ASSERT_EQ(L"1 2 ", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::Integer);
    ASSERT_EQ(L"3", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_TRUE(token.string.expressionFollowed);
    ASSERT_EQ(L" 4 ", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::Integer);
    ASSERT_EQ(L"5", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_TRUE(!token.string.expressionFollowed);
    ASSERT_EQ(L" 6", token.token);

    ASSERT_TRUE(!tokenizer.next(token));



    //TODO: test \U
    tokenizer.set(L"\"\\U34f3fa40\"");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(token.type, TokenType::String);
    ASSERT_TRUE(!tokenizer.next(token));

    //TODO: test bad strings

}

TEST(TestTokenizer, testComment)
{
    Tokenizer tokenizer(NULL);
    Token token;

    tokenizer.set(L"a//hello\nb");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_TRUE(!token.comment.multiline);
    ASSERT_EQ(TokenType::Comment, token.type);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);

    //test mutiline comment

    tokenizer.set(L"a/*hello*/\nb");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_TRUE(token.comment.multiline);
    ASSERT_EQ(0, token.comment.nestedLevels);
    ASSERT_EQ(L"hello", token.token);
    ASSERT_EQ(TokenType::Comment, token.type);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);


    tokenizer.set(L"a/*he/*dd*/llo*/\nb");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_TRUE(token.comment.multiline);
    ASSERT_EQ(1, token.comment.nestedLevels);
    ASSERT_EQ(L"he/*dd*/llo", token.token);
    ASSERT_EQ(TokenType::Comment, token.type);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);

}

TEST(TestTokenizer, testKeywords)
{
    Tokenizer tokenizer(L"");
    Token token;

    tokenizer.set(L"for value in $1 `class`");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"for", token.token);
    ASSERT_TRUE(!token.identifier.backtick);
    ASSERT_TRUE(!token.identifier.implicitParameterName);
    ASSERT_EQ(KeywordType::Statement, token.identifier.type);
    ASSERT_EQ(Keyword::For, token.identifier.keyword);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"value", token.token);
    ASSERT_TRUE(!token.identifier.backtick);
    ASSERT_TRUE(!token.identifier.implicitParameterName);
    ASSERT_EQ(KeywordType::_, token.identifier.type);
    ASSERT_EQ(Keyword::_, token.identifier.keyword);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"in", token.token);
    ASSERT_TRUE(!token.identifier.backtick);
    ASSERT_TRUE(!token.identifier.implicitParameterName);
    ASSERT_EQ(KeywordType::Statement, token.identifier.type);
    ASSERT_EQ(Keyword::In, token.identifier.keyword);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"$1", token.token);
    ASSERT_TRUE(!token.identifier.backtick);
    ASSERT_TRUE(token.identifier.implicitParameterName);
    ASSERT_EQ(KeywordType::_, token.identifier.type);
    ASSERT_EQ(Keyword::_, token.identifier.keyword);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"class", token.token);
    ASSERT_TRUE(token.identifier.backtick);
    ASSERT_TRUE(!token.identifier.implicitParameterName);
    ASSERT_EQ(KeywordType::_, token.identifier.type);
    ASSERT_EQ(Keyword::_, token.identifier.keyword);

    ASSERT_TRUE(!tokenizer.next(token));

}

TEST(TestTokenizer, testCustomizedOperators)
{
    Tokenizer tokenizer(L"");
    Token token;

    tokenizer.set(L"a++.b");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::PostfixUnary, token.operators.type);
    ASSERT_EQ(L"++", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::InfixBinary, token.operators.type);
    ASSERT_EQ(L".", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

    //
    tokenizer.set(L"1+2");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(L"1", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::InfixBinary, token.operators.type);
    ASSERT_EQ(L"+", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(L"2", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

    //
    tokenizer.set(L"1 +2");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(L"1", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(L"+2", token.token);

    ASSERT_TRUE(!tokenizer.next(token));


    //
    tokenizer.set(L"a ++b");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::PrefixUnary, token.operators.type);
    ASSERT_EQ(L"++", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

    //
    tokenizer.set(L"a++ b");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::PostfixUnary, token.operators.type);
    ASSERT_EQ(L"++", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

    //
    tokenizer.set(L"a!+b");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"a", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::PostfixUnary, token.operators.type);
    ASSERT_EQ(L"!", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(OperatorType::InfixBinary, token.operators.type);
    ASSERT_EQ(L"+", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"b", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

    //
    tokenizer.set(L"true ? 1 : 0");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"true", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Optional, token.type);
    ASSERT_EQ(L"?", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(L"1", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Colon, token.type);
    ASSERT_EQ(L":", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(L"0", token.token);

    ASSERT_TRUE(!tokenizer.next(token));


}



TEST(TestNumberToken,  testExpression)
{
    Tokenizer tokenizer(L"self * 1_000.0");
    Token token;

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(L"self", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(L"*", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(L"1_000.0", token.token);


    ASSERT_TRUE(!tokenizer.next(token));
}

TEST(TestNumberToken, testNumberSelf)
{
    Tokenizer tokenizer(L"3.self");
    Token token;


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ(L"3", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Operator, token.type);
    ASSERT_EQ(L".", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Identifier, token.type);
    ASSERT_EQ(L"self", token.token);

}
TEST(TestNumberToken, testDecimal)
{

    Tokenizer tokenizer(L"3 345 0345 34_333_444 +43 -03_4");
    Token token;

    //test decimal
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((int64_t)3, token.number.value);
    ASSERT_EQ(L"3", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((int64_t)345, token.number.value);
    ASSERT_EQ(L"345", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((int64_t)345, token.number.value);
    ASSERT_EQ(L"0345", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((int64_t)34333444, token.number.value);
    ASSERT_EQ(L"34_333_444", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ((int64_t)43, token.number.value);
    ASSERT_EQ(L"+43", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((int64_t)-34, token.number.value);
    ASSERT_EQ(L"-03_4", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

}
TEST(TestNumberToken, testBinary)
{
    Token token;
    Tokenizer tokenizer(L"0b100_010_000 0b0 -0b1 +0b0");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(2, token.number.base);
    ASSERT_EQ((int64_t)272, token.number.value);
    ASSERT_EQ(L"0b100_010_000", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(2, token.number.base);
    ASSERT_EQ((int64_t)0, token.number.value);
    ASSERT_EQ(L"0b0", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(2, token.number.base);
    ASSERT_EQ((int64_t)-1, token.number.value);
    ASSERT_EQ(L"-0b1", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(2, token.number.base);
    ASSERT_EQ((int64_t)0, token.number.value);
    ASSERT_EQ(L"+0b0", token.token);

    ASSERT_TRUE(!tokenizer.next(token));

}
TEST(TestNumberToken, testHexadecimal)
{

    Token token;
    Tokenizer tokenizer(L"0xffe 0xfep2 0xaa -0xacd +0xfe");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ((int64_t)0xffe, token.number.value);
    ASSERT_EQ(L"0xffe", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ((double)1016, token.number.dvalue);
    ASSERT_EQ(L"0xfep2", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ((int64_t)0xaa, token.number.value);
    ASSERT_EQ(L"0xaa", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ((int64_t)-0xacd, token.number.value);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ(L"-0xacd", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ((int64_t)0xfe, token.number.value);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ(L"+0xfe", token.token);

    ASSERT_TRUE(!tokenizer.next(token));
}
TEST(TestNumberToken, testOctal)
{
    Token token;
    Tokenizer tokenizer(L"0o444_555_777 0o340 -0o176 +0o340");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(8, token.number.base);
    ASSERT_EQ((int64_t)76733439, token.number.value);
    ASSERT_EQ(L"0o444_555_777", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(8, token.number.base);
    ASSERT_EQ((int64_t)224, token.number.value);
    ASSERT_EQ(L"0o340", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(8, token.number.base);
    ASSERT_EQ((int64_t)-126, token.number.value);
    ASSERT_EQ(L"-0o176", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(8, token.number.base);
    ASSERT_EQ((int64_t)224, token.number.value);
    ASSERT_EQ(L"+0o340", token.token);

    ASSERT_TRUE(!tokenizer.next(token));
}
TEST(TestNumberToken, testDecimalFloat)
{
    Token token;
    Tokenizer tokenizer(L"1_000.0 345 345.543 -345.6e7 +3e4 ");

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((double)1000.0, token.number.dvalue);
    ASSERT_EQ(L"1_000.0", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Integer, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((double)345.0, token.number.dvalue);
    ASSERT_EQ(L"345", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((double)345.543, token.number.dvalue);
    ASSERT_EQ(L"345.543", token.token);


    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((double)-345.6e7, token.number.dvalue);
    ASSERT_EQ(L"-345.6e7", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(10, token.number.base);
    ASSERT_EQ((double)+3e4, token.number.dvalue);
    ASSERT_EQ(L"+3e4", token.token);

    ASSERT_TRUE(!tokenizer.next(token));
}

TEST(TestNumberToken, testHexadecimalFloat)
{
    Token token;
    Tokenizer tokenizer(L"0x345.6 -0x345.6p7 +0x3p4");
    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ((double)837.375, token.number.dvalue);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ(L"0x345.6", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ((double)-107184, token.number.dvalue);
    ASSERT_EQ(L"-0x345.6p7", token.token);

    ASSERT_TRUE(tokenizer.next(token));
    ASSERT_EQ(TokenType::Float, token.type);
    ASSERT_EQ(16, token.number.base);
    ASSERT_EQ((double)48, token.number.dvalue);
    ASSERT_EQ(L"+0x3p4", token.token);

    ASSERT_TRUE(!tokenizer.next(token));
}


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
