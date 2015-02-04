/* TestSubscripts.cpp --
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
#include "semantics/Symbol.h"
#include "semantics/ScopedNodes.h"
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GenericArgument.h"

using namespace Swallow;
using namespace std;

TEST(TestSubscripts, Subscript)
{
    SEMANTIC_ANALYZE(L"struct TimesTable {\n"
        L"    let multiplier: Int\n"
        L"    subscript(index: Int) -> Int {\n"
        L"        return multiplier * index\n"
        L"    }\n"
        L"}\n"
        L"let threeTimesTable = TimesTable(multiplier: 3)\n"
        L"println(\"six times three is \\(threeTimesTable[6])\")");
    ASSERT_NO_ERRORS();
}

TEST(TestSubscripts, SubscriptOptions)
{
    SEMANTIC_ANALYZE(L"struct Matrix {\n"
        L"    let rows: Int, columns: Int\n"
        L"    var grid: [Double]\n"
        L"    init(rows: Int, columns: Int) {\n"
        L"        self.rows = rows\n"
        L"        self.columns = columns\n"
//        L"        grid = Array(count: rows * columns, repeatedValue: 0.0)\n"
        L"        grid = []\n"
        L"    }\n"
        L"    func indexIsValidForRow(row: Int, column: Int) -> Bool {\n"
        L"        return row >= 0 && row < rows && column >= 0 && column < columns\n"
        L"    }\n"
        L"    subscript(row: Int, column: Int) -> Double {\n"
        L"        get {\n"
        L"            assert(indexIsValidForRow(row, column), \"Index out of range\")\n"
        L"            return grid[(row * columns) + column]\n"
        L"        }\n"
        L"        set {\n"
        L"            assert(indexIsValidForRow(row, column), \"Index out of range\")\n"
        L"            grid[(row * columns) + column] = newValue\n"
        L"        }\n"
        L"    }\n"
        L"}");
    ASSERT_NO_ERRORS();
}

