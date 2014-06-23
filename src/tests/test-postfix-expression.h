#include "utils.h"
#include "parser/parser.h"
#include "parser/symbol-registry.h"
#include "ast/ast.h"
#include "ast/node-factory.h"


using namespace Swift;

class TestPostfixExpression : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestPostfixExpression);
    CPPUNIT_TEST(testPostfixOperator);
    CPPUNIT_TEST(testFunctionCall);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testSelf);
    CPPUNIT_TEST(testDynamicType);
    CPPUNIT_TEST(testExplicitMember);
    CPPUNIT_TEST(testSubscript);
    CPPUNIT_TEST(testForcedValue);
    CPPUNIT_TEST(testOptionalChaining);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testPostfixOperator()
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        parser.setFileName(L"<file>");
        
        Node* root = parser.parse(L"i++ % 3");
        CPPUNIT_ASSERT(root != NULL);
        BinaryOperator* mod = dynamic_cast<BinaryOperator*>(root);
        CPPUNIT_ASSERT(mod != NULL);
        ASSERT_EQUALS(L"%", mod->getOperator().c_str());
        CPPUNIT_ASSERT(mod->getLHS() != NULL);
        UnaryOperator* inc = dynamic_cast<UnaryOperator*>(mod->getLHS());
        CPPUNIT_ASSERT(inc != NULL);
        ASSERT_EQUALS(L"++", inc->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(OperatorType::PostfixUnary, inc->getType());
        
        
    }
    
    
    void testFunctionCall()
    {
    }
    
    void testInit()
    {
        
    }
    
    void testSelf()
    {
        
    }
    
    void testDynamicType()
    {
        
    }
    
    void testExplicitMember()
    {
        
    }
    
    void testSubscript()
    {
        
    }
    
    void testForcedValue()
    {
        
    }
    
    void testOptionalChaining()
    {
        
    }
    
};
;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestPostfixExpression, "alltest");