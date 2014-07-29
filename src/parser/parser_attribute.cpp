#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "semantics/symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
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
