#ifndef TEST_TYPE_H
#define TEST_TYPE_H

#include "tests/utils.h"

using namespace Swift;

TEST(TestClosure, testClosure)
{
    PARSE_STATEMENT(L"sort(names, { (s1: String, s2: String) -> Bool in "
                    L"  return s1 > s2 "
                    L"})");
    FunctionCallPtr f = NULL;
    ClosurePtr c = NULL;
    TypeIdentifierPtr type = NULL;
    ParameterPtr param = NULL;
    ReturnStatementPtr ret = NULL;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(c->getReturnType()));
    ASSERT_EQ(L"Bool", type->getName());
    ASSERT_EQ(2, c->getParameters()->numParameters());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(0));
    ASSERT_EQ(L"s1", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"String", type->getName());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(1));
    ASSERT_EQ(L"s2", param->getLocalName());
    ASSERT_NOT_NULL(type = std::dynamic_pointer_cast<TypeIdentifier>(param->getDeclaredType()));
    ASSERT_EQ(L"String", type->getName());

    ASSERT_EQ(1, c->numStatement());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(c->getStatement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(ret->getExpression()));

}

TEST(TestClosure, testInferringType)
{
    PARSE_STATEMENT(L"sort(names, { s1, s2 in return s1 > s2 } )");

    FunctionCallPtr f;
    ClosurePtr c;
    ParameterPtr param;
    ReturnStatementPtr ret;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
    ASSERT_NULL(c->getReturnType());

    ASSERT_EQ(2, c->getParameters()->numParameters());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(0));
    ASSERT_EQ(L"s1", param->getLocalName());
    ASSERT_NULL(param->getType());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(1));
    ASSERT_EQ(L"s2", param->getLocalName());
    ASSERT_NULL(param->getType());

    ASSERT_EQ(1, c->numStatement());
    ASSERT_NOT_NULL(ret = std::dynamic_pointer_cast<ReturnStatement>(c->getStatement(0)));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(ret->getExpression()));

}

TEST(TestClosure, testImplicitReturn)
{
    PARSE_STATEMENT(L"sort(names, { s1, s2 in s1 > s2 } )");

    FunctionCallPtr f;
    ClosurePtr c;
    ParameterPtr param;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
    ASSERT_NULL(c->getReturnType());

    ASSERT_EQ(2, c->getParameters()->numParameters());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(0));
    ASSERT_EQ(L"s1", param->getLocalName());
    ASSERT_NULL(param->getType());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(1));
    ASSERT_EQ(L"s2", param->getLocalName());
    ASSERT_NULL(param->getType());

    ASSERT_EQ(1, c->numStatement());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(c->getStatement(0)));

}
TEST(TestClosure, testShorthandArgument)
{
    PARSE_STATEMENT(L"sort(names, { $0 > $1 } )");

    FunctionCallPtr f;
    ClosurePtr c;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
    ASSERT_NULL(c->getReturnType());
    ASSERT_NULL(c->getParameters());

    ASSERT_EQ(1, c->numStatement());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(c->getStatement(0)));

}

TEST(TestClosure, testOperatorFunction)
{
    PARSE_STATEMENT(L"sort(names, >)");

    FunctionCallPtr f;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(f->getArguments()->get(1)));

}


TEST(TestClosure, testTrailingClosure)
{
    PARSE_STATEMENT(L"sort(names) { $0 > $1 }");

    FunctionCallPtr f;
    ClosurePtr c;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getTrailingClosure()));


}


TEST(TestClosure, testTrailingClosure2)
{
    PARSE_STATEMENT(L"3.repetitions {\n"
                    L"println(\"Goodbye!\")\n"
                    L"}");

    FunctionCallPtr f;
    ClosurePtr c;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getTrailingClosure()));


}

TEST(TestClosure, testCaptureList)
{

    PARSE_STATEMENT(L"sort(names, { [unowned self] s1, s2 in s1 > s2 } )");

    FunctionCallPtr f;
    ClosurePtr c;
    ParameterPtr param;
    IdentifierPtr id;
    ASSERT_NOT_NULL(f = std::dynamic_pointer_cast<FunctionCall>(root));
    ASSERT_NOT_NULL(c = std::dynamic_pointer_cast<Closure>(f->getArguments()->get(1)));
    ASSERT_EQ(Closure::Unowned, c->getCaptureSpecifier());
    ASSERT_NOT_NULL(id = std::dynamic_pointer_cast<Identifier>(c->getCapture()));
    ASSERT_EQ(L"self", id->getIdentifier());


    ASSERT_NULL(c->getReturnType());


    ASSERT_EQ(2, c->getParameters()->numParameters());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(0));
    ASSERT_EQ(L"s1", param->getLocalName());
    ASSERT_NULL(param->getType());

    ASSERT_NOT_NULL(param = c->getParameters()->getParameter(1));
    ASSERT_EQ(L"s2", param->getLocalName());
    ASSERT_NULL(param->getType());

    ASSERT_EQ(1, c->numStatement());
    ASSERT_NOT_NULL(std::dynamic_pointer_cast<BinaryOperator>(c->getStatement(0)));

}
    


#endif
