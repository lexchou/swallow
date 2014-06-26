#ifndef PARSER_H
#define PARSER_H
#include "swift_conf.h"
#include "tokenizer/tokens.h"
#include <string>

SWIFT_NS_BEGIN

class Tokenizer;
class Node;
class ExpressionNode;
class FunctionCall;
class SymbolRegistry;
class Identifier;
class NodeFactory;
class ClosureExpression;
class CodeBlock;
class Statement;
class ParenthesizedExpression;
class TypeNode;
class CaseStatement;
class Tuple;
class Parser
{
    enum
    {
        ENABLE_GENERIC = 1,
        UNDER_SWITCH_CASE = 2,
    };
    struct Flag
    {
        Flag(Parser* parser):parser(parser), flags(parser->flags){}
        ~Flag(){parser->flags = flags;};
        
        Parser* parser;
        int flags;
    };
public:
    Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry);
    ~Parser();
public:
    Node* parse(const wchar_t* code);
    void setFileName(const wchar_t* fileName);
    void setFunctionName(const wchar_t* function);
private://statement
    Statement* parseStatement();
    Statement* parseLoopStatement();
    Statement* parseForLoop();
    Statement* parseWhileLoop();
    Statement* parseDoLoop();
    Statement* parseIf();
    Statement* parseSwitch();
    void parseSwitchStatements(CaseStatement* case_);
    Statement* parseBreak();
    Statement* parseContinue();
    Statement* parseFallthrough();
    Statement* parseReturn();
    Statement* parseLabeledStatement();
    ExpressionNode* parseConditionExpression();
    
    CodeBlock* parseCodeBlock();
private://pattern
    ExpressionNode* parsePattern();
    ExpressionNode* parseTuple();
    ExpressionNode* parseEnumPattern();
    ExpressionNode* parseTypeCastingPattern();
private://declaration
    Node* parseDeclaration();
private://expression
    ExpressionNode* parseFloat();
    ExpressionNode* parseInteger();
    ExpressionNode* parseString();
    ExpressionNode* parseExpression();
    ExpressionNode* parseBinaryExpression(ExpressionNode* lhs);
    ExpressionNode* parsePrefixExpression();
    ExpressionNode* parsePostfixExpression();
    FunctionCall* parseFunctionCallExpression();
    ExpressionNode* parsePrimaryExpression();
    ExpressionNode* parseLiteralExpression();
    ExpressionNode* parseParenthesizedExpression();
    void parseExpressionItem(ParenthesizedExpression* parent);
    ExpressionNode* parseLiteral();
    
    ExpressionNode* parseSelfExpression();
    ExpressionNode* parseSuperExpression();
    Identifier* parseIdentifier();
    ClosureExpression* parseClosureExpression();
    TypeNode* parseType();
    
    std::pair<ExpressionNode*, ExpressionNode*> parseDictionaryLiteralItem();
private:
    /**
     * Read next token from tokenizer, throw exception if EOF reached.
     */
    void next(Token& token);
    /**
     * Peek next token from tokenizer, return false if EOF reached.
     */
    bool peek(Token& token);
    /**
     * Check if the following token is an identifier, throw exception if not matched
     */
    void expect_identifier(Token& token);
    /**
     * Check if the following token is the specified one, throw exception if not matched
     */
    void expect(const wchar_t* token);
    /**
     * Check if the following token is the specified keyword, throw exception if not matched
     */
    void expect(Keyword::T keyword);

    /**
     * Throw an exception with the unexpected token
     */
    void unexpected(const Token& token);

    /**
     * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
     */
    bool match(const wchar_t* token);
    /**
     * Check if the following token is an identifier(keywords included), consume the token and return true if matched or rollback and return false
     */
    bool match_identifier(Token& token);
    
    /**
     * Return true if the next token is the specified one, no token will be consumed
     */
    bool predicate(const wchar_t* token);

    void tassert(Token& token, bool cond);
private:
    Tokenizer* tokenizer;
    NodeFactory* nodeFactory;
    SymbolRegistry* symbolRegistry;
    std::wstring fileName;
    std::wstring functionName;
    int flags;
};



SWIFT_NS_END

#endif//PARSER_H
