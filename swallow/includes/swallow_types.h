/* swallow_types.h --
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
#ifndef SWALLOW_TYPES_H
#define SWALLOW_TYPES_H
#include "swallow_conf.h"

SWALLOW_NS_BEGIN

struct TokenType
{
    enum T
    {
        _,
        EndOfFile,
        Identifier,
        String,
        Integer,
        Float,
        Comment,
        
        Optional, // ?
        Dot, // .
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
        Internal,
        Let,
        Operator,
        Private,
        Protocol,
        Public,
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
        False,
        Is,
        Nil,
        New,
        Super,
        Self,
        SelfU,
        True,
        Type,
        Column, // __COLUMN__
        File, // __FILE__
        Function,//__FUNCTION__
        Line, //__LINE__.
        
        //Keywords reserved in particular contexts, Outside the context in which they appear in the grammar, they can be used as identifiers.
        Associativity,
        Convenience,
        Dynamic,
        DidSet,
        Final,
        Get,
        Infix,
        Inout,
        Lazy,
        Left,
        Mutating,
        None,
        Nonmutating,
        Optional,
        Override,
        Postfix,
        Precedence,
        Prefix,
        Protocol_Reserved,
        Required,
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

struct DeclarationModifiers
{
    enum T
    {
        Class         =        1 << 0,
        Convenience   =        1 << 1,
        Dynamic       =        1 << 2,
        Final         =        1 << 3,
        Infix         =     1 << 4,
        Lazy          =     1 << 5,
        Mutating      =     1 << 6,
        NonMutating   =     1 << 7,
        Optional      =    1 << 8,
        Override      =    1 << 9,
        Postfix       =    1 << 10,
        Prefix        =    1 << 11,
        Required      =   1 << 12,
        Static        =   1 << 13,
        Unowned       =   1 << 14,
        Unowned_Safe  =   (1 << 15) | Unowned,
        Unowned_Unsafe=   (1 << 16) | Unowned,
        Weak          =  1 << 17,
        //Access Level Modifiers
        Internal      =  1 << 18,
        Internal_Set  =  Internal | (1 << 19),
        Private       = 1 << 20,
        Private_Set   = Private | (1 << 21),
        Public        = 1 << 22,
        Public_Set    = Public | (1 << 23),

        //The node is generated by compiler
        _Generated    = 1 << 30
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
