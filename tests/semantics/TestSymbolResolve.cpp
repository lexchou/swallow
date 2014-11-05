/* TestSymbolResolve.cpp --
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
#include "semantics/FunctionSymbol.h"

using namespace Swallow;

TEST(TestSymbolResolve, testUndeclaredVars)
{
    SEMANTIC_ANALYZE(L"a = 34");
    //dumpCompilerResults(compilerResults);
    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_USE_OF_UNRESOLVED_IDENTIFIER_1, r.code);
    ASSERT_EQ(L"a", r.items[0]);
}

TEST(TestSymbolResolve, testUseConstantBeforeDeclaration)
{
    SEMANTIC_ANALYZE(L"let a = b, b = 1;");
    ASSERT_EQ(1, compilerResults.numResults());

    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_USE_OF_UNINITIALIZED_VARIABLE_1, r.code);
    ASSERT_EQ(L"b", r.items[0]);

}

TEST(TestSymbolResolve, testVariables)
{
    SEMANTIC_ANALYZE(L"var a : Int = 1;");
    ASSERT_EQ(0, compilerResults.numResults());


}

TEST(TestSymbolResolve, testVariableUsedWithinItsOwnInitialization)
{
    SEMANTIC_ANALYZE(L"let a = a + 1;");
    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_USE_OF_INITIALIZING_VARIABLE, r.code);
    ASSERT_EQ(L"a", r.items[0]);

}

TEST(TestSymbolResolve, testDuplicatedVars)
{
    SEMANTIC_ANALYZE(L"let fa = 3;var fa = 3")

    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_DEFINITION_CONFLICT, r.code);
    ASSERT_EQ(L"fa", r.items[0]);
}

TEST(TestSymbolResolve, testLocalGlobal)
{
    SEMANTIC_ANALYZE(L"let global = 0; func test(){let local = 1;}");
    CodeBlockPtr test = NULL;
    FunctionSymbolPtr symbol;
    ASSERT_NOT_NULL(symbol = std::dynamic_pointer_cast<FunctionSymbol>(scope->lookup(L"test")));
    ASSERT_NOT_NULL(test = symbol->getDefinition());
    ScopedCodeBlockPtr cb = std::static_pointer_cast<ScopedCodeBlock>(test);
    SymbolPtr local = cb->getScope()->lookup(L"local");
    SymbolPtr global = root->getScope()->lookup(L"global");
    ASSERT_TRUE(local != NULL);
    ASSERT_TRUE(global != NULL);
}
TEST(TestSymbolResolve, testLocalTypeUsesUpperLocalSymbol)
{
    const wchar_t* code = L"func test() -> Int\n"
    L"{\n"
    L"    let b = 11\n"
    L"    struct TTT\n"
    L"    {\n"
    L"        let i = b\n"
    L"    }\n"
    L"    let i = TTT()\n"
    L"    return i.i;\n"
    L"\n"
    L"}\n";
    SEMANTIC_ANALYZE(code);
    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_USE_OF_FUNCTION_LOCAL_INSIDE_TYPE, r.code);
    ASSERT_EQ(L"b", r.items[0]);
}
TEST(TestSymbolResolve, testClassVariable1)
{
    const wchar_t* code =
            L"class Class\n"
            L"{\n"
            L"    var _val : Int = 0\n"
            L"    var val : Int = 0 {\n"
            L"        willSet(newVal) {\n"
            L"            _val = newVal;\n"
            L"        }\n"
            L"        didSet {\n"
            L"            println(oldValue);\n"
            L"        }\n"
            L"    }\n"
            L"}";
    SEMANTIC_ANALYZE(code);
    dumpCompilerResults(compilerResults);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestSymbolResolve, testClassVariable2)
{
    const wchar_t* code =
            L"class Class\n"
    L"{\n"
    L"    var _val : Int = 0\n"
    L"    var val : Int = 0 {\n"
    L"        willSet {\n"
    L"            _val = newValue;\n"
    L"        }\n"
    L"        didSet(old) {\n"
    L"            println(old);\n"
    L"        }\n"
    L"    }\n"
    L"}";
    SEMANTIC_ANALYZE(code);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestSymbolResolve, testClassVariable3)
{
    const wchar_t* code =
            L"class Class\n"
    L"{\n"
    L"    var _val : Int = 0\n"
    L"    var val : Int = 0 {\n"
    L"        get{\n"
    L"            return _val;\n"
    L"        }\n"
    L"        set {\n"
    L"            _val = newValue;\n"
    L"        }\n"
    L"    }\n"
    L"}";
    SEMANTIC_ANALYZE(code);
    ASSERT_EQ(0, compilerResults.numResults());
}

TEST(TestSymbolResolve, testStructureInit)
{
    const wchar_t* code =
            L"struct Test\n"
            L"{\n"
            L"    init(a : Int, a : Int)\n"
            L"    {\n"
            L"        \n"
            L"    }\n"
            L"}";
    SEMANTIC_ANALYZE(code);
    ASSERT_EQ(1, compilerResults.numResults());
    const CompilerResult& r = compilerResults.getResult(0);
    ASSERT_EQ((int)Errors::E_DEFINITION_CONFLICT, r.code);
    ASSERT_EQ(L"a", r.items[0]);
}

