/* TestClosure.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "../utils.h"

using namespace Swallow;

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
    

