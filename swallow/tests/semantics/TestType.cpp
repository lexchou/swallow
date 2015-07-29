/* TestType.cpp --
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
#include "semantics/Type.h"
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "common/Errors.h"


using namespace Swallow;

TEST(TestType, testCovariance)
{
    TypePtr Base = Type::newType(L"Base", Type::Class);
    TypePtr Base2 = Type::newType(L"Base2", Type::Class, nullptr, Base);

    TypePtr Child1 = Type::newType(L"Child1", Type::Class, nullptr, Base2);
    TypePtr Child2 = Type::newType(L"Child2", Type::Class, nullptr, Base2);
    //calculate the common parent class between Child1 and Child2 with the minimum inheritance distance
    TypePtr parent = Child1->getCommonParent(Child2);
    ASSERT_NOT_NULL(parent);
    ASSERT_TRUE(parent == Base2);

}

TEST(TestType, testInheritance)
{
    SEMANTIC_ANALYZE(L"class SomeSuperclass{}\n"
        L"class SomeClass : SomeSuperclass{}");
    TypePtr SomeSuperclass, SomeClass;

    ASSERT_NO_ERRORS();
    ASSERT_NOT_NULL(SomeSuperclass = std::dynamic_pointer_cast<Type>(scope->lookup(L"SomeSuperclass")));
    ASSERT_NOT_NULL(SomeClass = std::dynamic_pointer_cast<Type>(scope->lookup(L"SomeClass")));

    ASSERT_EQ(SomeSuperclass, SomeClass->getParentType());

}

TEST(TestType, testTypealias)
{
    SEMANTIC_ANALYZE(L"typealias IntegerLiteralType = Int");
    TypePtr IntegerLiteralType, Int;
    ASSERT_NO_ERRORS();
    Int = symbolRegistry.lookupType(L"Int");
    ASSERT_NOT_NULL(IntegerLiteralType = std::dynamic_pointer_cast<Type>(scope->lookup(L"IntegerLiteralType")));
    ASSERT_EQ(Int, IntegerLiteralType);
}


TEST(TestType, testTypealias_UndeclaredType)
{
    SEMANTIC_ANALYZE(L"typealias IntegerLiteralType = Intff");

    ASSERT_ERROR(Errors::E_USE_OF_UNDECLARED_TYPE_1);
    ASSERT_EQ(L"Intff", error->items[0]);
}

TEST(TestType, testTypealias_Redefinition)
{
    SEMANTIC_ANALYZE(L"typealias IntegerLiteralType = Int\n"
        L"typealias IntegerLiteralType = Intff");

    ASSERT_EQ(1, compilerResults.numResults());
    auto result = compilerResults.getResult(0);

    ASSERT_EQ(Errors::E_INVALID_REDECLARATION_1, result.code);
    ASSERT_EQ(L"IntegerLiteralType", result.items[0]);
}
TEST(TestType, NestedGenericType)
{
    SEMANTIC_ANALYZE(L"class Base"
        L"{"
        L"    class Child<T>"
        L"    {"
        L"    }"
        L"}");
    //generic type 'Child' nested in type 'Base' is not allowed
    ASSERT_EQ(1, compilerResults.numResults());
    auto result = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_GENERIC_TYPE_A_NESTED_IN_TYPE_B_IS_NOT_ALLOWED_2, result.code);
    ASSERT_EQ(L"Child", result.items[0]);
    ASSERT_EQ(L"Base", result.items[1]);
}

TEST(TestType, TypeNestedInGenericType)
{
    SEMANTIC_ANALYZE(L"class Base<T>"
        L"{"
        L"    class Child"
        L"    {"
        L"    }"
        L"}");
    //type 'Child' nested in generic type 'Base' is not allowed
    ASSERT_EQ(1, compilerResults.numResults());
    auto result = compilerResults.getResult(0);

    ASSERT_EQ(Errors::E_TYPE_A_NESTED_IN_GENERIC_TYPE_B_IS_NOT_ALLOWED_2, result.code);
    ASSERT_EQ(L"Child", result.items[0]);
    ASSERT_EQ(L"Base", result.items[1]);
}

TEST(TestType, InitializeClass)
{
    SEMANTIC_ANALYZE(L"class cobject{var fd = -1}\n"
            L"let b = cobject()");
    ASSERT_NO_ERRORS();
}

TEST(TestType, InitsWithStoredProperties)
{
    SEMANTIC_ANALYZE(L"struct foo\n"
            L"{\n"
            L"    var (_a, _b) : (Int, Int)\n"
            L"    var a : Int\n"
            L"    {\n"
            L"        get {return _a}\n"
            L"        set {_a = newValue}\n"
            L"    }\n"
            L"}\n"
            L"\n"
            L"var a = foo(_a : 3, _b : 4)\n"
            L"\n");
    ASSERT_NO_ERRORS();
}


TEST(TestType, LetWithoutInitializer)
{
    SEMANTIC_ANALYZE(L"class Foo\n"
            L"{"
            L"    let a : Int"
            L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestType, TypeAliasRecursion)
{
    SEMANTIC_ANALYZE(L"typealias T = Test\n"
            L"struct Test\n"
            L"{\n"
            L"    func foo() -> T {\n"
            L"        return self\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
