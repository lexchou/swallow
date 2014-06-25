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
    CPPUNIT_TEST(testFor2);
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
    
    Node* parse(const wchar_t* str)
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        return parser.parse(str);
    }
    
    void testFor()
    {
        Node* root = parse(L"for i = 0;i < 10; i++{a = i * 2;println(a);}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoop* loop = dynamic_cast<ForLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(1, loop->numInit());
        Assignment* eq = dynamic_cast<Assignment*>(loop->getInit(0));
        CPPUNIT_ASSERT(eq != NULL);
        
        BinaryOperator* le = dynamic_cast<BinaryOperator*>(loop->getCondition());
        ASSERT_EQUALS(L"<", le->getOperator().c_str());
        CPPUNIT_ASSERT(le != NULL);
        
        UnaryOperator* step = dynamic_cast<UnaryOperator*>(loop->getStep());
        CPPUNIT_ASSERT(step != NULL);
        ASSERT_EQUALS(L"++", step->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(OperatorType::PostfixUnary, step->getType());
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(2, body->numStatements());
        CPPUNIT_ASSERT(eq = dynamic_cast<Assignment*>(body->getStatement(0)));
        FunctionCall* call;
        CPPUNIT_ASSERT(call = dynamic_cast<FunctionCall*>(body->getStatement(1)));
        Identifier* func;
        CPPUNIT_ASSERT(func = dynamic_cast<Identifier*>(call->getFunction()));
        ASSERT_EQUALS(L"println", func->getIdentifier().c_str());
    }
    
    void testFor2()
    {
        Node* root = parse(L"for ;; {}");
        CPPUNIT_ASSERT(root != NULL);
        ForLoop* loop = dynamic_cast<ForLoop*>(root);
        CPPUNIT_ASSERT(loop != NULL);
        
        CPPUNIT_ASSERT_EQUAL(0, loop->numInit());
        
        CPPUNIT_ASSERT(loop->getCondition() == NULL);
        CPPUNIT_ASSERT(loop->getStep() == NULL);
        
        
        CodeBlock* body = loop->getCodeBlock();
        
        CPPUNIT_ASSERT_EQUAL(0, body->numStatements());
        
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