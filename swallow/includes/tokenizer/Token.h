/* Token.h --
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
#ifndef TOKEN_H
#define TOKEN_H
#include "swallow_types.h"
#include <vector>
#include <cstring>
#include <string>
#include <wchar.h>

SWALLOW_NS_BEGIN


/*!
 * Swift's tokenizer is context-sensitive, some keywords only appears in special context, parser need to tell
 * tokenizer about context
 */
enum TokenizerContext
{
    TokenizerContextUnknown,
    TokenizerContextFile = 1,
    TokenizerContextOperator = 2,
    TokenizerContextClass = 4,
    TokenizerContextComputedProperty = 8,
    TokenizerContextFunctionSignature = 0x10,
    TokenizerContextCaptureList = 0x20,
    TokenizerContextFunctionBody = 0x40,
    TokenizerContextSIL = 0x80,
    /*!
     * In type context, '>' will not be concatenated with following operator characters
     */
    TokenizerContextType = 0x100,
    TokenizerContextDeclaration = TokenizerContextFile | TokenizerContextClass | TokenizerContextFunctionBody,
    TokenizerContextAll = TokenizerContextFile | TokenizerContextOperator | TokenizerContextClass | TokenizerContextComputedProperty | TokenizerContextFunctionSignature | TokenizerContextCaptureList,
};
struct TokenizerState : SourceInfo
{
    int cursor;
    bool hasSpace;
    int inStringExpression;
    TokenizerContext context;
};

struct Token
{
    //Operators are made up of one or more of the following characters: /, =, -, +, !, *, %, <, >, &, |, ^, ~, and .. That said, the tokens =, ->, //, /*, */, ., and the unary prefix operator & are reserved. 
    union
    {
        struct
        {
            bool multiline;
            int nestedLevels;
        } comment;
        struct
        {
            KeywordType::T type;
            Keyword::T keyword;
            bool backtick;
            bool implicitParameterName;
        } identifier;
        struct
        {
            bool expressionFollowed;
        } string;
        struct
        {
            int base;
            bool sign;
            int fraction;
            int exponent;

            int64_t value;
            double dvalue;
        } number;
        struct
        {
            OperatorType::T type;
        } operators;
    };
    TokenType::T type;
    std::wstring token;
    size_t size;
    TokenizerState state;
    void append(wchar_t ch)
    {
        token.push_back(ch);
        size++;
    }
    Keyword::T getKeyword() const
    {
        if(type != TokenType::Identifier)
            return Keyword::_;
        return identifier.keyword;
    }
    
    bool operator ==(const wchar_t*str) const
    {
        return wcscmp(token.c_str(), str) == 0;
    }
    bool operator ==(TokenType::T type) const
    {
        return this->type == type;
    }
};

SWALLOW_NS_END
#endif//TOKEN_H
