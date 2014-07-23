#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "semantics/symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include "common/compiler_results.h"
#include "common/auto-release-pool.h"
#include "swift_errors.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
using namespace Swift;

struct Interrupt
{

};


Parser::Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry, CompilerResults* compilerResults)
    :nodeFactory(nodeFactory), symbolRegistry(symbolRegistry), compilerResults(compilerResults)
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
void Parser::expect_next(Token& token)
{
    if(next(token))
        return;
    compilerResults->add(ErrorLevel::Fatal, token.state, Errors::E_UNEXPECTED_EOF);
    throw Interrupt();
}
/**
 * Peek next token from tokenizer, return false if EOF reached.
 */
bool Parser::peek(Token& token)
{
    token.type = TokenType::_;
    while(tokenizer->next(token))
    {
        if(token.type == TokenType::Comment)
            continue;
        tokenizer->restore(token);
        return true;
    }
    return false;
}
/**
 * Read next token from tokenizer, return false if EOF reached.
 */
bool Parser::next(Token& token)
{
    while(tokenizer->next(token))
    {
        if(token.type == TokenType::Comment)
            continue;
        return true;
    }
    return false;
}
/**
 * Restore the position of tokenizer to specified token
 */
void Parser::restore(Token& token)
{
    tokenizer->restore(token);
}
/**
 * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
 */
bool Parser::match(const wchar_t* str, Token& token)
{
    if(!next(token))
        return false;
    if(token == str)
        return true;
    restore(token);
    return false;
}
bool Parser::match(Keyword::T keyword, Token& token)
{
    if(!next(token))
        return false;
    if(token.getKeyword() == keyword)
        return true;
    restore(token);
    return false;
}

bool Parser::match(const wchar_t* token)
{
    Token t;
    return match(token, t);
    
}
bool Parser::match(Keyword::T keyword)
{
    Token t;
    return match(keyword, t);
}
/**
 * Check if the following token is an identifier(keywords included), consume the token and return true if matched or rollback and return false
 */
bool Parser::match_identifier(Token& token)
{
    if(!next(token))
        return false;
    if(token.type == TokenType::Identifier)
        return true;
    restore(token);
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
    expect(str, token);
}
void Parser::expect(const wchar_t* str, Token& token)
{
    expect_next(token);
    tassert(token, token == str, Errors::E_EXPECT, str);
}

void Parser::expect(Keyword::T keyword)
{
    Token token;
    expect(keyword, token);
}

void Parser::expect(Keyword::T keyword, Token& token)
{
    expect_next(token);
    if(keyword == token.getKeyword())
        return;

    const std::wstring& str = tokenizer->getKeyword(keyword);
    tassert(token, false, Errors::E_EXPECT_KEYWORD, str);
}
void Parser::expect_identifier(Token& token)
{
    expect_next(token);
    tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_IDENTIFIER, token.token);
    tassert(token, token.identifier.keyword == Keyword::_, Errors::E_EXPECT_IDENTIFIER, token.token);
}

/**
 * Throw an exception with the unexpected token
 */
void Parser::unexpected(const Token& token)
{
    compilerResults->add(ErrorLevel::Fatal, token.state, Errors::E_UNEXPECTED, token.token);
    throw Interrupt();
}
void Parser::tassert(Token& token, bool cond, int errorCode, const std::wstring& s)
{
    if(cond)
        return;
    //record this issue
    compilerResults->add(ErrorLevel::Fatal, token.state, errorCode, s);
    throw Interrupt();
}

Node* Parser::parseStatement(const wchar_t* code)
{
    tokenizer->set(code);
    AutoReleasePool pool;
    nodeFactory->setAutoReleasePool(&pool);
    Node* ret = NULL;
    try
    {
        ret = parseStatement();
        pool.removeAll();
    }
    catch(...)
    {
        //clean up all nodes created during the parsing
        ret = NULL;
        nodeFactory->setAutoReleasePool(NULL);
    }
    return ret;
}

Program* Parser::parse(const wchar_t* code)
{
    tokenizer->set(code);
    AutoReleasePool pool;
    nodeFactory->setAutoReleasePool(&pool);
    Program* ret = nodeFactory->createProgram();
    try
    {
        Token token;
        while(peek(token))
        {
            Statement* statement = parseStatement();
            if(!statement)
                break;
            ret->addStatement(statement);
            match(L";");
        }
        pool.removeAll();
    }
    catch(...)
    {
        //clean up all nodes created during the parsing
        delete ret;
        ret = NULL;
        nodeFactory->setAutoReleasePool(NULL);
    }
    return ret;
}