#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "tokens.h"
#include <cstring>
#include <string>
#include <map>

SWIFT_NS_BEGIN

struct TokenizerError
{
    int line;
    int column;
    int error_code;
};
struct KeywordInfo
{
    Keyword::T keyword;
    KeywordType::T type;
};
class Tokenizer
{
public:
    Tokenizer(const wchar_t* data);
    ~Tokenizer();
public:
    void set(const wchar_t* data);
    bool next(Token& token);
    bool peek(Token& token);

    const std::wstring& getKeyword(Keyword::T k);

    /**
     * Save current state for restoring later
     */
    const TokenizerState& save();
    /**
     * Restore the specified state
     */
    void restore(const TokenizerState& state);
    /**
     * Restore the state that used to parse given token
     */
    void restore(const Token& token);
private:
    bool nextImpl(Token& token);
    void resetToken(Token& token);
    bool skipSpaces();
    bool get(wchar_t &ch);
    void unget();
    bool peek(wchar_t &ch);
    
    void tassert(bool cond);
    wchar_t must_get();
    void match(wchar_t ch);
    
    
    bool hasWhiteLeft(const wchar_t* cursor);
    bool hasWhiteRight(const wchar_t* cursor);
    OperatorType::T calculateOperatorType(const wchar_t* begin, const wchar_t* end);
    
    bool readSymbol(Token& token, TokenType::T type);
    bool readOperator(Token& token, int max = 0);
    bool readComment(Token& token);
    bool readMultilineComment(Token& token);
    bool readString(Token& token);
    bool readNumber(Token& token);
    bool readNumberLiteral(Token& token, int base, int64_t& out);
    bool readFraction(Token& token, int base, double& out);
    bool readIdentifier(Token& token);
private:
    wchar_t* data;
    const wchar_t* end;
    size_t size;
    TokenizerState state;
    int positions[16];
    std::map<std::wstring, KeywordInfo> keywords;
    std::map<Keyword::T, std::wstring> keywordNames;
};


SWIFT_NS_END
#endif//TOKENIZER_H
