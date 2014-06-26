#ifndef TEST_OPERATOR_EXPRESSION_H
#define TEST_OPERATOR_EXPRESSION_H

#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"

using namespace Swift;

class TestOperatorExpression : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestOperatorExpression);
    CPPUNIT_TEST(testExpr1);
    CPPUNIT_TEST_SUITE_END();
public:
    void testExpr1()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        Node* root = parser.parse(L"a=4+1*(3+1*4)");
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
        CPPUNIT_ASSERT_EQUAL(1, p->numChildren());
        
        add = dynamic_cast<BinaryOperator*>(p->get(0));
        CPPUNIT_ASSERT(add != NULL);
        ASSERT_EQUALS(L"+", add->getOperator().c_str());
        
        mul = dynamic_cast<BinaryOperator*>(add->getRHS());
        CPPUNIT_ASSERT(mul != NULL);
        ASSERT_EQUALS(L"*", mul->getOperator().c_str());
        
        
        delete root;
        CPPUNIT_ASSERT_EQUAL(0, Node::NodeCount);
        
    }
    void testParenthesizedExpr()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestOperatorExpression, "alltest");



#endif