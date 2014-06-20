#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
using namespace Swift;

#define CHECK_EOF(x) x

Parser::Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry)
    :nodeFactory(nodeFactory), symbolRegistry(symbolRegistry)
{
    tokenizer = new Tokenizer(NULL);
}
Parser::~Parser()
{
    delete tokenizer;
}
void Parser::match_identifier(Token& token)
{
    CHECK_EOF(tokenizer->next(token));
    tassert(token, token.type == TokenType::Identifier);
    tassert(token, token.identifier.keyword == Keyword::_);
}
void Parser::tassert(Token& token, bool cond)
{
    if(!cond)
        throw 0;
}

Node* Parser::parse(const wchar_t* code)
{
    tokenizer->set(code);
    Token token;
    return parseExpression();
}
Node* Parser::parseFloat()
{
    Token token;
    tokenizer->next(token);
    Node* ret = nodeFactory->createFloat(token.c_str());
    return ret;
}
Node* Parser::parseInteger()
{
    Token token;
    tokenizer->next(token);
    Node* ret = nodeFactory->createInteger(token.c_str());
    return ret;
}
Node* Parser::parseString()
{
    Token token;
    tokenizer->next(token);
    Node* ret = nodeFactory->createString(token.c_str());
    return ret;
}
/*
 GRAMMAR OF A PREFIX EXPRESSION
 prefix-expression → prefix-operator opt postfix-expression
 prefix-expression → in-out-expression
 in-out-expression → & identifier
*/
Node* Parser::parsePrefixExpression()
{
    Token token;
    tokenizer->next(token);

    if(token.type == TokenType::Operator && token == L"&")
    {
         //in-out-expression → & identifier
        match_identifier(token);
        Identifier* identifier = nodeFactory->createIdentifier(token.c_str());
        InOutParameter* ret = nodeFactory->createInOutParameter(identifier);
        return ret;
    }


    Node* postfixExpression = parsePostfixExpression();
    if(symbolRegistry->isPrefixOperator(token.c_str()))
    {
        UnaryOperator* op = nodeFactory->createUnary(token.c_str(), postfixExpression);
        return op;
    }
    return postfixExpression;
}
/*
GRAMMAR OF A POSTFIX EXPRESSION
 postfix-expression → primary-expression
 postfix-expression → postfix-expression postfix-operator
 postfix-expression → function-call-expression
 postfix-expression → initializer-expression
 postfix-expression → explicit-member-expression
 postfix-expression → postfix-self-expression
 postfix-expression → dynamic-type-expression
 postfix-expression → subscript-expression
 postfix-expression → forced-value-expression
 postfix-expression → optional-chaining-expression
*/
Node* Parser::parsePostfixExpression()
{
// postfix-expression → primary-expression
// postfix-expression → postfix-expression postfix-operator
// postfix-expression → function-call-expression
// postfix-expression → initializer-expression
// postfix-expression → explicit-member-expression
// postfix-expression → postfix-self-expression
// postfix-expression → dynamic-type-expression
// postfix-expression → subscript-expression
// postfix-expression → forced-value-expression
// postfix-expression → optional-chaining-expression
}

/*
GRAMMAR OF A PRIMARY EXPRESSION
 primary-expression → identifier generic-argument-clauseopt
 primary-expression → literal-expression
 primary-expression → self-expression
 primary-expression → superclass-expression
 primary-expression → closure-expression
 primary-expression → parenthesized-expression
 primary-expression → implicit-member-expression
 primary-expression → wildcard-expression
*/
Node* Parser::parsePrimaryExpression()
{
    Token token;
    tassert(token, false);
}
/*
 literal-expression → literal
 literal-expression → array-literal dictionary-literal
 literal-expression → __FILE__ __LINE__ __COLUMN__ __FUNCTION__
 array-literal → [ array-literal-items opt ]
 array-literal-items → array-literal-item ,opt
 array-literal-item , array-literal-items
 array-literal-item → expression
 dictionary-literal → [ dictionary-literal-items ] [ : ]
 dictionary-literal-item → dictionary-literal-item, opt dictionary-literal-item, dictionarya-literal-items
 dictionary-literal-item → expression : expression
*/
Node* Parser::parseLiteralExpression()
{
    Token token;
    Tokenizer::State state = tokenizer->save();
    tokenizer->next(token);
    // literal-expression → literal
    if(token.type == TokenType::Integer || token.type == TokenType::Float || token.type == TokenType::String)
    {
        tokenizer->restore(token);
        return parseLiteral();
    }
    // literal-expression → array-literal dictionary-literal

    if(token.type == TokenType::OpenBracket)
    {
        Node* tmp = parseExpression();
        if(tmp
        delete tmp;

    }
    
}
Node* Parser::parseLiteral()
{
    Token token;
    tokenizer->next(token);
    Node* ret = NULL;
    switch(token.type)
    {
        case TokenType::Integer:
            ret = nodeFactory->createInteger(token.c_str());
            break;
        case TokenType::String:
            ret = nodeFactory->createString(token.c_str());
            break;
        case TokenType::Float:
            ret = nodeFactory->createFloat(token.c_str());
            break;
        default:
            tassert(token, false);
            break;
    }
    return ret;
}
std::pair<Node*, Node*> Parser::parseDictionaryLiteralItem()
{
    Node* key = parseExpression();
    match(L":");
    Node* value = parseExpression();
    return std::make_pair(key, value);
}
Node* Parser::parseExpression()
{
    return parsePrefixExpression();
}
