#ifndef TEST_STATEMENT_H
#define TEST_STATEMENT_H

#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"

using namespace Swift;

class TestStatement : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestStatement);
    CPPUNIT_TEST(testFor);
    CPPUNIT_TEST(testForIn);
    CPPUNIT_TEST(testWhile);
    CPPUNIT_TEST(testDo);
    CPPUNIT_TEST(testIf);
    CPPUNIT_TEST(testSwitch);
    CPPUNIT_TEST(testLabeledWhile);
    CPPUNIT_TEST(testLabeledDo);
    CPPUNIT_TEST(testLabeledFor);
    CPPUNIT_TEST_SUITE_END();
public:
    void testFor()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        Node* root = parser.parse(L"a=4+1*(3+1*4)");
        
        
        
    }
    void testForIn()
    {
        
    }
    void testWhile()
    {
        
    }
    void testDo()
    {
        
    }
    void testIf()
    {
        
    }
    void testSwitch()
    {
        
    }
    void testLabeledWhile()
    {
        
    }
    void testLabeledDo()
    {
        
    }
    void testLabeledFor()
    {
        
    }
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestStatement, "alltest");



#endif