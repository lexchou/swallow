#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
#include <sstream>
using namespace Swift;

#define CHECK_EOF(x) x

Parser::Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry)
    :nodeFactory(nodeFactory), symbolRegistry(symbolRegistry)
{
    tokenizer = new Tokenizer(NULL);
    functionName = L"<top>";
}
Parser::~Parser()
{
    delete tokenizer;
}

void Parser::setFileName(const wchar_t* fileName)
{
    this->fileName = fileName;
}
void Parser::setFunctionName(const wchar_t* function)
{
    this->functionName = functionName;
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
    if(match(L"&"))
    {
         //in-out-expression → & identifier
        expect_identifier(token);
        Identifier* identifier = nodeFactory->createIdentifier(token.c_str());
        InOutParameter* ret = nodeFactory->createInOutParameter(identifier);
        return ret;
    }

    CHECK_EOF(tokenizer->next(token));
    if(symbolRegistry->isPrefixOperator(token.c_str()))
    {
        ExpressionNode* postfixExpression = parsePostfixExpression();
        UnaryOperator* op = nodeFactory->createUnary(token.c_str(), postfixExpression);
        return op;
    }
    tokenizer->restore(token);
    ExpressionNode* ret = parsePostfixExpression();
    return ret;
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
    return parsePrimaryExpression();
    // postfix-expression → postfix-expression postfix-operator
    //TODO
    // postfix-expression → function-call-expression
    //TODO
    // postfix-expression → initializer-expression
    //TODO
    // postfix-expression → explicit-member-expression
    //TODO
    // postfix-expression → postfix-self-expression
    //TODO
    // postfix-expression → dynamic-type-expression
    //TODO
    // postfix-expression → subscript-expression
    //TODO
    // postfix-expression → forced-value-expression
    //TODO
    // postfix-expression → optional-chaining-expression
    //TODO

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
    // primary-expression → self-expression
    if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::Self)
        return parseSelfExpression();
    // primary-expression → superclass-expression
    if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::Super)
        return parseSuperExpression();
    // primary-expression → closure-expression
    //TODO : not implemeneted
    
    // primary-expression → parenthesized-expression
    //TODO : not implemeneted
    
    // primary-expression → implicit-member-expression
    //TODO : not implemeneted
    
    // primary-expression → wildcard-expression
    if(token == L"_")
    {
        tokenizer->next(token);
        return nodeFactory->createIdentifier(L"_");
    }
    unexpected(token);
    return NULL;
}
/*
  GRAMMAR OF A SELF EXPRESSION
 
 ‌ self-expression → self
 ‌ self-expression → self.identifier
 ‌ self-expression → self[expression]
 ‌ self-expression → self.init
 */
ExpressionNode* Parser::parseSelfExpression()
{
    Token token;
    this->expect_identifier(token);
    CHECK_EOF(tokenizer->next(token));
    Identifier* self = nodeFactory->createIdentifier(L"self");
    if(token == L".")
    {
        expect_identifier(token);
        Identifier* field = nodeFactory->createIdentifier(token.c_str());
        MemberAccess* ret = nodeFactory->createMemberAccess(self, field);
        return ret;
    }
    else if(token == L"[")
    {
        ExpressionNode* expr = this->parseExpression();
        Subscript* sub = nodeFactory->createSubscript(self, expr);
        return sub;
    }
    else
    {
        tokenizer->restore(token);
        return self;
    }
}
/*
 superclass-expression → superclass-method-expression superclass-subscript-expression  superclass-initializer-expression
‌ superclass-method-expression → super.identifier
‌ superclass-subscript-expression → super[expression]
‌ superclass-initializer-expression → super.init
*/
ExpressionNode* Parser::parseSuperExpression()
{
    Token token;
    this->expect_identifier(token);
    CHECK_EOF(tokenizer->next(token));
    Identifier* super = nodeFactory->createIdentifier(L"super");
    if(token == L".")
    {
        expect_identifier(token);
        Identifier* field = nodeFactory->createIdentifier(token.c_str());
        MemberAccess* ret = nodeFactory->createMemberAccess(super, field);
        return ret;
    }
    else if(token == L"[")
    {
        ExpressionNode* expr = this->parseExpression();
        Subscript* sub = nodeFactory->createSubscript(super, expr);
        return sub;
    }
    unexpected(token);
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
            match(L":");
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
        unexpected(token);
    }
    
    if(token.type == TokenType::Identifier)
    {
        switch(token.identifier.keyword)
        {
            case Keyword::File:
                return nodeFactory->createCompilecConstant(L"__FILE__", fileName);
            case Keyword::Line:
            {
                std::wstringstream ss;
                ss<<token.state.column;
                return nodeFactory->createCompilecConstant(L"__LINE__", ss.str());
            }
            case Keyword::Column:
            {
                std::wstringstream ss;
                ss<<token.state.line;
                return nodeFactory->createCompilecConstant(L"__COLUMN__", ss.str());
            }
            case Keyword::Function:
                return nodeFactory->createCompilecConstant(L"__FUNCTION__", functionName);
            default:
                break;
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

/*
 expression → prefix-expression binary-expressions opt
‌ expression-list → expression  expression,expression-list
*/
ExpressionNode* Parser::parseExpression()
{
    Token token;
    ExpressionNode* lhs = parsePrefixExpression();
    CHECK_EOF(tokenizer->next(token));
    std::wstring op = token.c_str();
    if(token.type == TokenType::Operator && token.operators.type == OperatorType::InfixBinary)
    {
        ExpressionNode* rhs = parsePostfixExpression();
        BinaryOperator* ret = nodeFactory->createBinary(op, lhs, rhs);
        return ret;
    }
    tokenizer->restore(token);
    return lhs;
}
