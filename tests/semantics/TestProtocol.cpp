/* TestProtocol.cpp --
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
#include "semantics/FunctionSymbol.h"
#include "semantics/FunctionOverloadedSymbol.h"
#include "common/Errors.h"


using namespace Swallow;

TEST(TestProtocol, ProtocolInheritance)
{
    SEMANTIC_ANALYZE(L"protocol Base{}\n"
        L"protocol Child : Base{}")
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr Base, Child;
    ASSERT_NOT_NULL(Base = std::dynamic_pointer_cast<Type>(scope->lookup(L"Base")));
    ASSERT_NOT_NULL(Child = std::dynamic_pointer_cast<Type>(scope->lookup(L"Child")));

}

TEST(TestProtocol, ProtocolHasNoVariable)
{
    SEMANTIC_ANALYZE(L"protocol Test {\n"
        L"var a : Int = 3\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1, res.code);
}


TEST(TestProtocol, MethodRequirement_Decl)
{
    SEMANTIC_ANALYZE(L"protocol RandomNumberGenerator {\n"
            L" func random() -> Double\n"
            L"  } ");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr RandomNumberGenerator;
    ASSERT_NOT_NULL(RandomNumberGenerator = std::dynamic_pointer_cast<Type>(scope->lookup(L"RandomNumberGenerator")));
    SymbolPtr sym = RandomNumberGenerator->getDeclaredMember(L"random");
    FunctionOverloadedSymbolPtr random;

    ASSERT_NOT_NULL(random = std::dynamic_pointer_cast<FunctionOverloadedSymbol>(sym));
    ASSERT_EQ(1, random->numOverloads());

}

TEST(TestProtocol, MethodRequirements_DefaultParameterInitializer)

{
    SEMANTIC_ANALYZE(L"protocol RandomNumberGenerator {\n"
            L" func random(a : Int = 3) -> Double\n"
            L"  } ");

    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_DEFAULT_ARGUMENT_NOT_PERMITTED_IN_A_PROTOCOL_METHOD, res.code);


}
TEST(TestProtocol, MethodRequirements_Impl)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {"
        L"func test()\n"
        L"}\n"
        L"class Test : MyProtocol { \n"
        L"func test(){}\n"
        L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestProtocol, MethodRequirements_NotImpl)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {"
        L"func test()\n"
        L"}\n"
        L"class Test : MyProtocol { \n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_FUNCTION_3, res.code);
}

TEST(TestProtocol, TypeRequirements)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {\n"
        L"typealias Element\n"
        L"}\n"
        L"class Test : MyProtocol\n"
        L"{\n"
        L"  class Element {}\n"
        L"}\n");
    ASSERT_EQ(0, compilerResults.numResults());
}
/*
NOTE: TypeRequirement is optional, if the type is being used in methods, it should be infered by compiler
TEST(TestProtocol, TypeRequirements_Impl)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {\n"
            "typealias Element\n"
            "}\n"
            "class Test : MyProtocol\n"
            "{\n"
            "}\n");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_TYPE_3, res.code);
}
*/

TEST(TestProtocol, TypeInheritance)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol { typealias Element = Int }\n"
        L"class Test : MyProtocol {}");
    //TODO: type inference for let a : Test.Element
    ASSERT_EQ(0, compilerResults.numResults());

}

TEST(TestProtocol, Let)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {\n"
        L"let a : Int = 3\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_PROTOCOL_CANNOT_DEFINE_LET_CONSTANT_1, res.code);
}

TEST(TestProtocol, Var)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {\n"
        L"var a : Int = 3\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_PROTOCOL_VAR_MUST_BE_COMPUTED_PROPERTY_1, res.code);
}

TEST(TestProtocol, ReadOnlyComputedProperty)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol {\n"
        L"var a : Int {get}\n"
        L"}");
    ASSERT_EQ(0, compilerResults.numResults());

    TypePtr RandomNumberGenerator;
    ASSERT_NOT_NULL(RandomNumberGenerator = std::dynamic_pointer_cast<Type>(scope->lookup(L"MyProtocol")));
    SymbolPtr sym = RandomNumberGenerator->getDeclaredMember(L"a");
    SymbolPlaceHolderPtr a;

    ASSERT_NOT_NULL(a = std::dynamic_pointer_cast<SymbolPlaceHolder>(sym));
    ASSERT_TRUE((a->flags & SymbolPlaceHolder::F_MEMBER) != 0);
    ASSERT_TRUE((a->flags & SymbolPlaceHolder::F_READABLE) != 0);
    ASSERT_TRUE((a->flags & SymbolPlaceHolder::F_WRITABLE) == 0);

}
TEST(TestProtocol, PropertyRequirement)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol { \n"
        L"var a : Int {get set} \n"
        L"}\n"
        L"class MyClass : MyProtocol {\n"
        L"var a : Int {get { return 3}}\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3, res.code);


}

TEST(TestProtocol, PropertyRequirement2)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol { \n"
        L"var a : Int {get set} \n"
        L"}\n"
        L"class MyClass : MyProtocol {\n"
        L"var af : Int {get { return 3}}\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNIMPLEMENTED_PROPERTY_3, res.code);


}

TEST(TestProtocol, PropertyRequirement3)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol { \n"
        L"var a : Int {get set} \n"
        L"}\n"
        L"class MyClass : MyProtocol {"
        L"var a : Int {get { return 0 } set {}}\n"
        L"}");
    ASSERT_EQ(0, compilerResults.numResults());

}

TEST(TestProtocol, PropertyRequirement4)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol { \n"
        L"var a : Int {get set} \n"
        L"}\n"
        L"class MyClass : MyProtocol {\n"
        L"var a : Int\n"
        L"}");
    ASSERT_EQ(0, compilerResults.numResults());

}

TEST(TestProtocol, PropertyRequirement5)
{
    SEMANTIC_ANALYZE(L"protocol MyProtocol { \n"
        L"var a : Int {get set} \n"
        L"}\n"
        L"class MyClass : MyProtocol {\n"
        L"let a : Int = 3\n"
        L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_TYPE_DOES_NOT_CONFORM_TO_PROTOCOL_UNWRITABLE_PROPERTY_3, res.code);


}

