/* Tokenizer.h --
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
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "Token.h"
#include <cstring>
#include <string>
#include <map>

SWALLOW_NS_BEGIN

struct TokenizerError
{
    int line;
    int column;
    int errorCode;
    std::wstring item;
};
struct KeywordInfo
{
    Keyword::T keyword;
    KeywordType::T type;
};

class SWALLOW_EXPORT Tokenizer
{
public:
    Tokenizer(const wchar_t* data);
    ~Tokenizer();
public:
    void set(const wchar_t* data);
    bool next(Token& token);
    bool peek(Token& token);

    const std::wstring& getKeyword(Keyword::T k);

    /**
     * Save current state for restoring later
     */
    const TokenizerState& save();
    /**
     * Restore the specified state
     */
    void restore(const TokenizerState& state);
    /**
     * Restore the state that used to parse given token
     */
    void restore(const Token& token);
private:
    bool nextImpl(Token& token);
    void resetToken(Token& token);
    bool skipSpaces();
    bool get(wchar_t &ch);
    void unget();
    bool peek(wchar_t &ch);
    
    wchar_t must_get();
    void match(wchar_t ch);
    
    
    bool hasWhiteLeft(const wchar_t* cursor);
    bool hasWhiteRight(const wchar_t* cursor);
    OperatorType::T calculateOperatorType(const wchar_t* begin, const wchar_t* end);
    
    bool readSymbol(Token& token, TokenType::T type);
    bool readOperator(Token& token, bool dotOperator, int max);
    bool readComment(Token& token);
    bool readMultilineComment(Token& token);
    bool readString(Token& token);
    bool readNumber(Token& token);
    bool readNumberLiteral(Token& token, int base, int64_t& out);
    bool readFraction(Token& token, int base, double& out);
    bool readIdentifier(Token& token);
private:
    void error(int errorCode, const std::wstring& str = L"");
private:
    wchar_t* data;
    const wchar_t* end;
    size_t size;
    TokenizerState state;
    int positions[16];
    std::map<std::wstring, KeywordInfo> keywords;
    std::map<Keyword::T, std::wstring> keywordNames;
};


SWALLOW_NS_END
#endif//TOKENIZER_H
