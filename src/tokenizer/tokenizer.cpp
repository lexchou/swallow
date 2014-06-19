#include "tokenizer.h"
#include "token_char_types.h"
#include <cstring>
Tokenizer::Tokenizer(const wchar_t* data)
{
    this->data = NULL;
    set(data);
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
    cursor = 0;
    end = 0;
    hasSpace = false;
    inStringExpression = false;
    //copy string
    if(data)
    {
        size = wcslen(data);
        this->data = new wchar_t[size + 1];
        memcpy(this->data, data, (size + 1) * sizeof(wchar_t));
        cursor = this->data;
        end = cursor + size;
    }
}
Tokenizer::~Tokenizer()
{
    set(nullptr);
}

void Tokenizer::resetToken(Token& token)
{
    token.customOperator = false;
    token.comment.nestedLevels = 0;
    token.comment.multiline = 0;
    
    token.identifier.keyword = kNone;
    token.identifier.backtick = false;
    token.identifier.implicitParameterName = false;
    
    
    token.type = kEOF;
    token.token.clear();
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
    if(cursor >= end)
        return false;
    ch = *cursor++;
    return true;
}
void Tokenizer::unget()
{
    cursor--;
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
    token.type = kOperator;
    token.operators.type = kUnknownOperator;
    bool spaceAfter = false;
    bool spaceBefore = false;
    
    if(cursor > data)
    {
        ch = cursor[-1];
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
        token.token.push_back(ch);
        if(!spaceBefore && (ch == '?' || ch == '!'))
        {
            //no white before, ?! will be used as syntax sugar operator, only one character
            break;
        }
    }
    if(cursor < data + size)
    {
        ch = *cursor;
        spaceAfter = iswhite(ch) || ch == '}' || ch == ')' || ch == ']' || ch == ',' || ch == ';' || ch == ':';
    }
    else
        spaceAfter = true;//EOF means has white after
    if((spaceAfter && spaceBefore) || (!spaceBefore && !spaceAfter))
    {
        //If an operator has whitespace around both sides or around neither side, it is treated as a binary operator. As an example, the + operator in a+b and a + b is treated as a binary operator.
        token.operators.type = kInfixBinary;
    }
    else if(spaceBefore && !spaceAfter)
    {
        //If an operator has whitespace on the left side only, it is treated as a prefix unary operator. As an example, the ++ operator in a ++b is treated as a prefix unary operator.
        token.operators.type = kPrefixUnary;
    }
    else if(!spaceBefore && spaceAfter)
    {
        //If an operator has whitespace on the right side only, it is treated as a postfix unary operator. As an example, the ++ operator in a++ b is treated as a postfix unary operator.
        token.operators.type = kPostfixUnary;
    }
    
    if(!spaceBefore && (cursor < (data + size)) && *cursor == '.')
    {
        //If an operator has no whitespace on the left but is followed immediately by a dot (.), it is treated as a postfix unary operator. As an example, the ++ operator in a++.b is treated as a postfix unary operator (a++ . b rather than a ++ .b).
        token.operators.type = kPostfixUnary;
    }
    if(!spaceBefore && token.token.size() == 1 && (token.token.front() == '?' || token.token.front() == '!'))
    {
        //“If the ! or ? operator has no whitespace on the left, it is treated as a postfix operator”
        token.operators.type = kPostfixUnary;
    }
    
    return true;
}
bool Tokenizer::readMultilineComment(Token& token)
{
    wchar_t ch, last;
    token.type = kComment;
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
        token.token.push_back(ch);
    }
    
    return true;
}
bool Tokenizer::readComment(Token& token)
{
    wchar_t ch;
    //read until end of line
    token.type = kComment;
    token.comment.multiline = false;
    token.comment.nestedLevels = 0;
    get(ch);
    get(ch);
    while (get(ch))
    {
        if(ch == '\n')
            break;
        token.token.push_back(ch);
    }
    return true;
}
bool Tokenizer::readString(Token& token)
{
    wchar_t ch;
    int len = 0;
    inStringExpression = false;
    token.type = kString;
    token.string.expressionFollowed = false;
    
    must_get();//“string-literal → "quoted-text”
    while(get(ch))//“quoted-text → quoted-text-itemquoted-textopt”
    {
        if(ch == '\"')
            break;
        if(ch != '\\')
        {
            token.token.push_back(ch);
            continue;
        }
        ch = must_get();
        if(ch == '(')//“quoted-text-item → \(expression)”
        {
            token.string.expressionFollowed = true;
            inStringExpression = true;
            break;
        }
        
        //escaped-character → \0  \\  \t  \n  \r  \" \'
        switch(ch)
        {
            case '0':
                token.token.push_back('\0');
                break;
            case '\\':
                token.token.push_back('\\');
                break;
            case 't':
                token.token.push_back('\t');
                break;
            case 'n':
                token.token.push_back('\n');
                break;
            case 'r':
                token.token.push_back('\r');
                break;
            case '\"':
                token.token.push_back('\"');
                break;
            case '\'':
                token.token.push_back('\'');
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
                    tassert(ishexnumber(ch));
                    if(ch >= '0' && ch <= '9')
                        ch -= '0';
                    else if(ch >= 'a' && ch <= 'f')
                        ch = ch - 'a' + 0xa;
                    else if(ch >= 'A' && ch <= 'F')
                        ch = ch - 'A' + 0xa;
                    r = (r << 4) | ch;
                }
                token.token.push_back(r);
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
    token.token.push_back(ch);
    while(get(ch))
    {
        if(!check_digit(base, ch))
        {
            unget();
            break;
        }
        token.token.push_back(ch);
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
    token.type = kInteger;
    
    if(ch == '+' || ch == '-')
    {
        token.token.push_back(ch);
        ch = must_get();
        token.number.sign = true;
    }
    token.token.push_back(ch);
        
    if(!peek(ch))
        return true;
    int base = 10;
    switch(ch)
    {
        case 'b': base = 2; token.token.push_back(must_get()); break;
        case 'o': base = 8; token.token.push_back(must_get()); break;
        case 'x': base = 16; token.token.push_back(must_get()); break;
        default: if(!isdigit(ch)) return true;
    }
    token.number.base = base;
    readNumberLiteral(token, base);
    if(peek(ch) && ch == '.')//read fraction part
    {
        token.type = kFloat;
        token.token.push_back(must_get());
        readNumberLiteral(token, base);
    }
    if(peek(ch))
    {
        if(base == 10 && ch == 'e')
        {
            token.token.push_back(must_get());
            readNumberLiteral(token, base);
            token.number.exponent = true;
        }
        else if(base == 16 && ch == 'p')
        {
            token.token.push_back(must_get());
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
    token.type = kIdentifier;    
    if(ch == '$')
    {
        //implicit-parameter-name -> $ decimal-digits
        token.token.push_back(ch);
        ch = must_get();
        tassert(isdigit(ch));
        token.token.push_back(ch);
        while(get(ch))
        {
            if(!isdigit(ch))
            {
                unget();
                break;
            }
            token.token.push_back(ch);
        }
        token.identifier.implicitParameterName = true;
        return true;
    }
    if(ch == '`')
        token.identifier.backtick = true;
    else
        token.token.push_back(ch);
    while(get(ch))
    {
        if(!isIdentifierCharacter(ch))
        {
            unget();
            break;
        }
        token.token.push_back(ch);
    }
    if(token.identifier.backtick)
    {
        token.identifier.keyword = kNone;
        match('`');
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
bool Tokenizer::readSymbol(Token& token, TokenType type)
{
    token.token.push_back(must_get());
    token.type = type;
    return true;
}
bool Tokenizer::next(Token& token)
{
    resetToken(token);
    skipSpaces();
    bool ret = nextImpl(token);
    token.token.push_back(0);
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
            return readSymbol(token, kColon);
        case '[':
            return readSymbol(token, kOpenBracket);
        case ']':
            return readSymbol(token, kCloseBracket);
        case '(':
            return readSymbol(token, kOpenParen);
        case ')':
            if(inStringExpression)
                return readString(token);
            return readSymbol(token, kCloseParen);
        case '{':
            return readSymbol(token, kOpenBrace);
        case '}':
            return readSymbol(token, kCloseBrace);
        case '@':
            return readSymbol(token, kAttribute);
        case '?':
            return readSymbol(token, kOptional);
        case ',':
            return readSymbol(token, kComma);
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
        ch = *cursor;
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