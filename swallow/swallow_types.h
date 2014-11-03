/* swallow_types.h --
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
#ifndef SWALLOW_TYPES_H
#define SWALLOW_TYPES_H
#include "swallow_conf.h"

SWALLOW_NS_BEGIN

struct TokenType
{
    enum T
    {
        _,
        Identifier,
        String,
        Integer,
        Float,
        Comment,
        
        Optional, // ?
        Attribute, // @
        Sharp, // #
        Colon, // :
        Comma, // ,
        Semicolon, // ;
        Operator,
        OpenParen, // (
        CloseParen, // )
        OpenBracket, // [
        CloseBracket, // ]
        OpenBrace, // {
        CloseBrace // }
    };
};
struct IntegerPrefix
{
    enum T
    {
        Decimal,
        Binary,
        Octet,
        Hexadecimal
    };
};
struct Keyword
{
    enum T
    {
        _,
        //keywords used in declarations
        Class,
        Deinit,
        Enum,
        Extension,
        Func,
        Import,
        Init,
        Let,
        Protocol,
        Static,
        Struct,
        Subscript,
        Typealias,
        Var,
        //Keywords used in statements
        Break,
        Case,
        Continue,
        Default,
        Do,
        Else,
        Fallthrough,
        If,
        In,
        For,
        Return,
        Switch,
        Where,
        While,
        
        //Keywords used in expressions and types:
        As,
        DynamicType,
        Is,
        New,
        Super,
        Self,
        SelfU,
        Type,
        Column, // __COLUMN__
        File, // __FILE__
        Function,//__FUNCTION__
        Line, //__LINE__.
        
        //Keywords reserved in particular contexts, Outside the context in which they appear in the grammar, they can be used as identifiers.
        Associativity,
        Convenience,
        DidSet,
        Get,
        Infix,
        Inout,
        Left,
        Mutating,
        None,
        Nonmutating,
        Operator,
        Override,
        Postfix,
        Precedence,
        Prefix,
        Right,
        Set,
        Unowned,
        Unowned_safe,
        Unowned_unsafe,
        Weak,
        WillSet
    };
};
struct KeywordType
{
    enum T
    {
        _,
        Declaration,
        Statement,
        Expression,
        Reserved
    };
};
struct OperatorType
{
    enum T
    {
        _,
        PrefixUnary = 1,
        InfixBinary = 2,
        PostfixUnary = 4,
        Ternary = 8
    };
};

struct Associativity
{
    enum T
    {
        None,
        Left,
        Right
    };
};

struct TypeSpecifier
{
    enum T
    {
        Class         =   1,
        Mutating      =   2,
        NonMutating   =   4,
        Override      =   8,
        Static        =   0x10,
        Unowned       =   0x20,
        Unowned_Safe  =   0x20 | 0x40,
        Unowned_Unsafe=   0x20 | 0x80,
        Weak          =   0x100
    };
};


struct SourceInfo
{
    int fileHash;
    int line;
    int column;
    SourceInfo()
    :fileHash(0), line(0), column(0)
    {}
};

struct Abort
{

};


SWALLOW_NS_END

#endif//SWALLOW_TYPES_H
