#ifndef TEST_OPERATOR_EXPRESSION_H
#define TEST_OPERATOR_EXPRESSION_H

#include "tests/utils.h"

using namespace Swift;

class TestOperatorExpression : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestOperatorExpression);
    CPPUNIT_TEST(testTypeCheck);
    CPPUNIT_TEST(testTypeCast);
    CPPUNIT_TEST(testTypeCast2);
    CPPUNIT_TEST(testSubscriptThroughOptionalChaining);
    CPPUNIT_TEST(testExpr1);
    CPPUNIT_TEST(testMinus);
    CPPUNIT_TEST_SUITE_END();
public:
    
    
    void testTypeCheck()
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
        
        CPPUNIT_ASSERT(_if = std::dynamic_pointer_cast<IfStatement>(root));
        CPPUNIT_ASSERT(is = std::dynamic_pointer_cast<TypeCheck>(_if->getCondition()));
        ASSERT_EQUALS(L"is", is->getOperator().c_str());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(is->getLHS()));
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(is->getDeclaredType()));
        
        ASSERT_EQUALS(L"item", id->getIdentifier());
        ASSERT_EQUALS(L"Movie", type->getName());
        
        
        
    }
    
    void testTypeCast()
    {
        PARSE_STATEMENT(L"m = item as? Movie");
        
        
        AssignmentPtr eq;
        TypeCastPtr as;
        IdentifierPtr id;
        TypeIdentifierPtr type;
        
        CPPUNIT_ASSERT(eq = std::dynamic_pointer_cast<Assignment>(root));
        CPPUNIT_ASSERT(as = std::dynamic_pointer_cast<TypeCast>(eq->getRHS()));
        ASSERT_EQUALS(L"as?", as->getOperator().c_str());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(as->getLHS()));
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(as->getDeclaredType()));
        
        ASSERT_EQUALS(L"item", id->getIdentifier());
        ASSERT_EQUALS(L"Movie", type->getName());
        
    }
    
    void testTypeCast2()
    {
        PARSE_STATEMENT(L"m = item as Movie");
        
        
        AssignmentPtr eq;
        TypeCastPtr as;
        IdentifierPtr id;
        TypeIdentifierPtr type;
        
        CPPUNIT_ASSERT(eq = std::dynamic_pointer_cast<Assignment>(root));
        CPPUNIT_ASSERT(as = std::dynamic_pointer_cast<TypeCast>(eq->getRHS()));
        ASSERT_EQUALS(L"as", as->getOperator().c_str());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(as->getLHS()));
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(as->getDeclaredType()));
        
        ASSERT_EQUALS(L"item", id->getIdentifier());
        ASSERT_EQUALS(L"Movie", type->getName());
        
    }
    
    void testSubscriptThroughOptionalChaining()
    {
        PARSE_STATEMENT(L"firstRoomName = john.residence?[0].name");
        
    }
    
    void testExpr1()
    {
        /*
        PARSE_STATEMENT(L"a=4+1*(3+1*4)");
        CPPUNIT_ASSERT(root != NULL);
        AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
        
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT_EQUAL(2, eq->numChildren());
        
        IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(eq->getLHS());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        
        BinaryOperatorPtr add = std::dynamic_pointer_cast<BinaryOperator>(eq->getRHS());
        CPPUNIT_ASSERT(add != NULL);
        ASSERT_EQUALS(L"+", add->getOperator().c_str());
        
        
        BinaryOperatorPtr mul = std::dynamic_pointer_cast<BinaryOperator>(add->getRHS());
        CPPUNIT_ASSERT(mul != NULL);
        ASSERT_EQUALS(L"*", mul->getOperator().c_str());
        
        ParenthesizedExpressionPtr p = std::dynamic_pointer_cast<ParenthesizedExpression>(mul->getRHS());
        CPPUNIT_ASSERT(p != NULL);
        CPPUNIT_ASSERT_EQUAL(1, p->numExpressions());
        
        add = std::dynamic_pointer_cast<BinaryOperator>(p->get(0));
        CPPUNIT_ASSERT(add != NULL);
        ASSERT_EQUALS(L"+", add->getOperator().c_str());
        
        mul = std::dynamic_pointer_cast<BinaryOperator>(add->getRHS());
        CPPUNIT_ASSERT(mul != NULL);
        ASSERT_EQUALS(L"*", mul->getOperator().c_str());
        */
        
    }
    void testMinus()
    {
        PARSE_STATEMENT(L"-y");
        
    }
    void testParenthesizedExpr()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestOperatorExpression, "alltest");



#endif
