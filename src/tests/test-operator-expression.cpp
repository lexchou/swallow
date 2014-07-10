#ifndef TEST_OPERATOR_EXPRESSION_H
#define TEST_OPERATOR_EXPRESSION_H

#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"

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
        
        IfStatement* _if = NULL;
        TypeCheck* is = NULL;
        Identifier* id = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(_if = dynamic_cast<IfStatement*>(root));
        CPPUNIT_ASSERT(is = dynamic_cast<TypeCheck*>(_if->getCondition()));
        ASSERT_EQUALS(L"is", is->getOperator().c_str());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(is->getLHS()));
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(is->getType()));
        
        ASSERT_EQUALS(L"item", id->getIdentifier());
        ASSERT_EQUALS(L"Movie", type->getName());
        
        
        
        DESTROY(root);
    }
    
    void testTypeCast()
    {
        PARSE_STATEMENT(L"m = item as? Movie");
        
        
        Assignment* eq = NULL;
        TypeCast* as = NULL;
        Identifier* id = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(eq = dynamic_cast<Assignment*>(root));
        CPPUNIT_ASSERT(as = dynamic_cast<TypeCast*>(eq->getRHS()));
        ASSERT_EQUALS(L"as?", as->getOperator().c_str());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(as->getLHS()));
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(as->getType()));
        
        ASSERT_EQUALS(L"item", id->getIdentifier());
        ASSERT_EQUALS(L"Movie", type->getName());
        
        DESTROY(root);
    }
    
    void testTypeCast2()
    {
        PARSE_STATEMENT(L"m = item as Movie");
        
        
        Assignment* eq = NULL;
        TypeCast* as = NULL;
        Identifier* id = NULL;
        TypeIdentifier* type = NULL;
        
        CPPUNIT_ASSERT(eq = dynamic_cast<Assignment*>(root));
        CPPUNIT_ASSERT(as = dynamic_cast<TypeCast*>(eq->getRHS()));
        ASSERT_EQUALS(L"as", as->getOperator().c_str());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(as->getLHS()));
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(as->getType()));
        
        ASSERT_EQUALS(L"item", id->getIdentifier());
        ASSERT_EQUALS(L"Movie", type->getName());
        
        DESTROY(root);
    }
    
    void testSubscriptThroughOptionalChaining()
    {
        PARSE_STATEMENT(L"firstRoomName = john.residence?[0].name");
        
        DESTROY(root);
    }
    
    void testExpr1()
    {
        PARSE_STATEMENT(L"a=4+1*(3+1*4)");
        CPPUNIT_ASSERT(root != NULL);
        Assignment* eq = dynamic_cast<Assignment*>(root);
        
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT_EQUAL(2, eq->numChildren());
        
        Identifier* id = dynamic_cast<Identifier*>(eq->getLHS());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"a", id->getIdentifier().c_str());
        
        BinaryOperator* add = dynamic_cast<BinaryOperator*>(eq->getRHS());
        CPPUNIT_ASSERT(add != NULL);
        ASSERT_EQUALS(L"+", add->getOperator().c_str());
        
        
        BinaryOperator* mul = dynamic_cast<BinaryOperator*>(add->getRHS());
        CPPUNIT_ASSERT(mul != NULL);
        ASSERT_EQUALS(L"*", mul->getOperator().c_str());
        
        ParenthesizedExpression* p = dynamic_cast<ParenthesizedExpression*>(mul->getRHS());
        CPPUNIT_ASSERT(p != NULL);
        CPPUNIT_ASSERT_EQUAL(1, p->numExpressions());
        
        add = dynamic_cast<BinaryOperator*>(p->get(0));
        CPPUNIT_ASSERT(add != NULL);
        ASSERT_EQUALS(L"+", add->getOperator().c_str());
        
        mul = dynamic_cast<BinaryOperator*>(add->getRHS());
        CPPUNIT_ASSERT(mul != NULL);
        ASSERT_EQUALS(L"*", mul->getOperator().c_str());
        
        
        DESTROY(root);
    }
    void testMinus()
    {
        PARSE_STATEMENT(L"-y");
        
        DESTROY(root);
    }
    void testParenthesizedExpr()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestOperatorExpression, "alltest");



#endif