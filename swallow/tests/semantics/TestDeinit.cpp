/* TestDeinit.cpp --
 *
 * Copyright (c) 2015, Lex Chou <lex at chou dot it>
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
#include "semantics/Type.h"
#include "common/Errors.h"
#include "semantics/GenericArgument.h"

using namespace Swallow;
using namespace std;

TEST(TestDeinit, Deinit)
{
    SEMANTIC_ANALYZE(L""
            L"func min(a : Int, b : Int) -> Int {\n"
            L"    if a < b { return a } else { return b}"
            L"}\n"
            L"struct Bank {\n"
            L"    static var coinsInBank = 10_000\n"
            L"    static func vendCoins(var numberOfCoinsToVend: Int) -> Int {\n"
            L"        numberOfCoinsToVend = min(numberOfCoinsToVend, coinsInBank)\n"
            L"        coinsInBank = coinsInBank - numberOfCoinsToVend\n"
            L"        return numberOfCoinsToVend\n"
            L"    }\n"
            L"    static func receiveCoins(coins: Int) {\n"
            L"        coinsInBank = coinsInBank + coins\n"
            L"    }\n"
            L"}\n"
            L"class Player {\n"
            L"    var coinsInPurse: Int\n"
            L"    init(coins: Int) {\n"
            L"        coinsInPurse = Bank.vendCoins(coins)\n"
            L"    }\n"
            L"    func winCoins(coins: Int) {\n"
            L"        coinsInPurse = coinsInPurse + Bank.vendCoins(coins)\n"
            L"    }\n"
            L"    deinit {\n"
            L"        Bank.receiveCoins(coinsInPurse)\n"
            L"    }\n"
            L"}");
    ASSERT_NO_ERRORS();
}
