/* TestDeclarationOrder.cpp --
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
#include "semantics/SymbolRegistry.h"
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GlobalScope.h"
#include "semantics/GenericArgument.h"

using namespace Swallow;
using namespace std;

TEST(TestDeclarationOrder, StructMemberOrder)
{
    SEMANTIC_ANALYZE(L"struct s \n"
        L"{ \n"
        L"    init()\n"
        L"    {\n"
        L"        self.test()\n"
        L"    }\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_NO_ERRORS();
}

TEST(TestDeclarationOrder, ClassMemberOrder)
{
    SEMANTIC_ANALYZE(L"class s \n"
        L"{ \n"
        L"    init()\n"
        L"    {\n"
        L"        self.test()\n"
        L"    }\n"
        L"    func test()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_NO_ERRORS();
}



TEST(TestDeclarationOrder, EnumMemberOrder)
{
    SEMANTIC_ANALYZE(L"enum s \n"
        L"{\n"
        L"    case A,B,C\n"
        L"    func foo()\n"
        L"    {\n"
        L"        self.bar()\n"
        L"    }\n"
        L"    func bar()\n"
        L"    {\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}

TEST(TestDeclarationOrder, GlobalDeclaration)
{
    SEMANTIC_ANALYZE(L"struct Foo {\n"
        L"}\n"
        L"var a : Foo = Foo()");
    ASSERT_NO_ERRORS();
    ASSERT_NOT_NULL(scope->lookup(L"Foo"));
}


TEST(TestDeclarationOrder, StructCrossReference)
{
    SEMANTIC_ANALYZE(L"struct Foo {\n"
        L"    func test(a : Bar){}\n"
        L"}\n"
        L"struct Bar {\n"
        L"    func test(a : Foo){}\n"
        L"}\n");
    ASSERT_NO_ERRORS();
    ASSERT_NOT_NULL(scope->lookup(L"Foo"));
    ASSERT_NOT_NULL(scope->lookup(L"Bar"));
}

TEST(TestDeclarationOrder, ClassCrossReference)
{
    SEMANTIC_ANALYZE(L"class Foo {\n"
        L"    func test(a : Bar){}\n"
        L"}\n"
        L"class Bar {\n"
        L"    func test(a : Foo){}\n"
        L"}\n");
    ASSERT_NO_ERRORS();
    ASSERT_NOT_NULL(scope->lookup(L"Foo"));
    ASSERT_NOT_NULL(scope->lookup(L"Bar"));
}



TEST(TestDeclarationOrder, ProtocolCrossReference)
{
    SEMANTIC_ANALYZE(L"protocol A\n"
        L"{\n"
        L"    func foo() -> B\n"
        L"}\n"
        L"protocol B\n"
        L"{\n"
        L"    func foo() -> A\n"
        L"}");
    ASSERT_NO_ERRORS();
    ASSERT_NOT_NULL(scope->lookup(L"A"));
    ASSERT_NOT_NULL(scope->lookup(L"B"));
}

TEST(TestDeclarationOrder, EnumCrossReference)
{
    SEMANTIC_ANALYZE(L"enum A\n"
        L"{\n"
        L"    case AA, BB\n"
        L"    func foo() -> B\n"
        L"    {\n"
        L"        return .BB\n"
        L"    }\n"
        L"}\n"
        L"enum B\n"
        L"{\n"
        L"    case AA, BB\n"
        L"    func foo() -> A\n"
        L"    {\n"
        L"        return .AA;\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
    ASSERT_NOT_NULL(scope->lookup(L"A"));
    ASSERT_NOT_NULL(scope->lookup(L"B"));
}

TEST(TestDeclarationOrder, Func_Struct_Var)
{
    SEMANTIC_ANALYZE(L"\n"
        L"func foobar() -> TTT\n"
        L"{\n"
        L"    return TTT()\n"
        L"}\n"
        L"struct TTT\n"
        L"{\n"
        L"    \n"
        L"}\n"
        L"var a = foobar()");
    ASSERT_NO_ERRORS();
    SymbolPtr a = scope->lookup(L"a");
    SymbolPtr TTT = scope->lookup(L"TTT");
    ASSERT_EQ(TTT, a->getType());
}

TEST(TestDeclarationOrder, Func_Use_Nested_Type)
{
    SEMANTIC_ANALYZE(L"struct Type\n"
                     L"{\n"
                     L"}\n"
                     L"extension Type\n"
                     L"{\n"
                     L"    typealias Inner2 = Int\n"
                     L"    struct Inner\n"
                     L"    {\n"
                     L"    }\n"
                     L"}\n"
                     L"\n"
                     L"func ==(a : Type.Inner, b : Type.Inner2)\n"
                     L"{\n"
                     L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestDeclarationOrder, Func_Use_NestedType2)
{
    SEMANTIC_ANALYZE(L"class MyClass\n"
                     L"{\n"
                     L"\n"
                     L"}\n"
                     L"extension MyClass\n"
                     L"{\n"
                     L"    struct Inner\n"
                     L"    {\n"
                     L"        typealias I = Int\n"
                     L"    }\n"
                     L"}\n"
                     L"\n"
                     L"var a : MyClass.Inner.I = 3\n");
    ASSERT_NO_ERRORS();
}

/*
//TODO: fix this later
TEST(TestDeclarationOrder, Func_Struct_Var_2)
{
    SEMANTIC_ANALYZE(L"\n"
        L"\n"
        L"func foobar() -> TTT\n"
        L"{\n"
        L"    return TTT()\n"
        L"}\n"
        L"var a = foobar()\n"
        L"\n"
        L"struct TTT\n"
        L"{\n"
        L"    \n"
        L"}");
    ASSERT_ERROR(Errors::E_USE_OF_UNDECLARED_TYPE_1);
    ASSERT_EQ(L"TTT", error->items[0]);
}

*/
