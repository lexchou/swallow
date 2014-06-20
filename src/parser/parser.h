#ifndef PARSER_H
#define PARSER_H
#include "swift_conf.h"
#include "tokenizer/tokens.h"

SWIFT_NS_BEGIN

class Tokenizer;
class Node;
class SymbolRegistry;
class NodeFactory;
class Parser
{
public:
    Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry);
    ~Parser();
public:
    Node* parse(const wchar_t* code);
private:
    Node* parseFloat();
    Node* parseInteger();
    Node* parseString();
    Node* parseExpression();
    Node* parsePrefixExpression();
    Node* parsePostfixExpression();
    Node* parsePrimaryExpression();
    Node* parseLiteralExpression();
    Node* parseLiteral();

    std::pair<Node*, Node*> parseDictionaryLiteralItem();
private:
    void match_identifier(Token& token);
    void match(const wchar_t* token);
    void tassert(Token& token, bool cond);
private:
    Tokenizer* tokenizer;
    NodeFactory* nodeFactory;
    SymbolRegistry* symbolRegistry;
};



SWIFT_NS_END

#endif//PARSER_H
