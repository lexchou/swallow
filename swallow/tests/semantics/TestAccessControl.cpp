/* TestAccessControl.cpp --
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
#include "semantics/FunctionOverloadedSymbol.h"
#include "semantics/FunctionSymbol.h"
#include "common/Errors.h"


using namespace Swallow;
using namespace std;

TEST(TestAccessControl, AccessControlSyntax)
{
    SEMANTIC_ANALYZE(L"public class SomePublicClass {}\n"
            L"internal class SomeInternalClass {}\n"
            L"private class SomePrivateClass {}\n"
            L" \n"
            L"public var somePublicVariable = 0\n"
            L"internal let someInternalConstant = 0\n"
            L"private func somePrivateFunction() {}");
    ASSERT_NO_ERRORS();
    SymbolPtr SomePublicClass, SomeInternalClass, SomePrivateClass, somePublicVariable, someInternalConstant, somePrivateFunction;
    ASSERT_NOT_NULL(SomePublicClass = scope->lookup(L"SomePublicClass"));
    ASSERT_NOT_NULL(SomeInternalClass = scope->lookup(L"SomeInternalClass"));
    ASSERT_NOT_NULL(SomePrivateClass = scope->lookup(L"SomePrivateClass"));
    ASSERT_NOT_NULL(somePublicVariable = scope->lookup(L"somePublicVariable"));
    ASSERT_NOT_NULL(someInternalConstant = scope->lookup(L"someInternalConstant"));
    ASSERT_NOT_NULL(somePrivateFunction = scope->lookup(L"somePrivateFunction"));
    ASSERT_EQ(AccessLevelPublic, SomePublicClass->getAccessLevel());
    ASSERT_EQ(AccessLevelInternal, SomeInternalClass->getAccessLevel());
    ASSERT_EQ(AccessLevelPrivate, SomePrivateClass->getAccessLevel());
    ASSERT_EQ(AccessLevelPublic, somePublicVariable->getAccessLevel());
    ASSERT_EQ(AccessLevelInternal, someInternalConstant->getAccessLevel());
    ASSERT_EQ(AccessLevelPrivate, somePrivateFunction->getAccessLevel());
}
TEST(TestAccessControl, DefaultLevels)
{
    SEMANTIC_ANALYZE(L"public class SomePublicClass {          // explicitly public class\n"
            L"    public var somePublicProperty = 0    // explicitly public class member\n"
            L"    var someInternalProperty = 0         // implicitly internal class member\n"
            L"    private func somePrivateMethod() {}  // explicitly private class member\n"
            L"}\n"
            L" \n"
            L"class SomeInternalClass {               // implicitly internal class\n"
            L"    var someInternalProperty = 0         // implicitly internal class member\n"
            L"    private func somePrivateMethod() {}  // explicitly private class member\n"
            L"}\n"
            L" \n"
            L"private class SomePrivateClass {        // explicitly private class\n"
            L"    var somePrivateProperty = 0          // implicitly private class member\n"
            L"    func somePrivateMethod() {}          // implicitly private class member\n"
            L"}");
    TypePtr SomePublicClass, SomeInternalClass, SomePrivateClass;
    ASSERT_NOT_NULL(SomePublicClass = static_pointer_cast<Type>(scope->lookup(L"SomePublicClass")));
    ASSERT_NOT_NULL(SomeInternalClass = static_pointer_cast<Type>(scope->lookup(L"SomeInternalClass")));
    ASSERT_NOT_NULL(SomePrivateClass = static_pointer_cast<Type>(scope->lookup(L"SomePrivateClass")));
    ASSERT_NO_ERRORS();
    FunctionOverloadedSymbolPtr funcs;
    SymbolPtr m;

    //SomePublicClass
    ASSERT_EQ(AccessLevelPublic, SomePublicClass->getAccessLevel());

    ASSERT_NOT_NULL(m = SomePublicClass->getMember(L"somePublicProperty"));
    ASSERT_EQ(AccessLevelPublic, m->getAccessLevel());

    ASSERT_NOT_NULL(m = SomePublicClass->getMember(L"someInternalProperty"));
    ASSERT_EQ(AccessLevelInternal, m->getAccessLevel());

    ASSERT_NOT_NULL(funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(SomePublicClass->getMember(L"somePrivateMethod")));
    ASSERT_EQ(AccessLevelPrivate, (*funcs->begin())->getAccessLevel());

    //SomeInternalClass
    ASSERT_EQ(AccessLevelInternal, SomeInternalClass->getAccessLevel());
    ASSERT_NOT_NULL(m = SomeInternalClass->getMember(L"someInternalProperty"));
    ASSERT_EQ(AccessLevelInternal, m->getAccessLevel());

    ASSERT_NOT_NULL(funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(SomeInternalClass->getMember(L"somePrivateMethod")));
    ASSERT_EQ(AccessLevelPrivate, (*funcs->begin())->getAccessLevel());

    //SomePrivateClass
    ASSERT_EQ(AccessLevelPrivate, SomePrivateClass->getAccessLevel());
    ASSERT_NOT_NULL(m = SomePrivateClass->getMember(L"somePrivateProperty"));
    ASSERT_EQ(AccessLevelPrivate, m->getAccessLevel());

    ASSERT_NOT_NULL(funcs = dynamic_pointer_cast<FunctionOverloadedSymbol>(SomePrivateClass->getMember(L"somePrivateMethod")));
    ASSERT_EQ(AccessLevelPrivate, (*funcs->begin())->getAccessLevel());
}

TEST(TestAccessControl, DuplicatedModifiers)
{
    SEMANTIC_ANALYZE(L"internal private class Base\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_ERROR(Errors::E_DUPLICATE_MODIFIER);
}
TEST(TestAccessControl, BaseHasLowerAccess)
{
    SEMANTIC_ANALYZE(L"private class Base\n"
            L"{\n"
            L"    \n"
            L"}\n"
            L"public class Child : Base\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Class", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"superclass", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, BaseHasLowerAccess2)
{
    SEMANTIC_ANALYZE(L"private class Base\n"
            L"{\n"
            L"    \n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_ERROR(Errors::E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Class", error->items[0]);
    ASSERT_EQ(L"private", error->items[1]);
    ASSERT_EQ(L"superclass", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, InheritBaseAccessLevel)
{
    SEMANTIC_ANALYZE(L"public class Base\n"
            L"{\n"
            L"    \n"
            L"}\n"
            L"class Child : Base\n"
            L"{\n"
            L"    \n"
            L"}");
    ASSERT_NO_ERRORS();
    SymbolPtr Child = scope->lookup(L"Child");
    ASSERT_NOT_NULL(Child);
    ASSERT_EQ(AccessLevelPublic, Child->getAccessLevel());
}


TEST(TestAccessControl, Variable)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"public var (global, g2): (V, V) = (V(), V())");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Variable", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"type", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Variable2)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"var (global, g2): (V, V) = (V(), V())");
    ASSERT_ERROR(Errors::E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Variable", error->items[0]);
    ASSERT_EQ(L"private", error->items[1]);
    ASSERT_EQ(L"type", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Variable3)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"public var PROP : V { return V()}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Variable", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"type", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Variable4)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"var PROP : V { return V()}");
    ASSERT_ERROR(Errors::E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Variable", error->items[0]);
    ASSERT_EQ(L"private", error->items[1]);
    ASSERT_EQ(L"type", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Function)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"public func a(v : V)\n"
            L"{\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Function", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"parameter", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Function2)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"func a(v : V)\n"
            L"{\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_MUST_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Function", error->items[0]);
    ASSERT_EQ(L"private", error->items[1]);
    ASSERT_EQ(L"parameter", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}

TEST(TestAccessControl, Initializer)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
           L"public class Class\n"
           L"{\n"
           L"    public init(a : INT)\n"
           L"    {\n"
           L"        \n"
           L"    }\n"
           L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Initializer", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"parameter", error->items[2]);
    ASSERT_EQ(L"internal", error->items[3]);
}
TEST(TestAccessControl, Initializer2)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"public class Class\n"
            L"{\n"
            L"    init(a : INT)\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
/*
TEST(TestAccessControl, Initializer3)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"class Class\n"
            L"{\n"
            L"    public init(a : INT)\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
*/
TEST(TestAccessControl, Subscript)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"public class Class\n"
            L"{\n"
            L"    public subscript(a : INT) -> Int\n"
            L"    {\n"
            L"        return 5\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Subscript", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"index", error->items[2]);
    ASSERT_EQ(L"internal", error->items[3]);
}
TEST(TestAccessControl, Subscript2)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"public class Class\n"
            L"{\n"
            L"    subscript(a : INT) -> Int\n"
            L"    {\n"
            L"        return 5\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestAccessControl, Method)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"public class Class\n"
            L"{\n"
            L"    public func foo(a : INT)\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Method", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"parameter", error->items[2]);
    ASSERT_EQ(L"internal", error->items[3]);
}
TEST(TestAccessControl, Method2)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"public class Class\n"
            L"{\n"
            L"    public func foo() -> INT\n"
            L"    {\n"
            L"       return INT()\n"
            L"    }\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Method", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"result", error->items[2]);
    ASSERT_EQ(L"internal", error->items[3]);
}
TEST(TestAccessControl, Method3)
{
    SEMANTIC_ANALYZE(L"class INT{}\n"
            L"public class Class\n"
            L"{\n"
            L"    func foo() -> INT\n"
            L"    {\n"
            L"       return INT()\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestAccessControl, Property)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"public class Class\n"
            L"{\n"
            L"    public var a : V = V()\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Property", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"type", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Property2)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"public class Class\n"
            L"{\n"
            L"    private var a : V = V()\n"
            L"}");
    ASSERT_NO_ERRORS();
}
TEST(TestAccessControl, Property3)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"public class Class\n"
            L"{\n"
            L"    public var a : V { return V() }\n"
            L"}");
    ASSERT_ERROR(Errors::E_A_CANNOT_BE_DECLARED_B_BECAUSE_ITS_C_USES_A_D_TYPE_4);
    ASSERT_EQ(L"Property", error->items[0]);
    ASSERT_EQ(L"public", error->items[1]);
    ASSERT_EQ(L"type", error->items[2]);
    ASSERT_EQ(L"private", error->items[3]);
}
TEST(TestAccessControl, Property4)
{
    SEMANTIC_ANALYZE(L"private class V{}\n"
            L"\n"
            L"public class Class\n"
            L"{\n"
            L"    private var a : V { return V() }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
