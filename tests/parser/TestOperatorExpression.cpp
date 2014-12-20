/* TestOperatorExpression.cpp --
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

using namespace Swallow;

TEST(TestOperatorExpression, testTypeCheck)
{
    PARSE_STATEMENT(L"if item is Movie {\n"
                    L"++movieCount\n"
                    L"} else if item is Song {\n"
                    L"    ++songCount\n"
                    L"}");

    IfStatementPtr _if = NULL;
    TypeCheckPtr is;
    IdentifierPtr id;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(_if = std::dynamic_pointer_cast<IfStatement>(root));
    ASSERT_NOT_NULL(is = std::dynamic_pointer_cast<TypeCheck>(_if->getCondition()));
    ASSERT_EQ(L"is", is->getOperator());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(is->getLHS()));
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(is->getDeclaredType()));

    ASSERT_EQ(L"item", id->getIdentifier());
    ASSERT_EQ(L"Movie", type->getName());



}

TEST(TestOperatorExpression, testTypeCast)
{
    PARSE_STATEMENT(L"item as? Movie");


    TypeCastPtr as;
    IdentifierPtr id;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(as = std::dynamic_pointer_cast<TypeCast>(root));
    ASSERT_EQ(L"as?", as->getOperator());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(as->getLHS()));
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(as->getDeclaredType()));

    ASSERT_EQ(L"item", id->getIdentifier());
    ASSERT_EQ(L"Movie", type->getName());

}

TEST(TestOperatorExpression, testTypeCast2)
{
    PARSE_STATEMENT(L"item as Movie");


    TypeCastPtr as;
    IdentifierPtr id;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(as = std::dynamic_pointer_cast<TypeCast>(root));
    ASSERT_EQ(L"as", as->getOperator());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(as->getLHS()));
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(as->getDeclaredType()));

    ASSERT_EQ(L"item", id->getIdentifier());
    ASSERT_EQ(L"Movie", type->getName());

}

TEST(TestOperatorExpression, testSubscriptThroughOptionalChaining)
{
    PARSE_STATEMENT(L"firstRoomName = john.residence?[0].name");

}

TEST(TestOperatorExpression, testExpr1)
{
    /*
    PARSE_STATEMENT(L"a=4+1*(3+1*4)");
    ASSERT_NOT_NULL(root != NULL);
    AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);

    ASSERT_NOT_NULL(eq != NULL);
    ASSERT_EQ(2, eq->numChildren());

    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(eq->getLHS());
    ASSERT_NOT_NULL(id != NULL);
    ASSERT_EQ(L"a", id->getIdentifier());

    BinaryOperatorPtr add = std::dynamic_pointer_cast<BinaryOperator>(eq->getRHS());
    ASSERT_NOT_NULL(add != NULL);
    ASSERT_EQ(L"+", add->getOperator());


    BinaryOperatorPtr mul = std::dynamic_pointer_cast<BinaryOperator>(add->getRHS());
    ASSERT_NOT_NULL(mul != NULL);
    ASSERT_EQ(L"*", mul->getOperator());

    ParenthesizedExpressionPtr p = std::dynamic_pointer_cast<ParenthesizedExpression>(mul->getRHS());
    ASSERT_NOT_NULL(p != NULL);
    ASSERT_EQ(1, p->numExpressions());

    add = std::dynamic_pointer_cast<BinaryOperator>(p->get(0));
    ASSERT_NOT_NULL(add != NULL);
    ASSERT_EQ(L"+", add->getOperator());

    mul = std::dynamic_pointer_cast<BinaryOperator>(add->getRHS());
    ASSERT_NOT_NULL(mul != NULL);
    ASSERT_EQ(L"*", mul->getOperator());
    */

}
TEST(TestOperatorExpression, testMinus)
{
    PARSE_STATEMENT(L"-y");

}
TEST(TestOperatorExpression, testParenthesizedExpr)
{

}



