/* TestEnumeration.cpp --
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

TEST(TestEnumeration, Declaration)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(Type::Enum, CompassPoint->getCategory());

}

TEST(TestEnumeration, CaseAccess)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}\n"
            L"var a = CompassPoint.East;");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a;
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(CompassPoint, a->getType());

}

TEST(TestEnumeration, CaseAccess_ShorterSyntax)
{
    SEMANTIC_ANALYZE(L"enum CompassPoint { \n"
            L"case North\n"
            L"case South\n"
            L"case East\n"
            L"case West\n"
            L"}\n"
            L"var a : CompassPoint;\n"
            L"a = .North");
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
    SymbolPtr a;
    TypePtr CompassPoint;
    ASSERT_NOT_NULL(a = scope->lookup(L"a"));
    ASSERT_NOT_NULL(CompassPoint = dynamic_pointer_cast<Type>(scope->lookup(L"CompassPoint")));
    ASSERT_EQ(CompassPoint, a->getType());

}
