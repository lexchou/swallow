/* Tokenizer.cpp --
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
#include "tokenizer/Tokenizer.h"
#include "tokenizer/token_char_types.h"
#include <cmath>
#include <cstring>
#include <wchar.h>
#include <ctype.h>
#include "common/Errors.h"
using namespace Swallow;

Tokenizer::Tokenizer(const wchar_t* data)
{
    this->data = NULL;
    set(data);
    
    //initialize keyword information
    KeywordType::T D = KeywordType::Declaration;
    KeywordType::T S = KeywordType::Statement;
    KeywordType::T E = KeywordType::Expression;
    KeywordType::T R = KeywordType::Reserved;
    
    struct {
        const wchar_t* name;
        KeywordType::T type;
        Keyword::T keyword;
        TokenizerContext context;
    } keywords[] = {
        //Declaration keywords
        {L"class",          D, Keyword::Class},
        {L"deinit",         D, Keyword::Deinit},
        {L"enum",           D, Keyword::Enum},
        {L"extension",      D, Keyword::Extension},
        {L"func",           D, Keyword::Func},
        {L"import",         D, Keyword::Import},
        {L"init",           D, Keyword::Init},
        {L"internal",       D, Keyword::Internal},
        {L"let",            D, Keyword::Let},
        {L"operator",       D, Keyword::Operator},
        {L"private",        D, Keyword::Private},
        {L"protocol",       D, Keyword::Protocol},
        {L"public",         D, Keyword::Public},
        {L"static",         D, Keyword::Static},
        {L"struct",         D, Keyword::Struct},
        {L"subscript",      D, Keyword::Subscript},
        {L"typealias",      D, Keyword::Typealias},
        {L"var",            D, Keyword::Var},
        //Statement keywords
        {L"break",          S, Keyword::Break},
        {L"case",           S, Keyword::Case},
        {L"continue",       S, Keyword::Continue},
        {L"default",        S, Keyword::Default},
        {L"do",             S, Keyword::Do},
        {L"else",           S, Keyword::Else},
        {L"fallthrough",    S, Keyword::Fallthrough},
        {L"for",            S, Keyword::For},
        {L"if",             S, Keyword::If},
        {L"in",             S, Keyword::In},
        {L"return",         S, Keyword::Return},
        {L"switch",         S, Keyword::Switch},
        {L"where",          S, Keyword::Where},
        {L"while",          S, Keyword::While},
        //Expression and type keywords
        {L"as",             E, Keyword::As},
        {L"dynamicType",    E, Keyword::DynamicType},
        {L"false",          E, Keyword::False},

        {L"is",             E, Keyword::Is},

        {L"nil",            E, Keyword::Nil},
        {L"new",            E, Keyword::New},
        {L"self",           E, Keyword::Self},
        {L"Self",           E, Keyword::SelfU},
        {L"super",          E, Keyword::Super},
        {L"true",           E, Keyword::True},

        {L"__COLUMN__",     E, Keyword::Column},
        {L"__FILE__",       E, Keyword::File},
        {L"__FUNCTION__",   E, Keyword::Function},
        {L"__LINE__",       E, Keyword::Line},
        //Reserved keywords
        {L"associativity",  R, Keyword::Associativity, TokenizerContextOperator},
        {L"convenience",    R, Keyword::Convenience, TokenizerContextClass},
        {L"dynamic",        R, Keyword::Dynamic, TokenizerContextClass},
        {L"didSet",         R, Keyword::DidSet, TokenizerContextComputedProperty},
        {L"final",          R, Keyword::Final, TokenizerContextDeclaration},
        {L"get",            R, Keyword::Get, TokenizerContextComputedProperty},
        {L"infix",          R, Keyword::Infix, TokenizerContextAll},
        {L"inout",          R, Keyword::Inout, TokenizerContextFunctionSignature},
        {L"lazy",           R, Keyword::Lazy, TokenizerContextDeclaration},
        {L"left",           R, Keyword::Left, TokenizerContextOperator},
        {L"mutating",       R, Keyword::Mutating, TokenizerContextDeclaration},
        {L"none",           R, Keyword::None, TokenizerContextOperator},
        {L"nonmutating",    R, Keyword::Nonmutating, TokenizerContextDeclaration},
        {L"optional",       R, Keyword::Optional, TokenizerContextClass},
        {L"override",       R, Keyword::Override, (TokenizerContext)(TokenizerContextClass | TokenizerContextFile)},
        {L"postfix",        R, Keyword::Postfix, TokenizerContextAll},
        {L"precedence",     R, Keyword::Precedence, TokenizerContextOperator},
        {L"prefix",         R, Keyword::Prefix, TokenizerContextAll},
        {L"Protocol",       R, Keyword::Protocol_Reserved, TokenizerContextUnknown}, //TODO: update the context here
        {L"required",       R, Keyword::Required, TokenizerContextClass},
        {L"right",          R, Keyword::Right, TokenizerContextOperator},
        {L"set",            R, Keyword::Set, TokenizerContextComputedProperty},
        {L"Type",           R, Keyword::Type, TokenizerContextUnknown}, //TODO: update the context according to its usage.
        {L"unowned",        R, Keyword::Unowned, (TokenizerContext)(TokenizerContextCaptureList | TokenizerContextClass)},
        {L"weak",           R, Keyword::Weak, (TokenizerContext)(TokenizerContextCaptureList | TokenizerContextClass)},
        {L"willSet",        R, Keyword::WillSet, TokenizerContextComputedProperty}
    };
    
    for(unsigned i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        std::wstring name = keywords[i].name;
        KeywordInfo info = {keywords[i].keyword, keywords[i].type, keywords[i].context};
        this->keywords.insert(std::make_pair(name, info));
        this->keywordNames.insert(std::make_pair(info.keyword, name));
    }
    
}

const std::wstring& Tokenizer::getKeyword(Keyword::T k)
{
    return keywordNames[k];
}

void Tokenizer::set(const wchar_t* data)
{
    if(this->data)
    {
        delete[] this->data;
    }
    //reset state
    this->data = NULL;
    size = 0;
    end = 0;
    state.cursor = 0;
    state.hasSpace = false;
    state.inStringExpression = 0;
    state.line = 1;
    state.column = 1;
    state.context = TokenizerContextFile;
    //copy string
    if(data)
    {
        size = wcslen(data);
        this->data = new wchar_t[size + 1];
        memcpy(this->data, data, (size + 1) * sizeof(wchar_t));
        state.cursor = 0;
        end = this->data + size;
    }
    memset(positions, 0, sizeof(positions));
}
Tokenizer::~Tokenizer()
{
    set(NULL);
}

/*!
 * Save current state for restoring later
 */
TokenizerState& Tokenizer::save()
{
    return state;
}
/*!
 * Restore the specified state
 */
void Tokenizer::restore(const TokenizerState& state)
{
    this->state = state;
}
/*!
 * Restore the state that used to parse given token
 */
void Tokenizer::restore(const Token& token)
{
    this->state = token.state;
}

/*!
 * Tells the tokenizer which context it is in
 */
void Tokenizer::setContext(TokenizerContext context)
{
    state.context = context;
}

void Tokenizer::resetToken(Token& token)
{
    token.type = TokenType::_;
    token.token.clear();
    token.size = 0;
}
bool Tokenizer::peek(wchar_t &ch)
{
    if(!get(ch))
        return false;
    unget();
    return true;
}
bool Tokenizer::get(wchar_t &ch)
{
    if(state.cursor >= (int)size)
        return false;
    //save line and column
    int p = (state.cursor << 1) & 0xf;
    positions[p] = state.line;
    positions[p | 1] = state.column;
    
    
    ch = data[state.cursor++];
    if(ch == '\n')
    {
        //move to next line
        state.line++;
        state.column = 1;
    }
    else
    {
        //move to next column
        state.column++;
    }
    return true;
}
void Tokenizer::unget()
{
    state.cursor--;
    int p = (state.cursor << 1) & 0xf;
    state.line = positions[p];
    state.column = positions[p | 1];
}
bool Tokenizer::skipSpaces()
{
    bool hasSpace = false;
    wchar_t ch;
    while(get(ch))
    {
        if(!iswhite(ch))
        {
            unget();
            return hasSpace;
        }
        hasSpace = true;
    }
    return hasSpace;
}
bool Tokenizer::hasWhiteLeft(const wchar_t* cursor)
{
    if(cursor > data)
    {
        wchar_t ch = cursor[-1];
        return iswhite(ch) || ch == '{' || ch == '(' || ch == '[' || ch == ',' || ch == ';' || ch == ':';
    }
    else
        return true;//BOF means has white before
}

bool Tokenizer::hasWhiteRight(const wchar_t* cursor)
{
    if(cursor < data + size)
    {
        wchar_t ch = *cursor;
        return iswhite(ch) || ch == '}' || ch == ')' || ch == ']' || ch == ',' || ch == ';' || ch == ':';
    }
    return true;//EOF means has white after
}

bool Tokenizer::readOperator(Token& token, bool dotOperator, int max)
{
    wchar_t ch;
    token.type = TokenType::Operator;
    token.operators.type = OperatorType::_;
    const wchar_t* cursor = data + state.cursor;
    bool whiteLeft = hasWhiteLeft(cursor);
    const wchar_t* begin = cursor;
    
    while(get(ch) && (!max || token.token.size() < (size_t)max))
    {
        bool ret = dotOperator ? isDotOperatorCharacter(ch) : isOperatorCharacter(ch);
        if(!ret)
        {
            unget();
            break;
        }
        if(!token.token.empty() && token.token.back() != '.' && ch == '.')
        {
            //This is a undocumented rule of operator
            //An operator contains dot(.) can not contain other characters.
            unget();
            break;
        }
        token.append(ch);
        if(!whiteLeft && (ch == '?' || ch == '!'))
        {
            //no white before, ?! will be used as syntax sugar operator, only one character
            break;
        }
    }
    token.operators.type = calculateOperatorType(begin, begin + token.token.size());
    return true;
}
OperatorType::T Tokenizer::calculateOperatorType(const wchar_t* begin, const wchar_t* end)
{
    OperatorType::T ret = OperatorType::_;
    bool whiteLeft = hasWhiteLeft(begin);
    bool whiteRight = hasWhiteRight(end);
    if((whiteRight && whiteLeft) || (!whiteLeft && !whiteRight))
    {
        //If an operator has whitespace around both sides or around neither side, it is treated as a binary operator. As an example, the + operator in a+b and a + b is treated as a binary operator.
        ret = OperatorType::InfixBinary;
    }
    else if(whiteLeft && !whiteRight)
    {
        //If an operator has whitespace on the left side only, it is treated as a prefix unary operator. As an example, the ++ operator in a ++b is treated as a prefix unary operator.
        ret = OperatorType::PrefixUnary;
    }
    else if(!whiteLeft && whiteRight)
    {
        //If an operator has whitespace on the right side only, it is treated as a postfix unary operator. As an example, the ++ operator in a++ b is treated as a postfix unary operator.
        ret = OperatorType::PostfixUnary;
    }
    
    if(!whiteLeft && (end < (data + size)) && *end == '.')
    {
        //If an operator has no whitespace on the left but is followed immediately by a dot (.), it is treated as a postfix unary operator. As an example, the ++ operator in a++.b is treated as a postfix unary operator (a++ . b rather than a ++ .b).
        ret = OperatorType::PostfixUnary;
    }
    wchar_t front = *begin;
    size_t size = end - begin;
    if(!whiteLeft && size == 1 && (front == '?' || front == '!'))
    {
        //“If the ! or ? operator has no whitespace on the left, it is treated as a postfix operator”
        ret = OperatorType::PostfixUnary;
    }
    return ret;
}

bool Tokenizer::readMultilineComment(Token& token)
{
    wchar_t ch, last;
    token.type = TokenType::Comment;
    token.comment.multiline = true;
    token.comment.nestedLevels = 0;
    get(ch);
    get(ch);
    int level = 1;
    for(;get(ch); last = ch)
    {
        if(last == '/' && ch == '*')
        {
            token.comment.nestedLevels++;
            level++;
        }
        if(last == '*' && ch == '/')
        {
            level--;
            if(level == 0)
            {
                token.token.pop_back();
                break;
            }
        }
        token.append(ch);
    }
    
    return true;
}
bool Tokenizer::readComment(Token& token)
{
    wchar_t ch;
    //read until end of line
    token.type = TokenType::Comment;
    token.comment.multiline = false;
    token.comment.nestedLevels = 0;
    get(ch);
    get(ch);
    while (get(ch))
    {
        if(ch == '\n')
            break;
        token.append(ch);
    }
    return true;
}
bool Tokenizer::readString(Token& token)
{
    wchar_t ch;
    int len = 0;
    token.type = TokenType::String;
    token.string.expressionFollowed = false;
    
    must_get();//“string-literal → "quoted-text”
    while(get(ch))//“quoted-text → quoted-text-itemquoted-textopt”
    {
        if(ch == '\"')
            break;
        if(ch != '\\')
        {
            token.append(ch);
            continue;
        }
        ch = must_get();
        if(ch == '(')//“quoted-text-item → \(expression)”
        {
            token.string.expressionFollowed = true;
            state.inStringExpression = 1;
            break;
        }
        
        //escaped-character → \0  \\  \t  \n  \r  \" \'
        switch(ch)
        {
            case '0':
                token.append('\0');
                break;
            case '\\':
                token.append('\\');
                break;
            case 't':
                token.append('\t');
                break;
            case 'n':
                token.append('\n');
                break;
            case 'r':
                token.append('\r');
                break;
            case '\"':
                token.append('\"');
                break;
            case '\'':
                token.append('\'');
                break;
            case 'U'://escaped-character → \Uhexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit”
                len = 4;  // 8 hexadecimal digits
            case 'u'://escaped-character → \uhexadecimal-digit hexadecimal-digit hexadecimal-digit hexadecimal-digit
                len += 2; // 4 hexadecimal digits
            case 'x'://escaped-character → \xhexadecimal-digit hexadecimal-digit
                len += 2; // 2 hexadecimal digits
            {
                wchar_t r = 0;
                for(int i = 0; i < len; i ++)
                {
                    wchar_t ch = must_get();
                    if(!check_digit(16, ch))
                        error(Errors::E_INVALID_ESCAPE_SEQUENCE_IN_LITERAL);
                    if(ch >= '0' && ch <= '9')
                        ch -= '0';
                    else if(ch >= 'a' && ch <= 'f')
                        ch = ch - 'a' + 0xa;
                    else if(ch >= 'A' && ch <= 'F')
                        ch = ch - 'A' + 0xa;
                    r = (r << 4) | ch;
                }
                token.append(r);
                len = 0;
                break;
            }
            default:
                break;
        }
    }
    return true;
}
bool Tokenizer::readNumberLiteral(Token& token, int base, int64_t& out)
{
    wchar_t ch = must_get();
    do
    {
        if(!check_digit(base, ch))
        {
            unget();
            break;
        }
        token.append(ch);
        if(ch != '_')
        {
            int digit = to_digit(ch);
            out = out * base + digit;
        }
    } while(get(ch));
    return true;
}

bool Tokenizer::readFraction(Token& token, int base, double& out)
{
    out = 0;
    wchar_t ch = must_get();
    double dbase = 1.0 / base;
    double b = dbase;
    do
    {
        if(!check_digit(base, ch))
        {
            unget();
            break;
        }
        token.append(ch);
        if(ch != '_')
        {
            int digit = to_digit(ch);
            out += digit * b;
            b *= dbase;
        }
    } while(get(ch));
    return true;
}
bool Tokenizer::readNumber(Token& token)
{
    wchar_t ch = must_get();
    
    token.number.base = 10;
    token.number.sign = false;
    token.number.exponent = 0;
    token.number.fraction = 0;
    token.number.dvalue = 0;
    token.number.value = 0;
    token.type = TokenType::Integer;
    int64_t val = 0, exponent = 0;
    double fract = 0;
    int base = 10;
    int sign = 1;
    if(ch == '+' || ch == '-')
    {
        token.append(ch);
        if(ch == '-')
            sign = -1;
        ch = must_get();
        token.number.sign = true;

    }
    token.append(ch);
    if(check_digit(base, ch))
        val = to_digit(ch);
    
    if(!peek(ch))
        goto done;
    switch(ch)
    {
        case 'b': base = 2; token.append(must_get()); break;
        case 'o': base = 8; token.append(must_get()); break;
        case 'x': base = 16; token.append(must_get()); break;
        case '.': break;
        default: if(!isdigit(ch) && ch != '_' && ch != 'e') goto done;
    }
    if(ch != '.')
        readNumberLiteral(token, base, val);
    if(peek(ch) && ch == '.')//read fraction part
    {
        //check next digit
        wchar_t dot = must_get();
        if(!peek(ch) || !check_digit(base, ch))
        {
            unget();//rollback .
            goto done;
        }
        
        token.type = TokenType::Float;
        token.append(dot);
        readFraction(token, base, fract);
    }
    if(peek(ch))
    {
        if(base == 10 && ch == 'e')
        {
            token.type = TokenType::Float;
            token.append(must_get());
            readNumberLiteral(token, base, exponent);
            token.number.exponent = true;
        }
        else if(base == 16 && ch == 'p')
        {
            token.type = TokenType::Float;
            token.append(must_get());
            readNumberLiteral(token, base, exponent);
            token.number.exponent = true;
        }
    }

    done:

    token.number.exponent = (int)exponent;
    token.number.base = base;
    token.number.fraction = fract;
    token.number.sign = sign;

    if(token.type == TokenType::Integer)
    {
        token.number.value = sign * val;
        token.number.dvalue = token.number.value;
    }
    else if(token.type == TokenType::Float)
    {
        double dval = (double)val + fract;
        if(exponent != 0)
        {
            int e = pow(base == 10 ? 10 : 2, exponent);

            dval = dval * e;
        }
        dval *= sign;
        token.number.dvalue = dval;
        token.number.value = (int64_t)dval;
    }


    return true;
}
bool Tokenizer::readIdentifier(Token& token)
{
    wchar_t ch;
    get(ch);
    token.type = TokenType::Identifier;
    token.identifier.backtick = false;
    token.identifier.implicitParameterName = false;
    token.identifier.keyword = Keyword::_;
    token.identifier.type = KeywordType::_;
    if(ch == '$')
    {
        //implicit-parameter-name -> $ decimal-digits
        token.append(ch);
        ch = must_get();
        token.append(ch);
        if(isdigit(ch))
        {
            while (get(ch))
            {
                if (!isdigit(ch))
                {
                    unget();
                    break;
                }
                token.append(ch);
            }
            token.identifier.implicitParameterName = true;
            return true;
        }
    }
    else if(ch == '`')
        token.identifier.backtick = true;
    else
        token.append(ch);
    while(get(ch))
    {
        if(!isIdentifierCharacter(ch))
        {
            unget();
            break;
        }
        token.append(ch);
    }
    if(token.identifier.backtick)
    {
        match('`');
    }
    if(!token.identifier.backtick && !token.identifier.implicitParameterName)
    {
        //resolve keyword
        KeywordInfo* keyword = getKeyword(token.token);
        if(keyword)
        {
            token.identifier.keyword = keyword->keyword;
            token.identifier.type = keyword->type;
        }
    }
    return true;
}

KeywordInfo* Tokenizer::getKeyword(const std::wstring& identifier)
{
    std::map<std::wstring, KeywordInfo>::iterator iter = keywords.find(identifier);
    if(iter == keywords.end())
        return nullptr;
    //if not a reserved keyword or can exists in any context
    if(iter->second.type != KeywordType::Reserved || iter->second.context == TokenizerContextAll)
        return &iter->second;
    //check if it's in right context
    if(state.context != TokenizerContextUnknown && (state.context & iter->second.context) == state.context)
        return &iter->second;
    return nullptr;
}

wchar_t Tokenizer::must_get()
{
    wchar_t ch;
    if(!get(ch))
    {
        error(Errors::E_UNEXPECTED_EOF);
    }
    return ch;
}
void Tokenizer::error(int errorCode, const std::wstring& str)
{
    TokenizerError error;
    error.column = state.column;
    error.line = state.line;
    error.errorCode = errorCode;
    error.item = str;
    throw error;
}
void Tokenizer::match(wchar_t expected)
{
    wchar_t ch = must_get();
    if(ch != expected)
    {
        const wchar_t s[2] = {ch, 0};
        error(Errors::E_UNEXPECTED_1, s);
    }
}
bool Tokenizer::readSymbol(Token& token, TokenType::T type)
{
    token.append(must_get());
    token.type = type;
    return true;
}
bool Tokenizer::peek(Token& token)
{
    TokenizerState state = this->state;
    bool ret = next(token);
    this->state = state;
    return ret;
}
bool Tokenizer::next(Token& token)
{
    resetToken(token);
    skipSpaces();
    bool ret = nextImpl(token);
    return ret;
}
bool Tokenizer::nextImpl(Token& token)
{
    wchar_t ch;
    if(!peek(ch))
    {
        token.type = TokenType::EndOfFile;
        token.state = state;
        return false;
    }
    
    token.state = state;

    switch(ch)
    {
        case '/'://check comment
            get(ch);
            if(!peek(ch))
            {
                unget();
                return readOperator(token, false, 1);
            }
            unget();
            if(ch == '/')
                return readComment(token);
            if(ch == '*')
                return readMultilineComment(token);
            return readOperator(token, false, 0);
        case '.':// could be just dot or operator
            get(ch);
            if(peek(ch) && ch == '.')
            {
                //it's an dot operator
                unget();
                return readOperator(token, true, 0);
            }
            unget();
            return readSymbol(token, TokenType::Dot);
        case ':':
            return readSymbol(token, TokenType::Colon);
        case '[':
            return readSymbol(token, TokenType::OpenBracket);
        case ']':
            return readSymbol(token, TokenType::CloseBracket);
        case '(':
            if(state.inStringExpression > 0)
                state.inStringExpression++;
            return readSymbol(token, TokenType::OpenParen);
        case ')':
            if(state.inStringExpression == 1)
            {
                state.inStringExpression = 0;
                return readString(token);
            }
            if(state.inStringExpression > 0)
                state.inStringExpression --;
            return readSymbol(token, TokenType::CloseParen);
        case '{':
            return readSymbol(token, TokenType::OpenBrace);
        case '}':
            return readSymbol(token, TokenType::CloseBrace);
        case '@':
            return readSymbol(token, TokenType::Attribute);
        case '#':
            return readSymbol(token, TokenType::Sharp);
        case '?':
        {
            bool whiteLeft = hasWhiteLeft(data + state.cursor);
            token.operators.type = whiteLeft ? OperatorType::InfixBinary : OperatorType::PostfixUnary;
            return readSymbol(token, TokenType::Optional);
        }
        case ',':
            return readSymbol(token, TokenType::Comma);
        case ';':
            return readSymbol(token, TokenType::Semicolon);
    }
    if(isdigit(ch))
        return readNumber(token);
    if(ch == '+' || ch == '-')
    {
        bool whiteLeft = hasWhiteLeft(data +state.cursor);
        must_get();
        if(whiteLeft && peek(ch) && isdigit(ch))
        {
            unget();
            return readNumber(token);
        }
        unget();
        ch = data[state.cursor];
    }

    if(isOperatorHead(ch))
        return readOperator(token, false, 0);
    if(ch == '"')
    {
        if(state.inStringExpression)
            error(Errors::E_UNEXPECTED_CHARACTER_A_IN_STRING_INTERPOLATION, L"\"");
        return readString(token);
    }
    if(isIdentifierHead(ch) || ch == '$' || ch == '`')
        return readIdentifier(token);
    
    //unexpected token

    return false;
}
