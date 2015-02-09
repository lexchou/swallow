/* TestOperators.cpp --
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



TEST(TestOperators, OperatorFunctions)
{

    SEMANTIC_ANALYZE(L"struct Vector2D {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"func + (left: Vector2D, right: Vector2D) -> Vector2D {\n"
            L"    return Vector2D(x: left.x + right.x, y: left.y + right.y)\n"
            L"}"
            L"let vector = Vector2D(x: 3.0, y: 1.0)\n"
            L"let anotherVector = Vector2D(x: 2.0, y: 4.0)\n"
            L"let combinedVector = vector + anotherVector");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr combinedVector = scope->lookup(L"combinedVector");
    ASSERT_NOT_NULL(combinedVector);
    TypePtr Vector2D = dynamic_pointer_cast<Type>(scope->lookup(L"Vector2D"));
    ASSERT_NOT_NULL(Vector2D);
    ASSERT_EQ(Vector2D, combinedVector->getType());
}


TEST(TestOperators, CompoundAssignmentOperators)
{

    SEMANTIC_ANALYZE(L"struct Vector2D {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"func + (left: Vector2D, right: Vector2D) -> Vector2D {\n"
            L"    return Vector2D(x: left.x + right.x, y: left.y + right.y)\n"
            L"}"
            L"func += (inout left: Vector2D, right: Vector2D) {\n"
            L"    left = left + right\n"
            L"}\n"
            L"var original = Vector2D(x: 1.0, y: 2.0)\n"
            L"let vectorToAdd = Vector2D(x: 3.0, y: 4.0)\n"
            L"original += vectorToAdd\n"
            L"prefix func ++ (inout vector: Vector2D) -> Vector2D {\n"
            L"    vector += Vector2D(x: 1.0, y: 1.0)\n"
            L"    return vector\n"
            L"}\n"
            L"var toIncrement = Vector2D(x: 3.0, y: 4.0)\n"
            L"let afterIncrement = ++toIncrement\n");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestOperators, PrefixAndPostfixOperators)
{

    SEMANTIC_ANALYZE(L"struct Vector2D {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"prefix func - (vector: Vector2D) -> Vector2D {\n"
            L"    return Vector2D(x: -vector.x, y: -vector.y)\n"
            L"}"
            L"let positive = Vector2D(x: 3.0, y: 4.0)\n"
            L"let negative = -positive");

            dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr negative = scope->lookup(L"negative");
    ASSERT_NOT_NULL(negative);
    TypePtr Vector2D = dynamic_pointer_cast<Type>(scope->lookup(L"Vector2D"));
    ASSERT_NOT_NULL(Vector2D);
    ASSERT_EQ(Vector2D, negative->getType());
}


TEST(TestOperators, EquivalenceOperators)
{

    SEMANTIC_ANALYZE(L"struct Vector2D {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"func == (left: Vector2D, right: Vector2D) -> Bool {\n"
            L"    return (left.x == right.x) && (left.y == right.y)\n"
            L"}\n"
            L"func != (left: Vector2D, right: Vector2D) -> Bool {\n"
            L"    return !(left == right)\n"
            L"}\n"
            L"let twoThree = Vector2D(x: 2.0, y: 3.0)\n"
            L"let anotherTwoThree = Vector2D(x: 2.0, y: 3.0)\n"
            L"if twoThree == anotherTwoThree {\n"
            L"    println(\"These two vectors are equivalent.\")\n"
            L"}");

    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestOperators, UnknownInfix)
{
    SEMANTIC_ANALYZE(L"var a = 3 +-+ 4");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_UNKNOWN_BINARY_OPERATOR_1, res.code);
}

TEST(TestOperators, UnknownPrefix)
{
    SEMANTIC_ANALYZE(L"var a = +-+4");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, res.code);
}

TEST(TestOperators, UnknownPostfix)
{
    SEMANTIC_ANALYZE(L"var a = +-+4");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, res.code);
}

TEST(TestOperators, IncorrectParameters)
{
    SEMANTIC_ANALYZE(L"func +(a : Int, b : Int, c : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_OPERATORS_MUST_HAVE_ONE_OR_TWO_ARGUMENTS, res.code);
}

TEST(TestOperators, IncorrectParameters2)
{
    SEMANTIC_ANALYZE(L"prefix func +(a : Int, b : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_REQUIRES_A_FUNCTION_WITH_ONE_ARGUMENT_1, res.code);
}

TEST(TestOperators, IncorrectParameters3)
{
    SEMANTIC_ANALYZE(L"postfix func +(a : Int, b : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_REQUIRES_A_FUNCTION_WITH_ONE_ARGUMENT_1, res.code);
}


TEST(TestOperators, NoPrefixOrPostfix)
{
    SEMANTIC_ANALYZE(L"func +(a : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_UNARY_OPERATOR_IMPLEMENTATION_MUST_HAVE_A_PREFIX_OR_POSTFIX_MODIFIER, res.code);
}


TEST(TestOperators, ImpWithoutDeclaration)
{
    SEMANTIC_ANALYZE(L"prefix func +++(a : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION, res.code);
}

TEST(TestOperators, ImpWithoutDeclaration2)
{
    SEMANTIC_ANALYZE(L"postfix func +++(a : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION, res.code);
}

TEST(TestOperators, ImpWithoutDeclaration3)
{
    SEMANTIC_ANALYZE(L"func +++(a : Int, b : Int) -> Int\n"
            L"{\n"
            L"    return 3;\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_OPERATOR_IMPLEMENTATION_WITHOUT_MATCHING_OPERATOR_DECLARATION, res.code);
}

TEST(TestOperators, CustomOperators)
{
    SEMANTIC_ANALYZE(L"struct Vector2D {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"func + (left: Vector2D, right: Vector2D) -> Vector2D {\n"
            L"    return Vector2D(x: left.x + right.x, y: left.y + right.y)\n"
            L"}\n"
            L"func += (inout left: Vector2D, right: Vector2D) {\n"
            L"    left = left + right\n"
            L"}\n"
            L"prefix operator +++ {}\n"
            L"prefix func +++ (inout vector: Vector2D) -> Vector2D {\n"
            L"    vector += vector\n"
            L"    return vector\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}


TEST(TestOperators, CustomOperatorsRedefined)
{
    SEMANTIC_ANALYZE(L"prefix operator +++ {}"
            L"prefix operator +++ {}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_OPERATOR_REDECLARED, res.code);
}


TEST(TestOperators, CustomOperatorsPrecedenceMustBeInteger)
{
    SEMANTIC_ANALYZE(L"infix operator +++ {precedence 100.3}"
            L"prefix operator +++ {}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_EXPECT_INTEGER_PRECEDENCE, res.code);
}


TEST(TestOperators, CustomOperatorsPrecedenceOutOfRange)
{
    SEMANTIC_ANALYZE(L"infix operator +++ {precedence 1003}"
            L"prefix operator +++ {}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_OPERATOR_PRECEDENCE_OUT_OF_RANGE, res.code);
}

TEST(TestOperators, OperatorMustBeDefinedAtFileScope)
{
    SEMANTIC_ANALYZE(L"class TestOperator\n"
            L"{\n"
            L"    infix operator +=+{}\n"
            L"}");
    ASSERT_EQ(1, compilerResults.numResults());
    auto res = compilerResults.getResult(0);
    ASSERT_EQ(Errors::E_A_MAY_ONLY_BE_DECLARED_AT_FILE_SCOPE_1, res.code);
}

TEST(TestOperators, CustomOperators2)
{
    SEMANTIC_ANALYZE(L"struct Vector2D {\n"
            L"    var x = 0.0, y = 0.0\n"
            L"}\n"
            L"func + (left: Vector2D, right: Vector2D) -> Vector2D {\n"
            L"    return Vector2D(x: left.x + right.x, y: left.y + right.y)\n"
            L"}\n"
            L"func += (inout left: Vector2D, right: Vector2D) {\n"
            L"    left = left + right\n"
            L"}\n"
            L"prefix operator +++ {}\n"
            L"prefix func +++ (inout vector: Vector2D) -> Vector2D {\n"
            L"    vector += vector\n"
            L"    return vector\n"
            L"}\n"
            L"infix operator +- { associativity left precedence 140 }\n"
            L"func +- (left: Vector2D, right: Vector2D) -> Vector2D {\n"
            L"    return Vector2D(x: left.x + right.x, y: left.y - right.y)\n"
            L"}\n"
            L"let firstVector = Vector2D(x: 1.0, y: 2.0)\n"
            L"let secondVector = Vector2D(x: 3.0, y: 4.0)\n"
            L"let plusMinusVector = firstVector +- secondVector");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr plusMinusVector = scope->lookup(L"plusMinusVector");
    ASSERT_EQ(L"Vector2D", plusMinusVector->getType()->toString());
}

TEST(TestOperators, Precedence)
{
    SEMANTIC_ANALYZE(L"var a = 3 + 4 * 5");
    //Without precedence it will be parsed into (* (+ 3 4) 5)
    //we need to make sure it is (+ 3 (* 4 5))
    ASSERT_EQ(0, compilerResults.numResults());
    ValueBindingsPtr bindings = dynamic_pointer_cast<ValueBindings>(root->getStatement(0));
    ASSERT_NOT_NULL(bindings);
    ValueBindingPtr a = bindings->get(0);
    BinaryOperatorPtr op = dynamic_pointer_cast<BinaryOperator>(a->getInitializer());
    ASSERT_NOT_NULL(op);
    ASSERT_EQ(L"+", op->getOperator());
}

TEST(TestOperators, Precedence2)
{
    SEMANTIC_ANALYZE(L"var a : Int = 0\n"
            L"a = 3 + 4 * 5");
    //Without precedence it will be parsed into (* (+ 3 4) 5)
    //we need to make sure it is (+ 3 (* 4 5))
    ASSERT_EQ(0, compilerResults.numResults());
    AssignmentPtr assignment = dynamic_pointer_cast<Assignment>(root->getStatement(1));
    ASSERT_NOT_NULL(assignment);
    BinaryOperatorPtr op = dynamic_pointer_cast<BinaryOperator>(assignment->getRHS());
    ASSERT_NOT_NULL(op);
    ASSERT_EQ(L"+", op->getOperator());
}
