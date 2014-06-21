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

/**
 * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
 */
bool Parser::match(const wchar_t* token)
{
    Token t;
    CHECK_EOF(tokenizer->next(t));
    if(!wcscmp(t.c_str(), token))
        return true;
    tokenizer->restore(t);
    return false;
}
/**
 * Return true if the next token is the specified one, no token will be consumed
 */
bool Parser::predicate(const wchar_t* token)
{
    Token t;
    CHECK_EOF(tokenizer->peek(t));
    return !wcscmp(t.c_str(), token);
}
void Parser::expect(const wchar_t* str)
{
    Token token;
    CHECK_EOF(tokenizer->next(token));
    tassert(token, wcscmp(token.c_str(), str) == 0);
}
void Parser::expect_identifier(Token& token)
{
    CHECK_EOF(tokenizer->next(token));
    tassert(token, token.type == TokenType::Identifier);
    tassert(token, token.identifier.keyword == Keyword::_);
}

/**
 * Throw an exception with the unexpected token
 */
void Parser::unexpected(const Token& token)
{
    throw 0;    
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
ExpressionNode* Parser::parseFloat()
{
    Token token;
    tokenizer->next(token);
    ExpressionNode* ret = nodeFactory->createFloat(token.c_str());
    return ret;
}
ExpressionNode* Parser::parseInteger()
{
    Token token;
    tokenizer->next(token);
    ExpressionNode* ret = nodeFactory->createInteger(token.c_str());
    return ret;
}
ExpressionNode* Parser::parseString()
{
    Token token;
    tokenizer->next(token);
    ExpressionNode* ret = nodeFactory->createString(token.c_str());
    return ret;
}
/*
 GRAMMAR OF A PREFIX EXPRESSION
 prefix-expression → prefix-operator opt postfix-expression
 prefix-expression → in-out-expression
 in-out-expression → & identifier
*/
ExpressionNode* Parser::parsePrefixExpression()
{
    Token token;
    tokenizer->next(token);

    if(token.type == TokenType::Operator && token == L"&")
    {
         //in-out-expression → & identifier
        expect_identifier(token);
        Identifier* identifier = nodeFactory->createIdentifier(token.c_str());
        InOutParameter* ret = nodeFactory->createInOutParameter(identifier);
        return ret;
    }


    ExpressionNode* postfixExpression = parsePostfixExpression();
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
ExpressionNode* Parser::parsePostfixExpression()
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

    return parsePrimaryExpression();
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
ExpressionNode* Parser::parsePrimaryExpression()
{
    Token token;
    CHECK_EOF(tokenizer->peek(token));
    if(token.type == TokenType::Identifier)
    {
        //primary-expression → identifier generic-argument-clauseopt
        switch(token.identifier.keyword)
        {
            case Keyword::File:
            case Keyword::Line:
            case Keyword::Column:
            case Keyword::Function:
                return parseLiteralExpression();
            default:
                break;
        }
        Identifier* identifier = parseIdentifier();
        if(!predicate(L"<"))
            return identifier;
        tassert(token, false);//TODO unsupported genetic expression
        return NULL;
        //GeneticArgument* geneticArgument = parseGenericArgument();
        //GeneticType* geneticType = nodeFactory->createGeneticType(identifier, geneticArgument);
        //return geneticType;
    }
    // primary-expression → literal-expression
    switch(token.type)
    {
        case TokenType::Integer:
        case TokenType::Float:
        case TokenType::String:
        case TokenType::OpenBracket:
            return parseLiteralExpression();
        default:
            break;
    }

    
    return NULL;
}

Identifier* Parser::parseIdentifier()
{
    Token token;
    expect_identifier(token);
    Identifier* ret = nodeFactory->createIdentifier(token.c_str());
    return ret;
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
ExpressionNode* Parser::parseLiteralExpression()
{
    Token token;
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
        tokenizer->next(token);
        if(token.type == TokenType::CloseBracket)
        {
            //[] detected, empty array
            return nodeFactory->createArrayLiteral();
        }
        if(token.type == TokenType::Colon)
        {
            //[: detected, empty dictionary
            expect(L"]");
            return nodeFactory->createDictionaryLiteral();
        }
        tokenizer->restore(token);
        //check if there's a colon after an expression
        
        ExpressionNode* tmp = parseExpression();
        tassert(token, tmp != NULL);
        tokenizer->peek(token);
        if(token.type == TokenType::Comma || token.type == TokenType::CloseBracket)//array
        {
            ArrayLiteral* array = nodeFactory->createArrayLiteral();
            array->push(tmp);
            while(match(L","))
            {
                if(predicate(L"]"))
                    break;
                tmp = parseExpression();
                array->push(tmp);
            }
            expect(L"]");
            return array;

        }
        else if(token.type == TokenType::Colon)//dictionary
        {
            DictionaryLiteral* dict = nodeFactory->createDictionaryLiteral();
            ExpressionNode* key = tmp;
            ExpressionNode* value = parseExpression();
            tassert(token, value != NULL);
            dict->insert(key, value);
            while(match(L","))
            {
                if(predicate(L"]"))
                    break;
                key = parseExpression();
                expect(L":");
                value = parseExpression();
                dict->insert(key, value);
            }
            expect(L"]");
            return dict;
        }
    }
    unexpected(token);
    return NULL;
}
ExpressionNode* Parser::parseLiteral()
{
    Token token;
    tokenizer->next(token);
    ExpressionNode* ret = NULL;
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
            unexpected(token);
            break;
    }
    return ret;
}
std::pair<ExpressionNode*, ExpressionNode*> Parser::parseDictionaryLiteralItem()
{
    ExpressionNode* key = parseExpression();
    expect(L":");
    ExpressionNode* value = parseExpression();
    return std::make_pair(key, value);
}
ExpressionNode* Parser::parseExpression()
{
    return parsePrefixExpression();
}
