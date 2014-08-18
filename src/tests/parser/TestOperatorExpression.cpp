#ifndef TEST_OPERATOR_EXPRESSION_H
#define TEST_OPERATOR_EXPRESSION_H

#include "tests/utils.h"

using namespace Swift;

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
    PARSE_STATEMENT(L"m = item as? Movie");


    AssignmentPtr eq;
    TypeCastPtr as;
    IdentifierPtr id;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(eq = std::dynamic_pointer_cast<Assignment>(root));
    ASSERT_NOT_NULL(as = std::dynamic_pointer_cast<TypeCast>(eq->getRHS()));
    ASSERT_EQ(L"as?", as->getOperator());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(as->getLHS()));
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(as->getDeclaredType()));

    ASSERT_EQ(L"item", id->getIdentifier());
    ASSERT_EQ(L"Movie", type->getName());

}

TEST(TestOperatorExpression, testTypeCast2)
{
    PARSE_STATEMENT(L"m = item as Movie");


    AssignmentPtr eq;
    TypeCastPtr as;
    IdentifierPtr id;
    TypeIdentifierPtr type;

    ASSERT_NOT_NULL(eq = std::dynamic_pointer_cast<Assignment>(root));
    ASSERT_NOT_NULL(as = std::dynamic_pointer_cast<TypeCast>(eq->getRHS()));
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



#endif
