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
    Node* parse(const wchar_t* str)
    {
        SymbolRegistry sregistry;
        NodeFactory nodeFactory;
        Parser parser(&nodeFactory, &sregistry);
        return parser.parse(str);
    }
    
    void testPostfixOperator()
    {
        Node* root = parse(L"i++ % 3");
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
        Node* root = parse(L"foo.bar(1,2)");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCall* call = dynamic_cast<FunctionCall*>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction() != NULL);
        MemberAccess* member = dynamic_cast<MemberAccess*>(call->getFunction());
        CPPUNIT_ASSERT(member != NULL);

        Identifier* id = dynamic_cast<Identifier*>(member->getSelf());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"foo", id->getIdentifier().c_str());
        id = dynamic_cast<Identifier*>(member->getField());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"bar", id->getIdentifier().c_str());
        
        
        
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