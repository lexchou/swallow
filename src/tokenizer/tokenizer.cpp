#include "tokenizer.h"
#include <cstring>
Tokenizer::Tokenizer(const wchar_t* data)
{
    size = wcslen(data);
    this->data = new wchar_t[size + 1];
    memcpy(this->data, data, (size + 1) * sizeof(wchar_t));
    cursor = this->data;
    end = cursor + size;
    hasSpace = false;
}
void Tokenizer::resetToken(Token& token)
{
    token.customOperator = false;
    token.comment.nestedLevels = 0;
    token.comment.multiline = 0;
    
    token.identifier.keyword = kNone;
    token.identifier.backtick = false;
    token.identifier.implicitParameterName = false;
    
    token.spaceAfter = false;
    token.spaceBefore = false;
    
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
        if(!isspace(ch))
        {
            unget();
            return hasSpace;
        }
        hasSpace = true;
    }
    return hasSpace;
}
inline static bool isOperator(wchar_t ch)
{
    switch(ch)
    {
        case '/':
        case '=':
        case '-':
        case '+':
        case '!':
        case '*':
        case '%':
        case '<':
        case '>':
        case '&':
        case '|':
        case '^':
        case '~':
        case '.':
            return true;
        default:
            return false;
            
    }
}
bool Tokenizer::readOperator(Token& token, int max)
{
    wchar_t ch;
    token.type = kOperator;
    while(get(ch) && (!max || token.token.size() < max))
    {
        if(!isOperator(ch))
        {
            unget();
            break;
        }
        token.token.push_back(ch);
    }
    token.token.push_back(0);
    return true;
}
bool Tokenizer::readMultilineComment(Token& token)
{
    token.comment.multiline = true;
    return true;
}
bool Tokenizer::readComment(Token& token)
{
    wchar_t ch;
    //read until end of line
    get(ch);
    get(ch);
    while (get(ch))
    {
        if(ch == '\n')
            break;
        token.token.push_back(ch);
    }
    token.token.push_back(0);
    return true;
}
bool Tokenizer::readString(Token& token)
{
    return false;
}
bool Tokenizer::readNumber(Token& token)
{
    wchar_t ch;
    
}
#define CHECK(A, B) if(ch >= A && ch <= B)return true;

inline static bool isIdentifierHead(wchar_t ch)
{
    if(ch >= 'a' && ch <= 'z')
        return true;
    if(ch >= 'A' && ch <= 'Z')
        return true;
    switch(ch)
    {
        case 0x00A8:
        case 0x00AA:
        case 0x00AD:
        case 0x00AF:
        case 0x2054:
            return true;
    }
// identifier-head → U+00A8, U+00AA, U+00AD, U+00AF, U+00B2–U+00B5, or U+00B7–U+00BA
    CHECK(0x00B2, 0x00B5)
    CHECK(0x00B7, 0x00BA)
// identifier-head → U+00BC–U+00BE, U+00C0–U+00D6, U+00D8–U+00F6, or U+00F8–U+00FF
    CHECK(0x00BC, 0x00BE)
    CHECK(0x00C0, 0x00D6)
    CHECK(0x00D8, 0x00F6)
    CHECK(0x00F8, 0x00FF)
// identifier-head → U+0100–U+02FF, U+0370–U+167F, U+1681–U+180D, or U+180F–U+1DBF
    CHECK(0x0100, 0x02FF)
    CHECK(0x0370, 0x167F)
    CHECK(0x1681, 0x180D)
    CHECK(0x180F, 0x1DBF)
    
// identifier-head → U+1E00–U+1FFF
    CHECK(0x1E00, 0x1FFF)

// identifier-head → U+200B–U+200D, U+202A–U+202E, U+203F–U+2040, U+2054, or U+2060–U+206F
    CHECK(0x200B, 0x200D)
    CHECK(0x202A, 0x202E)
    CHECK(0x203F, 0x2040)
    CHECK(0x2060, 0x206F)
// identifier-head → U+2070–U+20CF, U+2100–U+218F, U+2460–U+24FF, or U+2776–U+2793
    CHECK(0x2070, 0x20CF)
    CHECK(0x2100, 0x218F)
    CHECK(0x2460, 0x24FF)
    CHECK(0x2776, 0x2793)
    
// identifier-head → U+2C00–U+2DFF or U+2E80–U+2FFF
    CHECK(0x2C00, 0x2DFF)
    CHECK(0x2E80, 0x2FFF)
    
// identifier-head → U+3004–U+3007, U+3021–U+302F, U+3031–U+303F, or U+3040–U+D7FF
    CHECK(0x3004, 0x3007)
    CHECK(0x3021, 0x302F)
    CHECK(0x3031, 0x303F)
    CHECK(0x3040, 0xD7FF)
// identifier-head → U+F900–U+FD3D, U+FD40–U+FDCF, U+FDF0–U+FE1F, or U+FE30–U+FE44
    CHECK(0xF900, 0xFD3D)
    CHECK(0xFD40, 0xFDCF)
    CHECK(0xFDF0, 0xFE1F)
    CHECK(0xFE30, 0xFE44)
// identifier-head → U+FE47–U+FFFD
    CHECK(0xFE47, 0xFFFD)
    
// identifier-head → U+10000–U+1FFFD, U+20000–U+2FFFD, U+30000–U+3FFFD, or U+40000–U+4FFFD
    CHECK(0x10000, 0x1FFFD)
    CHECK(0x20000, 0x2FFFD)
    CHECK(0x30000, 0x3FFFD)
    CHECK(0x40000, 0x4FFFD)
// identifier-head → U+50000–U+5FFFD, U+60000–U+6FFFD, U+70000–U+7FFFD, or U+80000–U+8FFFD
    CHECK(0x50000, 0x5FFFD)
    CHECK(0x60000, 0x6FFFD)
    CHECK(0x70000, 0x7FFFD)
    CHECK(0x80000, 0x8FFFD)
// identifier-head → U+90000–U+9FFFD, U+A0000–U+AFFFD, U+B0000–U+BFFFD, or U+C0000–U+CFFFD
    CHECK(0x90000, 0x9FFFD)
    CHECK(0xA0000, 0xAFFFD)
    CHECK(0xB0000, 0xBFFFD)
    CHECK(0xC0000, 0xCFFFD)
// identifier-head → U+D0000–U+DFFFD or U+E0000–U+EFFFD
    CHECK(0xD0000, 0xDFFFD)
    CHECK(0xE0000, 0xEFFFD)

    return false;
}

inline static bool isIdentifierCharacter(wchar_t ch)
{
    
    // identifier-character → Digit 0 through 9
    if(ch >= '0' && ch <= '9')
        return true;
    // identifier-character → U+0300–U+036F, U+1DC0–U+1DFF, U+20D0–U+20FF, or U+FE20–U+FE2F
    CHECK(0x0300, 0x036F)
    CHECK(0x1DC0, 0x1DFF)
    CHECK(0x20D0, 0x20FF)
    CHECK(0xFE20, 0xFE2F)
    //‌ identifier-character → identifier-head”
    return isIdentifierHead(ch);
}
#undef CHECK

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
    token.token.push_back(0);
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
    token.token.push_back(0);
    token.type = type;
    return true;
}
bool Tokenizer::next(Token& token)
{
    resetToken(token);
    token.spaceBefore = skipSpaces();
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
    if(isOperator(ch) || ch == '`')
        return readOperator(token);
    if(ch == '"')
        return readString(token);
    if(isdigit(ch))
        return readNumber(token);
    if(isIdentifierHead(ch) || ch == '$')
        return readIdentifier(token);
    
    //unexpected token

    return false;
}