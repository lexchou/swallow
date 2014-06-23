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
class ParenthesizedExpression;
class TypeNode;
class Parser
{
public:
    Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry);
    ~Parser();
public:
    Node* parse(const wchar_t* code);
    void setFileName(const wchar_t* fileName);
    void setFunctionName(const wchar_t* function);
private:
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
     * Throw an exception with the unexpected token
     */
    void unexpected(const Token& token);

    /**
     * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
     */
    bool match(const wchar_t* token);
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
};



SWIFT_NS_END

#endif//PARSER_H
