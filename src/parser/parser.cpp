#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
using namespace Swift;

Parser::Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry)
    :nodeFactory(nodeFactory), symbolRegistry(symbolRegistry)
{
    tokenizer = new Tokenizer(NULL);
    functionName = L"<top>";
    flags = 0;
}
Parser::~Parser()
{
    delete tokenizer;
}

void Parser::setFileName(const wchar_t* fileName)
{
    this->fileName = fileName;
}
void Parser::setFunctionName(const wchar_t* function)
{
    this->functionName = functionName;
}
/**
 * Read next token from tokenizer, throw exception if EOF reached.
 */
void Parser::next(Token& token)
{
    if(tokenizer->next(token))
        return;
    throw 0;
}
/**
 * Peek next token from tokenizer, return false if EOF reached.
 */
bool Parser::peek(Token& token)
{
    token.type = TokenType::_;
    return tokenizer->peek(token);
}
/**
 * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
 */
bool Parser::match(const wchar_t* token)
{
    Token t;
    if(!tokenizer->next(t))
        return false;
    if(t == token)
        return true;
    tokenizer->restore(t);
    return false;
}
bool Parser::match(Keyword::T keyword)
{
    Token t;
    if(!tokenizer->next(t))
        return false;
    if(t.getKeyword() == keyword)
        return true;
    tokenizer->restore(t);
    return false;
}
/**
 * Check if the following token is an identifier(keywords included), consume the token and return true if matched or rollback and return false
 */
bool Parser::match_identifier(Token& token)
{
    if(!tokenizer->next(token))
        return false;
    if(token.type == TokenType::Identifier)
        return true;
    tokenizer->restore(token);
    return false;
}
/**
 * Return true if the next token is the specified one, no token will be consumed
 */
bool Parser::predicate(const wchar_t* token)
{
    Token t;
    if(!peek(t))
        return false;
    return t == token;
}
void Parser::expect(const wchar_t* str)
{
    Token token;
    next(token);
    tassert(token, token == str);
}
void Parser::expect(Keyword::T keyword)
{
    Token token;
    next(token);
    tassert(token, token.type == TokenType::Identifier);
    tassert(token, token.identifier.keyword == keyword);
}
void Parser::expect_identifier(Token& token)
{
    next(token);
    tassert(token, token.type == TokenType::Identifier);
    tassert(token, token.identifier.keyword == Keyword::_);
}

/**
 * Throw an exception with the unexpected token
 */
void Parser::unexpected(const Token& token)
{
    throw 0;    
}
void Parser::tassert(Token& token, bool cond)
{
    if(!cond)
        throw 0;
}

Node* Parser::parse(const wchar_t* code)
{
    tokenizer->set(code);
    return parseStatement();
}
