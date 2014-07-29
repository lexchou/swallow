#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

class TestClosure : public SwiftTestCase
{
    CPPUNIT_TEST_SUITE(TestClosure);
    CPPUNIT_TEST(testClosure);
    CPPUNIT_TEST(testInferringType);
    CPPUNIT_TEST(testImplicitReturn);
    CPPUNIT_TEST(testShorthandArgument);
    CPPUNIT_TEST(testOperatorFunction);
    CPPUNIT_TEST(testTrailingClosure);
    CPPUNIT_TEST(testTrailingClosure2);
    CPPUNIT_TEST(testCaptureList);
    CPPUNIT_TEST_SUITE_END();
public:
    void testClosure()
    {
        PARSE_STATEMENT(L"sort(names, { (s1: String, s2: String) -> Bool in "
                        L"  return s1 > s2 "
                        L"})");
        FunctionCallPtr f = NULL;
        ClosurePtr c = NULL;
        TypeIdentifierPtr type = NULL;
        ParameterPtr param = NULL;
        ReturnStatementPtr ret = NULL;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(c->getReturnType()));
        ASSERT_EQUALS(L"Bool", type->getName());
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getType()));
        ASSERT_EQUALS(L"String", type->getName());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(c->getStatement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<BinaryOperator>(ret->getExpression()));
        
    }
    
    void testInferringType()
    {
        PARSE_STATEMENT(L"sort(names, { s1, s2 in return s1 > s2 } )");
        
        FunctionCallPtr f;
        ClosurePtr c;
        ParameterPtr param;
        ReturnStatementPtr ret;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(ret = std::dynamic_pointer_cast<ReturnStatement>(c->getStatement(0)));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<BinaryOperator>(ret->getExpression()));
        
    }
    
    void testImplicitReturn()
    {
        PARSE_STATEMENT(L"sort(names, { s1, s2 in s1 > s2 } )");
        
        FunctionCallPtr f;
        ClosurePtr c;
        ParameterPtr param;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        
        CPPUNIT_ASSERT_EQUAL(2, c->getParameters()->numParameters());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(0));
        ASSERT_EQUALS(L"s1", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT(param = c->getParameters()->getParameter(1));
        ASSERT_EQUALS(L"s2", param->getLocalName());
        CPPUNIT_ASSERT(NULL == param->getType());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<BinaryOperator>(c->getStatement(0)));
        
    }
    void testShorthandArgument()
    {
        PARSE_STATEMENT(L"sort(names, { $0 > $1 } )");
        
        FunctionCallPtr f;
        ClosurePtr c;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT(NULL == c->getReturnType());
        CPPUNIT_ASSERT(NULL == c->getParameters());
        
        CPPUNIT_ASSERT_EQUAL(1, c->numStatement());
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<BinaryOperator>(c->getStatement(0)));
        
    }
    
    void testOperatorFunction()
    {
        PARSE_STATEMENT(L"sort(names, >)");
        
        FunctionCallPtr f;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<BinaryOperator>(f->getArguments()->get(1)));
        
    }
    
    
    void testTrailingClosure()
    {
        PARSE_STATEMENT(L"sort(names) { $0 > $1 }");
        
        FunctionCallPtr f;
        ClosurePtr c;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getTrailingClosure()));

        
    }
    
    
    void testTrailingClosure2()
    {
        PARSE_STATEMENT(L"3.repetitions {\n"
                        L"println(\"Goodbye!\")\n"
                        L"}");
        
        FunctionCallPtr f;
        ClosurePtr c;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getTrailingClosure()));

        
    }
    
    void testCaptureList()
    {
        
        PARSE_STATEMENT(L"sort(names, { [unowned self] s1, s2 in s1 > s2 } )");
        
        FunctionCallPtr f;
        ClosurePtr c;
        ParameterPtr param;
        IdentifierPtr id;
        CPPUNIT_ASSERT(f = std::dynamic_pointer_cast<FunctionCall>(root));
        CPPUNIT_ASSERT(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
        CPPUNIT_ASSERT_EQUAL(Closure::Unowned, c->getCaptureSpecifier());
        CPPUNIT_ASSERT(id = std::dynamic_pointer_cast<Identifier>(c->getCapture()));
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
        CPPUNIT_ASSERT(std::dynamic_pointer_cast<BinaryOperator>(c->getStatement(0)));
        
    }
    
};
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(TestClosure, "alltest");



#endif
