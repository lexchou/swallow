#ifndef TOKEN_H
#define TOKEN_H
#include <vector>

enum TokenType
{
    kEOF,
    kIdentifier,
    kString,
    kInteger,
    kFloat,
    kComment,
    
    kOptional, // ?
    kAttribute, // @
    kColon, // :
    kComma, // ,
    kOperator,
    kOpenParen, // (
    kCloseParen, // )
    kOpenBracket, // (
    kCloseBracket, // )
    kOpenBrace, // {
    kCloseBrace, // }
    
    
};
enum IntegerPrefix
{
    kDecimal,
    kBinary,
    kOctet,
    kHexadecimal
};
enum Keyword
{
    //keywords used in declarations
    kClass,
    kDeinit,
    kEnum,
    kExtension,
    kFunc,
    kImport,
    kInit,
    kLet,
    kProtocol,
    kStatic,
    kStruct,
    kSubscript,
    kTypealias,
    kVar
};
enum KeywordType
{
    kNone,
    kDeclaration,
    kStatement,
    kExpression,
    kReserved
};

struct Token
{
    bool spaceBefore;
    bool spaceAfter;
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
            KeywordType keyword;
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
    };
    TokenType type;
    std::vector<wchar_t> token;
    const wchar_t* c_str()
    {
        return &token.front();
    }
};

#endif//TOKEN_H