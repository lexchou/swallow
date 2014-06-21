#ifndef TOKEN_H
#define TOKEN_H
#include "swift_conf.h"
#include <vector>
#include <cstring>
#include <wchar.h>

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
        PostfixUnary
    };
};

struct TokenizerState
{

    const wchar_t* cursor;
    bool hasSpace;
    bool inStringExpression;
    int line;
    int column;
};

struct Token
{
    //“Operators are made up of one or more of the following characters: /, =, -, +, !, *, %, <, >, &, |, ^, ~, and .. That said, the tokens =, ->, //, /*, */, ., and the unary prefix operator & are reserved. ”
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
            bool fraction;
            bool exponent;
        } number;
        struct
        {
            OperatorType::T type;
        } operators;
    };
    TokenType::T type;
    std::vector<wchar_t> token;
    size_t size;
    TokenizerState state;
    void append(wchar_t ch)
    {
        token.push_back(ch);
        size++;
    }
    const wchar_t* c_str()
    {
        if(token.empty() || token.back() != '\0')
            token.push_back(0);
        return &token.front();
    }
    bool operator ==(const wchar_t*str)
    {
        return !wcscmp(c_str(), str);
    }
};

SWIFT_NS_END
#endif//TOKEN_H
