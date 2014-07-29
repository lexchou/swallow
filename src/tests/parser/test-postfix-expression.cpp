#include "tests/utils.h"


using namespace Swift;

class TestPostfixExpression : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestPostfixExpression);
    CPPUNIT_TEST(testPostfixOperator);
    CPPUNIT_TEST(testFunctionCall);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testSelf);
    CPPUNIT_TEST(testDynamicType);
    CPPUNIT_TEST(testSubscript);
    CPPUNIT_TEST(testSubscript2);
    CPPUNIT_TEST(testSubscript3);
    CPPUNIT_TEST(testForcedValue);
    CPPUNIT_TEST(testOptionalChaining);
    CPPUNIT_TEST_SUITE_END();
public:
    
    void testPostfixOperator()
    {
        PARSE_STATEMENT(L"i++ % 3");
        CPPUNIT_ASSERT(root != NULL);
        BinaryOperatorPtr mod = std::dynamic_pointer_cast<BinaryOperator>(root);
        CPPUNIT_ASSERT(mod != NULL);
        ASSERT_EQUALS(L"%", mod->getOperator().c_str());
        CPPUNIT_ASSERT(mod->getLHS() != NULL);
        UnaryOperatorPtr inc = std::dynamic_pointer_cast<UnaryOperator>(mod->getLHS());
        CPPUNIT_ASSERT(inc != NULL);
        ASSERT_EQUALS(L"++", inc->getOperator().c_str());
        CPPUNIT_ASSERT_EQUAL(OperatorType::PostfixUnary, inc->getType());
        
    }
    
    
    void testFunctionCall()
    {
        PARSE_STATEMENT(L"foo.bar(1, 2 + 3, id : 5)");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction() != NULL);
        MemberAccessPtr member = std::dynamic_pointer_cast<MemberAccess>(call->getFunction());
        CPPUNIT_ASSERT(member != NULL);

        IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(member->getSelf());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"foo", id->getIdentifier().c_str());
        id = std::dynamic_pointer_cast<Identifier>(member->getField());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"bar", id->getIdentifier().c_str());
        
        ParenthesizedExpressionPtr args = call->getArguments();
        CPPUNIT_ASSERT(args != NULL);
        CPPUNIT_ASSERT_EQUAL(3, args->numExpressions());
        
        NodePtr n = args->get(0);
        CPPUNIT_ASSERT(n != NULL);
        IntegerLiteralPtr i = std::dynamic_pointer_cast<IntegerLiteral>(n);
        CPPUNIT_ASSERT(i != NULL);
        ASSERT_EQUALS(L"1", i->valueAsString);
        
        n = args->get(1);
        CPPUNIT_ASSERT(n != NULL);
        BinaryOperatorPtr op = std::dynamic_pointer_cast<BinaryOperator>(n);
        CPPUNIT_ASSERT(op != NULL);
        ASSERT_EQUALS(L"+", op->getOperator().c_str());
        
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(op->getLHS()));
        ASSERT_EQUALS(L"2", i->valueAsString);
        
        
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(op->getRHS()));
        ASSERT_EQUALS(L"3", i->valueAsString);
        
        

        std::wstring name = args->getName(2);
        ASSERT_EQUALS(L"id", name.c_str());
        
        n = args->get(2);
        CPPUNIT_ASSERT(n != NULL);
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(n));
        ASSERT_EQUALS(L"5", i->valueAsString);
        
        
        
    }
    
    void testInit()
    {
        
        PARSE_STATEMENT(L"Shape.init(id : 5)");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction() != NULL);
        InitializerReferencePtr init = std::dynamic_pointer_cast<InitializerReference>(call->getFunction());
        CPPUNIT_ASSERT(init != NULL);

        IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(init->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"Shape", id->getIdentifier().c_str());
        
        ParenthesizedExpressionPtr args = call->getArguments();
        CPPUNIT_ASSERT(args != NULL);
        CPPUNIT_ASSERT_EQUAL(1, args->numExpressions());
        
        NodePtr n = args->get(0);
        CPPUNIT_ASSERT(n != NULL);
        IntegerLiteralPtr i = std::dynamic_pointer_cast<IntegerLiteral>(n);
        CPPUNIT_ASSERT(i != NULL);
        ASSERT_EQUALS(L"5", i->valueAsString);
        
        std::wstring name = args->getName(0);
        ASSERT_EQUALS(L"id", name.c_str());
        
    }
    
    void testSelf()
    {
        PARSE_STATEMENT(L"Shape.self");
        CPPUNIT_ASSERT(root != NULL);
        SelfExpressionPtr self = std::dynamic_pointer_cast<SelfExpression>(root);
        CPPUNIT_ASSERT(self != NULL);
        CPPUNIT_ASSERT(self->getExpression());
        IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(self->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"Shape", id->getIdentifier().c_str());
        
    }
    
    void testDynamicType()
    {
        PARSE_STATEMENT(L"345.dynamicType");
        CPPUNIT_ASSERT(root != NULL);
        DynamicTypePtr dyn = std::dynamic_pointer_cast<DynamicType>(root);
        CPPUNIT_ASSERT(dyn != NULL);
        CPPUNIT_ASSERT(dyn->getExpression());
        IntegerLiteralPtr id = std::dynamic_pointer_cast<IntegerLiteral>(dyn->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"345", id->valueAsString);
        
    }
    
    
    void testSubscript()
    {
        {
            PARSE_STATEMENT(L"matrix[0, 1] = 2");
            CPPUNIT_ASSERT(root != NULL);
            AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
            CPPUNIT_ASSERT(eq != NULL);
            CPPUNIT_ASSERT(eq->getLHS());
            SubscriptAccessPtr sub = std::dynamic_pointer_cast<SubscriptAccess>(eq->getLHS());
            CPPUNIT_ASSERT(eq != NULL);
            CPPUNIT_ASSERT_EQUAL(2, sub->numIndices());
            IntegerLiteralPtr i;
            
            CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(sub->getIndex(0)));
            ASSERT_EQUALS(L"0", i->valueAsString);
            CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(sub->getIndex(1)));
            ASSERT_EQUALS(L"1", i->valueAsString);
            
        }
        
    }
    void testSubscript2()
    {
        PARSE_STATEMENT(L"matrix[0, ] = 2");
        CPPUNIT_ASSERT(root != NULL);
        AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT(eq->getLHS());
        SubscriptAccessPtr sub = std::dynamic_pointer_cast<SubscriptAccess>(eq->getLHS());
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT_EQUAL(1, sub->numIndices());
        IntegerLiteralPtr i;
        
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(sub->getIndex(0)));
        ASSERT_EQUALS(L"0", i->valueAsString);
        
    }
    void testSubscript3()
    {
        PARSE_STATEMENT(L"matrix[0 ] = 2");
        CPPUNIT_ASSERT(root != NULL);
        AssignmentPtr eq = std::dynamic_pointer_cast<Assignment>(root);
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT(eq->getLHS());
        SubscriptAccessPtr sub = std::dynamic_pointer_cast<SubscriptAccess>(eq->getLHS());
        CPPUNIT_ASSERT(eq != NULL);
        CPPUNIT_ASSERT_EQUAL(1, sub->numIndices());
        IntegerLiteralPtr i;
        
        CPPUNIT_ASSERT(i = std::dynamic_pointer_cast<IntegerLiteral>(sub->getIndex(0)));
        ASSERT_EQUALS(L"0", i->valueAsString);
        
        
    }
    void testForcedValue()
    {
        PARSE_STATEMENT(L"val!");
        CPPUNIT_ASSERT(root != NULL);
        ForcedValuePtr val = std::dynamic_pointer_cast<ForcedValue>(root);
        CPPUNIT_ASSERT(val != NULL);
        CPPUNIT_ASSERT(val->getExpression());
        IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(val->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"val", id->getIdentifier().c_str());
        
    }
    
    void testOptionalChaining()
    {
        PARSE_STATEMENT(L"c?.performAction()");
        CPPUNIT_ASSERT(root != NULL);
        FunctionCallPtr call = std::dynamic_pointer_cast<FunctionCall>(root);
        CPPUNIT_ASSERT(call != NULL);
        CPPUNIT_ASSERT(call->getFunction());
        MemberAccessPtr mem = std::dynamic_pointer_cast<MemberAccess>(call->getFunction());
        CPPUNIT_ASSERT(mem != NULL);
        OptionalChainingPtr optional = std::dynamic_pointer_cast<OptionalChaining>(mem->getSelf());
        CPPUNIT_ASSERT(optional != NULL);
        IdentifierPtr id = std::dynamic_pointer_cast<Identifier>(optional->getExpression());
        CPPUNIT_ASSERT(id != NULL);
        ASSERT_EQUALS(L"c", id->getIdentifier().c_str());
        
        
    }
    
};
;

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestPostfixExpression, "alltest");
