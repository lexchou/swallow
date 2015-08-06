/* token_char_types.h --
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
#ifndef TOKEN_CHAR_TYPES_H
#define TOKEN_CHAR_TYPES_H
#define CHECK(A, B) if(ch >= A && ch <= B)return true;

static inline bool iswhite(wchar_t ch)
{
    switch(ch)
    {
        case 0x0020:// space
        case 0x000A:// line feed
        case 0x000D:// carriage return
        case 0x0009:// horizontal tab
        case 0x000B:// vertical tab
        case 0x000C:// form feed
        case 0x0000:// null
        case 0x200b://
            return true;
        default:
            return false;
    }
}

/*

 operator-head → /  =  -  +  !  *  %  <  >  &  |  ^  ~
‌ operator-head → U+00A1–U+00A7
‌ operator-head → U+00A9 or U+00AB
‌ operator-head → U+00AC or U+00AE
‌ operator-head → U+00B0–U+00B1, U+00B6, U+00BB, U+00BF, U+00D7, or U+00F7
‌ operator-head → U+2016–U+2017 or U+2020–U+2027
‌ operator-head → U+2030–U+203E
‌ operator-head → U+2041–U+2053
‌ operator-head → U+2055–U+205E
‌ operator-head → U+2190–U+23FF
‌ operator-head → U+2500–U+2775
‌ operator-head → U+2794–U+2BFF
‌ operator-head → U+2E00–U+2E7F
‌ operator-head → U+3001–U+3003
‌ operator-head → U+3008–U+3030
‌ operator-character → operator-head
‌ operator-character → U+0300–U+036F
‌ operator-character → U+1DC0–U+1DFF
‌ operator-character → U+20D0–U+20FF
‌ operator-character → U+FE00–U+FE0F
‌ operator-character → U+FE20–U+FE2F
‌ operator-character → U+E0100–U+E01EF
‌ operator-characters → operator-character operator-characters opt
‌ dot-operator-head → ..
‌ dot-operator-character → .  operator-character
‌ dot-operator-characters → dot-operator-character dot-operator-characters opt



 */
inline static bool isOperatorHead(wchar_t ch)
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
        case '?':
        case 0x00A9:
        case 0x00AB:
        case 0x00AC:
        case 0x00AE:
        case 0x00B6:
        case 0x00BB:
        case 0x00BF:
        case 0x00D7:
        case 0x00F7:
            return true;
        default:
            CHECK(0x00A1, 0x00A7);
            CHECK(0x00B0, 0x00B1);
            CHECK(0x2016, 0x2017);
            CHECK(0x2020, 0x2027);
            CHECK(0x2030, 0x203E);
            CHECK(0x2041, 0x2053);
            CHECK(0x2055, 0x205E);
            CHECK(0x2190, 0x23FF);
            CHECK(0x2500, 0x2775);
            CHECK(0x2794, 0x2BFF);
            CHECK(0x2E00, 0x2E7F);
            CHECK(0x3001, 0x3003);
            CHECK(0x3008, 0x3030);
            return false;
    }
}
static bool isOperatorCharacter(wchar_t ch)
{
    if(isOperatorHead(ch))
        return true;
    CHECK(0x0300, 0x036F);
    CHECK(0x1DC0, 0x1DFF);
    CHECK(0x20D0, 0x20FF);
    CHECK(0xFE00, 0xFE0F);
    CHECK(0xFE20, 0xFE2F);
    CHECK(0xE0100, 0xE01EF);
    return false;
}

static bool isDotOperatorCharacter(wchar_t ch)
{
    if(ch == '.')
        return true;
    return isOperatorCharacter(ch);
}


inline static bool check_digit(int base, wchar_t ch)
{
    if(ch == '_')
        return true;
    switch(base)
    {
        case 2: return ch == '0' || ch == '1';
        case 8: return ch >= '0' && ch <= '7';
        case 16: return (ch >= '0' && ch <= '9') || (ch >='a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
        default:
            return ch >='0' && ch <='9';
    }
}
inline static int to_digit(wchar_t ch)
{
    if(ch >= '0' && ch <= '9')
        return ch - '0';
    if(ch >= 'a' && ch <= 'f')
        return ch - 'a' + 0xa;
    if(ch >= 'A' && ch <= 'F')
        return ch - 'A' + 0xa;
    return 0;
}




static bool isIdentifierHead(wchar_t ch)
{
    if(ch >= 'a' && ch <= 'z')
        return true;
    if(ch >= 'A' && ch <= 'Z')
        return true;
    switch(ch)
    {
        case '_':
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
    //‌ identifier-character → identifier-head
    return isIdentifierHead(ch);
}
#undef CHECK




#endif//TOKEN_CHAR_TYPES_H
