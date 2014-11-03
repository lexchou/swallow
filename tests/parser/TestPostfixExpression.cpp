/* TestPostfixExpression.cpp --
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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


using namespace Swift;

    
TEST(TestPostfixExpression, testPostfixOperator)
{
    PARSE_STATEMENT(L"i++ % 3");
    ASSERT_NOT_NULL(root);
    BinaryOperatorPtr mod = std::dynamic_pointer_cast<BinaryOperator>(root);
    ASSERT_NOT_NULL(mod);
    ASSERT_EQ(L"%", mod->getOperator());
    ASSERT_NOT_NULL(mod->getLHS());
    UnaryOperatorPtr inc = std::dynamic_pointer_cast<UnaryOperator>(mod->getLHS());
    ASSERT_NOT_NULL(inc);
    ASSERT_EQ(L"++", inc->getOperator());
    ASSERT_EQ(OperatorType::PostfixUnary, inc->getOperatorType());

}


TEST(TestPostfixExpression, testFunctionCall)
{
    PARSE_STATEMENT(L"foo.bar(1, 2 + 3, id : 5)");
    ASSERT_NOT_NULL(root);
    FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(root);
    ASSERT_NOT_NULL(call);
    ASSERT_NOT_NULL(call->getFunction());
    MemberAccessPtr member = std::dynamic_pointer_cast<MemberAccess>(call->getFunction());
    ASSERT_NOT_NULL(member);

    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(member->getSelf());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"foo", id->getIdentifier());
    id = std::dynamic_pointer_cast<Identifier>(member->getField());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"bar", id->getIdentifier());

    ParenthesizedExpressionPtr args = call->getArguments();
    ASSERT_NOT_NULL(args);
    ASSERT_EQ(3, args->numExpressions());

    NodePtr n = args->get(0);
    ASSERT_NOT_NULL(n);
    IntegerLiteralPtr i = std::dynamic_pointer_cast<IntegerLiteral>(n);
    ASSERT_NOT_NULL(i);
    ASSERT_EQ(L"1", i->valueAsString);

    n = args->get(1);
    ASSERT_NOT_NULL(n);
    BinaryOperatorPtr op = std::dynamic_pointer_cast<BinaryOperator>(n);
    ASSERT_NOT_NULL(op);
    ASSERT_EQ(L"+", op->getOperator());

    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(op->getLHS()));
    ASSERT_EQ(L"2", i->valueAsString);


    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(op->getRHS()));
    ASSERT_EQ(L"3", i->valueAsString);



    std::wstring name = args->getName(2);
    ASSERT_EQ(L"id", name);

    n = args->get(2);
    ASSERT_NOT_NULL(n);
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(n));
    ASSERT_EQ(L"5", i->valueAsString);



}

TEST(TestPostfixExpression, testInit)
{

    PARSE_STATEMENT(L"Shape.init(id : 5)");
    ASSERT_NOT_NULL(root);
    FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(root);
    ASSERT_NOT_NULL(call);
    ASSERT_NOT_NULL(call->getFunction());
    InitializerReferencePtr init = std::dynamic_pointer_cast<InitializerReference>(call->getFunction());
    ASSERT_NOT_NULL(init);

    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(init->getExpression());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"Shape", id->getIdentifier());

    ParenthesizedExpressionPtr args = call->getArguments();
    ASSERT_NOT_NULL(args);
    ASSERT_EQ(1, args->numExpressions());

    NodePtr n = args->get(0);
    ASSERT_NOT_NULL(n);
    IntegerLiteralPtr i = std::dynamic_pointer_cast<IntegerLiteral>(n);
    ASSERT_NOT_NULL(i);
    ASSERT_EQ(L"5", i->valueAsString);

    std::wstring name = args->getName(0);
    ASSERT_EQ(L"id", name);

}

TEST(TestPostfixExpression, testSelf)
{
    PARSE_STATEMENT(L"Shape.self");
    ASSERT_NOT_NULL(root);
    SelfExpressionPtr self = std::dynamic_pointer_cast<SelfExpression>(root);
    ASSERT_NOT_NULL(self);
    ASSERT_NOT_NULL(self->getExpression());
    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(self->getExpression());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"Shape", id->getIdentifier());

}

TEST(TestPostfixExpression, testDynamicType)
{
    PARSE_STATEMENT(L"345.dynamicType");
    ASSERT_NOT_NULL(root);
    DynamicTypePtr dyn = std::dynamic_pointer_cast<DynamicType>(root);
    ASSERT_NOT_NULL(dyn);
    ASSERT_NOT_NULL(dyn->getExpression());
    IntegerLiteralPtr id = std::dynamic_pointer_cast<IntegerLiteral>(dyn->getExpression());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"345", id->valueAsString);

}


TEST(TestPostfixExpression, testSubscript)
{
    PARSE_STATEMENT(L"matrix[0, 1] = 2");
    ASSERT_NOT_NULL(root);
    AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
    ASSERT_NOT_NULL(eq);
    ASSERT_NOT_NULL(eq->getLHS());
    SubscriptAccessPtr sub = std::dynamic_pointer_cast<SubscriptAccess>(eq->getLHS());
    ASSERT_NOT_NULL(eq);
    ParenthesizedExpressionPtr index = sub->getIndex();
    ASSERT_EQ(2, index->numExpressions());
    IntegerLiteralPtr i;

    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(index->get(0)));
    ASSERT_EQ(L"0", i->valueAsString);
    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(index->get(1)));
    ASSERT_EQ(L"1", i->valueAsString);
}
TEST(TestPostfixExpression, testSubscript2)
{
    PARSE_STATEMENT(L"matrix[0, ] = 2");
    ASSERT_NOT_NULL(root);
    AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
    ASSERT_NOT_NULL(eq);
    ASSERT_NOT_NULL(eq->getLHS());
    SubscriptAccessPtr sub = std::dynamic_pointer_cast<SubscriptAccess>(eq->getLHS());
    ASSERT_NOT_NULL(eq);

    ParenthesizedExpressionPtr index = sub->getIndex();
    ASSERT_EQ(1, index->numExpressions());
    IntegerLiteralPtr i;

    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(index->get(0)));
    ASSERT_EQ(L"0", i->valueAsString);

}
TEST(TestPostfixExpression, testSubscript3)
{
    PARSE_STATEMENT(L"matrix[0 ] = 2");
    ASSERT_NOT_NULL(root);
    AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
    ASSERT_NOT_NULL(eq);
    ASSERT_NOT_NULL(eq->getLHS());
    SubscriptAccessPtr sub = std::dynamic_pointer_cast<SubscriptAccess>(eq->getLHS());
    ASSERT_NOT_NULL(eq);
    ParenthesizedExpressionPtr index = sub->getIndex();
    ASSERT_EQ(1, index->numExpressions());
    IntegerLiteralPtr i;

    ASSERT_NOT_NULL(i = std::dynamic_pointer_cast<IntegerLiteral>(index->get(0)));
    ASSERT_EQ(L"0", i->valueAsString);


}
TEST(TestPostfixExpression, testForcedValue)
{
    PARSE_STATEMENT(L"val!");
    ASSERT_NOT_NULL(root);
    ForcedValuePtr val = std::dynamic_pointer_cast<ForcedValue>(root);
    ASSERT_NOT_NULL(val);
    ASSERT_NOT_NULL(val->getExpression());
    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(val->getExpression());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"val", id->getIdentifier());

}

TEST(TestPostfixExpression, testOptionalChaining)
{
    PARSE_STATEMENT(L"c?.performAction()");
    ASSERT_NOT_NULL(root);
    FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(root);
    ASSERT_NOT_NULL(call);
    ASSERT_NOT_NULL(call->getFunction());
    MemberAccessPtr mem = std::dynamic_pointer_cast<MemberAccess>(call->getFunction());
    ASSERT_NOT_NULL(mem);
    OptionalChainingPtr optional = std::dynamic_pointer_cast<OptionalChaining>(mem->getSelf());
    ASSERT_NOT_NULL(optional);
    IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(optional->getExpression());
    ASSERT_NOT_NULL(id);
    ASSERT_EQ(L"c", id->getIdentifier());


}

