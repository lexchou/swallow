#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "utils.h"

using namespace Swift;

class TestClosure : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestClosure);
    CPPUNIT_TEST(testClosure);
    CPPUNIT_TEST(testInferringType);
    CPPUNIT_TEST(testImplicitReturn);
    CPPUNIT_TEST(testShorthandArgument);
    CPPUNIT_TEST(testOperatorFunction);
    CPPUNIT_TEST(testTrailingClosures);
    CPPUNIT_TEST(testCaptureList);
    CPPUNIT_TEST_SUITE_END();
public:
    void testClosure()
    {
        PARSE_STATEMENT(L"sort(names, { (s1: String, s2: String) -> Bool in "
                        L"  return s1 > s2 "
                        L"})");
        FunctionCall* f = NULL;
        Closure* c = NULL;
        TypeIdentifier* type = NULL;
        Parameter* param = NULL;
        ReturnStatement* ret = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(c = dynamic_cast<Closure*>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(c->getReturnType()));
        ASSERT_EQUALS(L"Bool", type->getName());
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(type = dynamic_cast<TypeIdentifier*>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(c->getStatement(0)));
        CPPUNIT_ASSERT(dynamic_cast<BinaryOperator*>(ret->getExpression()));
        
        DESTROY(root);
    }
    
    void testInferringType()
    {
        PARSE_STATEMENT(L"sort(names, { s1, s2 in return s1 > s2 } )");
        
        FunctionCall* f = NULL;
        Closure* c = NULL;
        Parameter* param = NULL;
        ReturnStatement* ret = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(c = dynamic_cast<Closure*>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(ret = dynamic_cast<ReturnStatement*>(c->getStatement(0)));
        CPPUNIT_ASSERT(dynamic_cast<BinaryOperator*>(ret->getExpression()));
        
        DESTROY(root);
    }
    
    void testImplicitReturn()
    {
        PARSE_STATEMENT(L"sort(names, { s1, s2 in s1 > s2 } )");
        
        FunctionCall* f = NULL;
        Closure* c = NULL;
        Parameter* param = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(c = dynamic_cast<Closure*>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(dynamic_cast<BinaryOperator*>(c->getStatement(0)));
        
        DESTROY(root);
    }
    void testShorthandArgument()
    {
        PARSE_STATEMENT(L"sort(names, { $0 > $1 } )");
        
        FunctionCall* f = NULL;
        Closure* c = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(c = dynamic_cast<Closure*>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        CPPUNIT_ASSERT(NULL == c->getParameters());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(dynamic_cast<BinaryOperator*>(c->getStatement(0)));
        
        DESTROY(root);
    }
    
    void testOperatorFunction()
    {
        PARSE_STATEMENT(L"sort(names, >)");
        
        FunctionCall* f = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(dynamic_cast<BinaryOperator*>(f->getArguments()->get(1)));
        
        DESTROY(root);
    }
    
    
    void testTrailingClosures()
    {
        PARSE_STATEMENT(L"sort(names) { $0 > $1 }");
        
        FunctionCall* f = NULL;
        Closure* c = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(c = dynamic_cast<Closure*>(f->getTrailingClosure()));

        
        DESTROY(root);
    }
    
    void testCaptureList()
    {
        
        PARSE_STATEMENT(L"sort(names, { [unowned self] s1, s2 in s1 > s2 } )");
        
        FunctionCall* f = NULL;
        Closure* c = NULL;
        Parameter* param = NULL;
        Identifier* id = NULL;
        CPPUNIT_ASSERT(f = dynamic_cast<FunctionCall*>(root));
        CPPUNIT_ASSERT(c = dynamic_cast<Closure*>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT_EQUAL(Closure::Unowned, c->getCaptureSpecifier());
        CPPUNIT_ASSERT(id = dynamic_cast<Identifier*>(c->getCapture()));
        ASSERT_EQUALS(L"self", id->getIdentifier().c_str());
        
        
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        
        
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(dynamic_cast<BinaryOperator*>(c->getStatement(0)));
        
        DESTROY(root);
    }
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestClosure, "alltest");



#endif
