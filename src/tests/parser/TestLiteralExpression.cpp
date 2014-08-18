#include "tests/utils.h"
#include "parser/Parser.h"
#include "semantics/SymbolRegistry.h"
#include "ast/ast.h"

using namespace Swift;

TEST(TestLiteralExpression, testLiteral)
{

    PARSE_STATEMENT(L"3");
    ASSERT_NOT_NULL(root);
    IntegerLiteralPtr n = std::dynamic_pointer_cast<IntegerLiteral>(root);
    ASSERT_NOT_NULL(n);
    ASSERT_EQ(L"3", n->valueAsString);

}
TEST(TestLiteralExpression, testLiteral2)
{
    PARSE_STATEMENT(L"3.45e4");
    ASSERT_NOT_NULL(root);
    FloatLiteralPtr f = std::dynamic_pointer_cast<FloatLiteral>(root);
    ASSERT_NOT_NULL(f);
    ASSERT_EQ(L"3.45e4", f->valueAsString);

}

TEST(TestLiteralExpression, testLiteral3)
{
    PARSE_STATEMENT(L"\"str\\thello\"");
    ASSERT_NOT_NULL(root);
    StringLiteralPtr str = std::dynamic_pointer_cast<StringLiteral>(root);
    ASSERT_NOT_NULL(str);
    ASSERT_EQ(L"str\thello", str->toString());


}
TEST(TestLiteralExpression, testArrayLiteral)
{
    PARSE_STATEMENT(L"[]");
    ASSERT_NOT_NULL(root);
    ArrayLiteralPtr a = std::dynamic_pointer_cast<ArrayLiteral>(root);
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(0, a->numElements());


}

TEST(TestLiteralExpression, testArrayLiteral2)
{
    PARSE_STATEMENT(L"[5]");
    ASSERT_NOT_NULL(root);
    ArrayLiteralPtr a = std::dynamic_pointer_cast<ArrayLiteral>(root);
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(1, a->numElements());
    IntegerLiteralPtr i = std::dynamic_pointer_cast<IntegerLiteral>(a->getElement(0));
    ASSERT_NOT_NULL(i);
    ASSERT_EQ(L"5", i->valueAsString);


}
TEST(TestLiteralExpression, testArrayLiteral3)
{
    PARSE_STATEMENT(L"[\"a\",]");
    ASSERT_NOT_NULL(root);
    ArrayLiteralPtr a = std::dynamic_pointer_cast<ArrayLiteral>(root);
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(1, a->numElements());
    StringLiteralPtr s = std::dynamic_pointer_cast<StringLiteral>(a->getElement(0));
    ASSERT_NOT_NULL(s);
    ASSERT_EQ(L"a", s->value);

}
TEST(TestLiteralExpression, testArrayLiteral4)
{
    PARSE_STATEMENT(L"[5,6]");
    ASSERT_NOT_NULL(root);
    ArrayLiteralPtr a = std::dynamic_pointer_cast<ArrayLiteral>(root);
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(2, a->numElements());

}
TEST(TestLiteralExpression, testArrayLiteral5)
{
    PARSE_STATEMENT(L"[5,6, [5]]");
    ASSERT_NOT_NULL(root);
    ArrayLiteralPtr a = std::dynamic_pointer_cast<ArrayLiteral>(root);
    ASSERT_NOT_NULL(a);
    ASSERT_EQ(3, a->numElements());


}
TEST(TestLiteralExpression, testDictionaryLiteral)
{
    PARSE_STATEMENT(L"[:]");
    ASSERT_NOT_NULL(root);
    DictionaryLiteralPtr d = std::dynamic_pointer_cast<DictionaryLiteral>(root);
    ASSERT_NOT_NULL(d);
    ASSERT_EQ(0, d->numElements());

}
TEST(TestLiteralExpression, testDictionaryLiteral2)
{
    PARSE_STATEMENT(L"[5 : 1]");
    ASSERT_NOT_NULL(root);
    DictionaryLiteralPtr d = std::dynamic_pointer_cast<DictionaryLiteral>(root);
    ASSERT_NOT_NULL(d);
    ASSERT_EQ(1, d->numElements());

}
TEST(TestLiteralExpression, testCompileConstants)
{
    PARSE_STATEMENT(L"__FILE__");
    ASSERT_NOT_NULL(root);
    CompileConstantPtr c = std::dynamic_pointer_cast<CompileConstant>(root);
    ASSERT_NOT_NULL(c);
    ASSERT_EQ(L"<file>", c->getValue());

}
TEST(TestLiteralExpression, testCompileConstants2)
{
    PARSE_STATEMENT(L"__LINE__");
    ASSERT_NOT_NULL(root);
    CompileConstantPtr c = std::dynamic_pointer_cast<CompileConstant>(root);
    ASSERT_NOT_NULL(c);
    ASSERT_EQ(L"1", c->getValue());


}
TEST(TestLiteralExpression, testCompileConstants3)
{
    PARSE_STATEMENT(L"__COLUMN__");
    ASSERT_NOT_NULL(root);
    CompileConstantPtr c = std::dynamic_pointer_cast<CompileConstant>(root);
    ASSERT_NOT_NULL(c);
    ASSERT_EQ(L"1", c->getValue());


}
TEST(TestLiteralExpression, testCompileConstants4)
{
    PARSE_STATEMENT(L"__FUNCTION__");
    ASSERT_NOT_NULL(root);
    CompileConstantPtr c = std::dynamic_pointer_cast<CompileConstant>(root);
    ASSERT_NOT_NULL(c);
    ASSERT_EQ(L"<top>", c->getValue());

}
