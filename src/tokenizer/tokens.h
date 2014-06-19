#ifndef TOKEN_H
#define TOKEN_H
#include <vector>

struct TokenType
{
    enum T
    {
        None,
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
        OpenBracket, // (
        CloseBracket, // )
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
        None,
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
        None,
        PrefixUnary,
        InfixBinary,
        PostfixUnary
    };
};

struct Token
{
    //“Operators are made up of one or more of the following characters: /, =, -, +, !, *, %, <, >, &, |, ^, ~, and .. That said, the tokens =, ->, //, /*, */, ., and the unary prefix operator & are reserved. ”
    union
    {
        
        bool customOperator;
        struct
        {
            bool multiline;
            int nestedLevels;
        } comment;
        struct
        {
            KeywordType type;
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
    const wchar_t* c_str()
    {
        return &token.front();
    }
};

#endif//TOKEN_H
