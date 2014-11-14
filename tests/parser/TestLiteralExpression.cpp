/* TestLiteralExpression.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "../utils.h"
#include "parser/Parser.h"
#include "semantics/SymbolRegistry.h"
#include "ast/ast.h"

using namespace Swallow;

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

TEST(TestLiteralExpression, StringInterpolation)
{
    PARSE_STATEMENT(L"\"\\(multiplier) times 2.5 is \\(Double(multiplier) * 2.5)\"");
    dumpCompilerResults(compilerResults);
    StringInterpolationPtr c = std::dynamic_pointer_cast<StringInterpolation>(root);
    ASSERT_NOT_NULL(c);
    //1th Expr: multipler
    //2nd Expr: " times 2.5 is "
    //3rd Expr: Double(multiplier) * 2.5
    ASSERT_EQ(3, c->numExpressions());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<Identifier>(c->getExpression(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<StringLiteral>(c->getExpression(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(c->getExpression(0)));

}
