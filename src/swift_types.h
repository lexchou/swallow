#ifndef SWIFT_TYPES_H
#define SWIFT_TYPES_H
#include "swift_conf.h"

SWIFT_NS_BEGIN

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
        Colon, // :
        Comma, // ,
        Operator,
        OpenParen, // (
        CloseParen, // )
        OpenBracket, // [
        CloseBracket, // ]
        OpenBrace, // {
        CloseBrace, // }
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
        PrefixUnary,
        InfixBinary,
        PostfixUnary,
        Ternary
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


SWIFT_NS_END

#endif//SWIFT_TYPES_H
