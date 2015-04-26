/* Parser.cpp --
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
#include "parser/Parser.h"
#include "tokenizer/Tokenizer.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
#include "common/CompilerResults.h"
#include "common/Errors.h"
#include <memory>
using namespace Swallow;


Parser::Parser(NodeFactory* nodeFactory, CompilerResults* compilerResults)
    :nodeFactory(nodeFactory), compilerResults(compilerResults)
{
    tokenizer = new Tokenizer(NULL);
    functionName = L"<top>";
    sourceFile = SourceFilePtr(new SourceFile());
    sourceFile->fileName = L"<code>";
    flags = 0;
}
Parser::~Parser()
{
    delete tokenizer;
}

void Parser::setSourceFile(const SourceFilePtr& sourceFile)
{
    this->sourceFile = sourceFile;
    tokenizer->setSourceFile(sourceFile);
}
void Parser::setFunctionName(const wchar_t* function)
{
    this->functionName = functionName;
}
/*!
 * Read next token from tokenizer, throw exception if EOF reached.
 */
void Parser::expect_next(Token& token)
{
    if(next(token))
        return;
    compilerResults->add(ErrorLevel::Fatal, token.state, Errors::E_UNEXPECTED_EOF);
    throw Abort();
}
/*!
 * Peek next token from tokenizer, return false if EOF reached.
 */
bool Parser::peek(Token& token)
{
    token.type = TokenType::_;
    try
    {
        while (tokenizer->next(token))
        {
            if (token.type == TokenType::Comment)
                continue;
            tokenizer->restore(token);
            return true;
        }
        return false;
    }
    catch(const TokenizerError& e)
    {
        return false;
    }
}
/*!
 * Read next token from tokenizer, return false if EOF reached.
 */
bool Parser::next(Token& token)
{
    try
    {
        while (tokenizer->next(token))
        {
            if (token.type == TokenType::Comment)
                continue;
            return true;
        }
        //eof reached, fill token with end-of-file for compiler error
        token.token = L"end-of-file";
        return false;
    }
    catch(const TokenizerError& e)
    {
        token.state.line = e.line;
        token.state.column = e.column;
        token.state.sourceFile = sourceFile;
        tassert(token, false, e.errorCode, e.item);
        return false;
    }
}
/*!
 * Restore the position of tokenizer to specified token
 */
void Parser::restore(Token& token)
{
    tokenizer->restore(token);
}
/*!
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
/*!
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
/*!
 * Return true if the next token is the specified one, no token will be consumed
 */
bool Parser::predicate(const wchar_t* token)
{
    Token t;
    if(!peek(t))
        return false;
    return t == token;
}
bool Parser::predicate(const wchar_t* token, Token& t)
{
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
    if(next(token))
    {
        if(token == str)
            return;
    }
    ResultItems items = {str, token.token};
    error(token, Errors::E_EXPECT_2, items);
}

void Parser::expect(Keyword::T keyword)
{
    Token token;
    expect(keyword, token);
}

void Parser::expect(Keyword::T keyword, Token& token)
{
    const std::wstring& str = tokenizer->getKeyword(keyword);
    if(next(token) && token.getKeyword() == keyword)
        return;
    ResultItems items = {str, token.token};
    error(token, Errors::E_EXPECT_KEYWORD_2, items);
}
void Parser::expect_identifier(Token& token)
{
    if(!next(token))
    {
        ResultItems items = {L"end-of-file"};
        error(token, Errors::E_EXPECT_IDENTIFIER_1, items);
    }
    if(token.type != TokenType::Identifier || token.identifier.keyword != Keyword::_)
    {
        ResultItems items = {token.token};
        error(token, Errors::E_EXPECT_IDENTIFIER_1, items);
    }
}

/*!
 * Throw an exception with the unexpected token
 */
void Parser::unexpected(Token& token)
{
    token.state.sourceFile = sourceFile;
    compilerResults->add(ErrorLevel::Fatal, token.state, Errors::E_UNEXPECTED_1, token.token);
    throw Abort();
}
void Parser::tassert(Token& token, bool cond, int errorCode)
{
    if(cond)
        return;
    ResultItems items;
    error(token, errorCode, items);
}
void Parser::tassert(Token& token, bool cond, int errorCode, const std::wstring& s)
{
    if(cond)
        return;
    ResultItems items = {s};
    error(token, errorCode, items);
}
void Parser::error(Token& token, int errorCode, const std::vector<std::wstring>& s)
{
    token.state.sourceFile = sourceFile;
    compilerResults->add(ErrorLevel::Fatal, token.state, errorCode, s);
    throw Abort();
}

NodePtr Parser::parseStatement(const wchar_t* code)
{
    tokenizer->set(code);
    NodePtr ret = NULL;
    try
    {
        ret = parseStatement();
    }
    catch(...)
    {
        //clean up all nodes created during the parsing
        ret = NULL;
    }
    return ret;
}

ProgramPtr Parser::parse(const wchar_t* code)
{
    ProgramPtr ret = nodeFactory->createProgram();
    if(parse(code, ret))
    {
        return ret;
    }
    return nullptr;
}
bool Parser::parse(const wchar_t* code, const ProgramPtr& program)
{
    tokenizer->set(code);
    try
    {
        Token token;
        while(peek(token))
        {
            StatementPtr statement = parseStatement();
            if(!statement)
                break;
            program->addStatement(statement);
            match(L";");
        }
    }
    catch(...)
    {
        return false;
    }
    return true;
}
