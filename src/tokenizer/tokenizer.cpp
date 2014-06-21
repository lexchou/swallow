#include "tokenizer.h"
#include "token_char_types.h"
#include <cstring>
#include <wchar.h>
#include <ctype.h>
using namespace Swift;

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
    } keywords[] = {
        //Declaration keywords
        {L"class",          D, Keyword::Class},
        {L"deinit",         D, Keyword::Deinit},
        {L"enum",           D, Keyword::Enum},
        {L"extension",      D, Keyword::Extension},
        {L"func",           D, Keyword::Func},
        {L"import",         D, Keyword::Import},
        {L"init",           D, Keyword::Init},
        {L"let",            D, Keyword::Let},
        {L"protocol",       D, Keyword::Protocol},
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
        {L"if",             S, Keyword::If},
        {L"in",             S, Keyword::In},
        {L"for",            S, Keyword::For},
        {L"return",         S, Keyword::Return},
        {L"switch",         S, Keyword::Switch},
        {L"where",          S, Keyword::Where},
        {L"while",          S, Keyword::While},
        //Expression and type keywords
        {L"as",             E, Keyword::As},
        {L"dynamicType",    E, Keyword::DynamicType},
        {L"is",             E, Keyword::Is},
        {L"new",            E, Keyword::New},
        {L"super",          E, Keyword::Super},
        {L"self",           E, Keyword::Self},
        {L"Self",           E, Keyword::SelfU},
        {L"Type",           E, Keyword::Type},
        {L"__COLUMN__",     E, Keyword::Column},
        {L"__FILE__",       E, Keyword::File},
        {L"__FUNCTION__",   E, Keyword::Function},
        {L"__LINE__",       E, Keyword::Line},
        //Reserved keywords
        {L"associativity",  R, Keyword::Associativity},
        {L"didSet",         R, Keyword::DidSet},
        {L"get",            R, Keyword::Get},
        {L"infix",          R, Keyword::Infix},
        {L"inout",          R, Keyword::Inout},
        {L"left",           R, Keyword::Left},
        {L"mutating",       R, Keyword::Mutating},
        {L"none",           R, Keyword::None},
        {L"nonmutating",    R, Keyword::Nonmutating},
        {L"operator",       R, Keyword::Operator},
        {L"override",       R, Keyword::Override},
        {L"postfix",        R, Keyword::Postfix},
        {L"precedence",     R, Keyword::Precedence},
        {L"prefix",         R, Keyword::Prefix},
        {L"right",          R, Keyword::Right},
        {L"set",            R, Keyword::Set},
        {L"unowned",        R, Keyword::Unowned},
        {L"unowned(safe)",  R, Keyword::Unowned_safe},
        {L"unowned(unsafe)",R, Keyword::Unowned_unsafe},
        {L"weak",           R, Keyword::Weak},
        {L"willSet",        R, Keyword::WillSet}
    };
    
    for(int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        std::wstring name = keywords[i].name;
        KeywordInfo info = {keywords[i].keyword, keywords[i].type};
        this->keywords.insert(std::make_pair(name, info));
    }
    
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
    state.inStringExpression = false;
    state.line = 1;
    state.column = 1;
    //copy string
    if(data)
    {
        size = wcslen(data);
        this->data = new wchar_t[size + 1];
        memcpy(this->data, data, (size + 1) * sizeof(wchar_t));
        state.cursor = this->data;
        end = state.cursor + size;
    }
}
Tokenizer::~Tokenizer()
{
    set(NULL);
}

/**
 * Save current state for restoring later
 */
TokenizerState Tokenizer::save()
{
    return state;
}
/**
 * Restore the specified state
 */
void Tokenizer::restore(const TokenizerState& state)
{
    this->state = state;
}
/**
 * Restore the state that used to parse given token
 */
void Tokenizer::restore(const Token& token)
{
    this->state = token.state;
}
void Tokenizer::resetToken(Token& token)
{
    token.type = TokenType::_;
    token.token.clear();
    token.size = 0;
    token.state = state;
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
    if(state.cursor >= end)
        return false;
    ch = *state.cursor++;
    return true;
}
void Tokenizer::unget()
{
    state.cursor--;
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
bool Tokenizer::readOperator(Token& token, int max)
{
    wchar_t ch;
    token.type = TokenType::Operator;
    token.operators.type = OperatorType::_;
    bool spaceAfter = false;
    bool spaceBefore = false;
    
    if(state.cursor > data)
    {
        ch = state.cursor[-1];
        spaceBefore = iswhite(ch) || ch == '{' || ch == '(' || ch == '[' || ch == ',' || ch == ';' || ch == ':';
    }
    else
        spaceBefore = true;//BOF means has white before
    while(get(ch) && (!max || token.token.size() < max))
    {
        if(!isOperator(ch))
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
        if(!spaceBefore && (ch == '?' || ch == '!'))
        {
            //no white before, ?! will be used as syntax sugar operator, only one character
            break;
        }
    }
    if(state.cursor < data + size)
    {
        ch = *state.cursor;
        spaceAfter = iswhite(ch) || ch == '}' || ch == ')' || ch == ']' || ch == ',' || ch == ';' || ch == ':';
    }
    else
        spaceAfter = true;//EOF means has white after
    if((spaceAfter && spaceBefore) || (!spaceBefore && !spaceAfter))
    {
        //If an operator has whitespace around both sides or around neither side, it is treated as a binary operator. As an example, the + operator in a+b and a + b is treated as a binary operator.
        token.operators.type = OperatorType::InfixBinary;
    }
    else if(spaceBefore && !spaceAfter)
    {
        //If an operator has whitespace on the left side only, it is treated as a prefix unary operator. As an example, the ++ operator in a ++b is treated as a prefix unary operator.
        token.operators.type = OperatorType::PrefixUnary;
    }
    else if(!spaceBefore && spaceAfter)
    {
        //If an operator has whitespace on the right side only, it is treated as a postfix unary operator. As an example, the ++ operator in a++ b is treated as a postfix unary operator.
        token.operators.type = OperatorType::PostfixUnary;
    }
    
    if(!spaceBefore && (state.cursor < (data + size)) && *state.cursor == '.')
    {
        //If an operator has no whitespace on the left but is followed immediately by a dot (.), it is treated as a postfix unary operator. As an example, the ++ operator in a++.b is treated as a postfix unary operator (a++ . b rather than a ++ .b).
        token.operators.type = OperatorType::PostfixUnary;
    }
    if(!spaceBefore && token.token.size() == 1 && (token.token.front() == '?' || token.token.front() == '!'))
    {
        //“If the ! or ? operator has no whitespace on the left, it is treated as a postfix operator”
        token.operators.type = OperatorType::PostfixUnary;
    }
    
    return true;
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
    state.inStringExpression = false;
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
            state.inStringExpression = true;
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
                    tassert(check_digit(16, ch));
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
bool Tokenizer::readNumberLiteral(Token& token, int base)
{
    wchar_t ch = must_get();
    tassert(check_digit(base, ch));
    token.append(ch);
    while(get(ch))
    {
        if(!check_digit(base, ch))
        {
            unget();
            break;
        }
        token.append(ch);
    }
    return true;
}
bool Tokenizer::readNumber(Token& token)
{
    wchar_t ch = must_get();
    
    token.number.base = 10;
    token.number.sign = false;
    token.number.exponent = false;
    token.number.fraction = false;
    token.type = TokenType::Integer;
    
    if(ch == '+' || ch == '-')
    {
        token.append(ch);
        ch = must_get();
        token.number.sign = true;
    }
    token.append(ch);
    
    if(!peek(ch))
        return true;
    int base = 10;
    switch(ch)
    {
        case 'b': base = 2; token.append(must_get()); break;
        case 'o': base = 8; token.append(must_get()); break;
        case 'x': base = 16; token.append(must_get()); break;
        case '.': break;
        default: if(!isdigit(ch)) return true;
    }
    token.number.base = base;
    if(ch != '.')
        readNumberLiteral(token, base);
    if(peek(ch) && ch == '.')//read fraction part
    {
        token.type = TokenType::Float;
        token.append(must_get());
        readNumberLiteral(token, base);
    }
    if(peek(ch))
    {
        if(base == 10 && ch == 'e')
        {
            token.append(must_get());
            readNumberLiteral(token, base);
            token.number.exponent = true;
        }
        else if(base == 16 && ch == 'p')
        {
            token.append(must_get());
            readNumberLiteral(token, base);
            token.number.exponent = true;
        }
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
        tassert(isdigit(ch));
        token.append(ch);
        while(get(ch))
        {
            if(!isdigit(ch))
            {
                unget();
                break;
            }
            token.append(ch);
        }
        token.identifier.implicitParameterName = true;
        return true;
    }
    if(ch == '`')
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
        using namespace std;
        map<wstring, KeywordInfo>::iterator iter = keywords.find(token.c_str());
        if(iter != keywords.end())
        {
            token.identifier.keyword = iter->second.keyword;
            token.identifier.type = iter->second.type;
        }
    }
    return true;
}
void Tokenizer::tassert(bool cond)
{
    if(!cond)
    {
        throw TokenizerError();
    }
}
wchar_t Tokenizer::must_get()
{
    wchar_t ch;
    if(!get(ch))
    {
        throw TokenizerError();
    }
    return ch;
}
void Tokenizer::match(wchar_t expected)
{
    wchar_t ch = must_get();
    tassert(ch == expected);
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
    token.append(0);
    return ret;
}
bool Tokenizer::nextImpl(Token& token)
{
    wchar_t ch;
    if(!peek(ch))
        return false;

    switch(ch)
    {
        case '/'://check comment
            get(ch);
            if(!peek(ch))
            {
                unget();
                return readOperator(token, 1);
            }
            unget();
            if(ch == '/')
                return readComment(token);
            if(ch == '*')
                return readMultilineComment(token);
            return readOperator(token);
        case ':':
            return readSymbol(token, TokenType::Colon);
        case '[':
            return readSymbol(token, TokenType::OpenBracket);
        case ']':
            return readSymbol(token, TokenType::CloseBracket);
        case '(':
            return readSymbol(token, TokenType::OpenParen);
        case ')':
            if(state.inStringExpression)
                return readString(token);
            return readSymbol(token, TokenType::CloseParen);
        case '{':
            return readSymbol(token, TokenType::OpenBrace);
        case '}':
            return readSymbol(token, TokenType::CloseBrace);
        case '@':
            return readSymbol(token, TokenType::Attribute);
        case '?':
            return readSymbol(token, TokenType::Optional);
        case ',':
            return readSymbol(token, TokenType::Comma);
    }
    if(isdigit(ch))
        return readNumber(token);
    if(ch == '+' || ch == '-')
    {
        must_get();
        if(peek(ch) && isdigit(ch))
        {
            unget();
            return readNumber(token);
        }
        unget();
        ch = *state.cursor;
    }
    
    if(isOperator(ch))
        return readOperator(token);
    if(ch == '"')
        return readString(token);
    if(isIdentifierHead(ch) || ch == '$' || ch == '`')
        return readIdentifier(token);
    
    //unexpected token

    return false;
}
