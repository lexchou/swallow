/* parser_attribute.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot com>
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
#include "Parser.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"

using namespace Swift;

void Parser::parseAttributes(std::vector<AttributePtr>& attributes)
{
    attributes.clear();
    while(predicate(L"@"))
    {
        AttributePtr attr = parseAttribute();
        attributes.push_back(attr);
    }
}
/*
  GRAMMAR OF AN ATTRIBUTE
 
 ‌ attribute → @attribute-nameattribute-argument-clauseopt
 ‌ attribute-name → identifier
 ‌ attribute-argument-clause → (balanced-tokensopt)
 ‌ attributes → attributeattributesopt
 ‌ balanced-tokens → balanced-tokenbalanced-tokensopt
 ‌ balanced-token → (balanced-tokensopt)
 ‌ balanced-token → [balanced-tokensopt]
 ‌ balanced-token → {balanced-tokensopt}
 ‌ balanced-token → Any identifier, keyword, literal, or operator
 ‌ balanced-token → Any punctuation except (, ), [, ], {, or }
*/
AttributePtr Parser::parseAttribute()
{
    Token token;
    expect(L"@", token);
    AttributePtr ret = nodeFactory->createAttribute(token.state);
    expect_identifier(token);
    ret->setName(token.token);
    if(match(L"("))
    {
        //read
        parseBalancedTokens(ret, L")");
    }
    return ret;
}
void Parser::parseBalancedToken(const AttributePtr& attr)
{
    Token token;
    expect_next(token);
    switch(token.type)
    {
        case TokenType::OpenBrace:
            parseBalancedTokens(attr, L"}");
            return;
        case TokenType::OpenBracket:
            parseBalancedTokens(attr, L"]");
            return;
        case TokenType::OpenParen:
            parseBalancedTokens(attr, L")");
            return;
        default:
            attr->addBalancedToken(token.token);
            return;
    }
}
void Parser::parseBalancedTokens(const AttributePtr& attr, const wchar_t* end)
{
    while(!predicate(end))
    {
        parseBalancedToken(attr);
    }
    expect(end);
}
