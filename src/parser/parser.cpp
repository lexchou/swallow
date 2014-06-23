#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
using namespace Swift;

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
 * Read next token from tokenizer, throw exception if EOF reached.
 */
void Parser::next(Token& token)
{
    if(tokenizer->next(token))
        return;
    throw 0;
}
/**
 * Peek next token from tokenizer, return false if EOF reached.
 */
bool Parser::peek(Token& token)
{
    token.type = TokenType::_;
    return tokenizer->peek(token);
}
/**
 * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
 */
bool Parser::match(const wchar_t* token)
{
    Token t;
    next(t);
    if(t == token)
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
    if(!peek(t))
        return false;
    return t == token;
}
void Parser::expect(const wchar_t* str)
{
    Token token;
    next(token);
    tassert(token, token == str);
}
void Parser::expect_identifier(Token& token)
{
    next(token);
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
    next(token);
    ExpressionNode* ret = nodeFactory->createFloat(token.token);
    return ret;
}
ExpressionNode* Parser::parseInteger()
{
    Token token;
    next(token);
    ExpressionNode* ret = nodeFactory->createInteger(token.token);
    return ret;
}
ExpressionNode* Parser::parseString()
{
    Token token;
    next(token);
    ExpressionNode* ret = nodeFactory->createString(token.token);
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
        Identifier* identifier = nodeFactory->createIdentifier(token.token);
        InOutParameter* ret = nodeFactory->createInOutParameter(identifier);
        return ret;
    }

    next(token);
    if(symbolRegistry->isPrefixOperator(token.token))
    {
        ExpressionNode* postfixExpression = parsePostfixExpression();
        UnaryOperator* op = nodeFactory->createUnary(token.token, token.operators.type);
        op->setOperand(postfixExpression);
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
    Token token;
    // postfix-expression → primary-expression
    ExpressionNode* ret =  parsePrimaryExpression();
    while(peek(token))
    {
        if(token.type == TokenType::Operator)
        {
            if(token == L".")
            {
                next(token);
                // postfix-expression → initializer-expression
                if(token == L"init")
                {
                    ret = nodeFactory->createInitializer(ret);
                    continue;
                }
                // postfix-expression → postfix-self-expression
                tassert(token, token.type == TokenType::Identifier);
                if(token.identifier.keyword == Keyword::Self)
                {
                    ret = nodeFactory->createSelfExpression(ret);
                    continue;
                }
                // postfix-expression → dynamic-type-expression
                if(token.identifier.keyword == Keyword::DynamicType)
                {
                    ret = nodeFactory->createDynamicType(ret);
                    continue;
                }
                tassert(token, token.identifier.keyword == Keyword::_);
                // postfix-expression → explicit-member-expression
                Identifier* field = nodeFactory->createIdentifier(token.token);
                ret = nodeFactory->createMemberAccess(ret, field);
                continue;
            }
            if(token == L"!")
            {
                // postfix-expression → forced-value-expression
                ret = nodeFactory->createForcedValue(ret);
                continue;
            }
            if(token == L"?")
            {
                // postfix-expression → optional-chaining-expression
                ret = nodeFactory->createOptionalChaining(ret);
                continue;
            }
            // postfix-expression → postfix-expression postfix-operator
            if(symbolRegistry->isPostfixOperator(token.token))
            {
                next(token);
                UnaryOperator* postfix = nodeFactory->createUnary(token.token, OperatorType::PostfixUnary);
                postfix->setOperand(ret);
                ret = postfix;
                continue;
            }
            break;
        }
        // postfix-expression → function-call-expression
        if(token.type == TokenType::OpenBrace)
        {
            FunctionCall* call = parseFunctionCallExpression();
            call->setFunction(ret);
            ret = call;
            continue;
        }
        // postfix-expression → subscript-expression
        if(token.type == TokenType::OpenBracket)
        {
            // subscript-expression → postfix-expression[expression-list]
            match(L"[");
            ExpressionNode* expr = parseExpression();
            Subscript* subscript = nodeFactory->createSubscript(ret, expr);
            while(match(L","))
            {
                expr = parseExpression();
                subscript->addIndex(expr);
            }
            match(L"]");
            ret = subscript;
            continue;
        }
        break;
    }
    return ret;
}

/*
 GRAMMAR OF A FUNCTION CALL EXPRESSION

‌ function-call-expression → postfix-expression parenthesized-expression
‌ function-call-expression → postfix-expression parenthesized-expression opt trailing-closure
‌ trailing-closure → closure-expression
*/
FunctionCall* Parser::parseFunctionCallExpression()
{
    FunctionCall* ret = nodeFactory->createFunctionCall();
    if(match(L"("))
    {
        ParenthesizedExpression* p = static_cast<ParenthesizedExpression*>(this->parseParenthesizedExpression());
        ret->setArguments(p);
        match(L")");
    }
    if(predicate(L"{"))
    {
        ClosureExpression* closure = parseClosureExpression();
        ret->setTrailingClosure(closure);
    }
    return ret;
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
    peek(token);
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

    // primary-expression → parenthesized-expression
    if(token.type == TokenType::OpenParen)
        return parseParenthesizedExpression();
    // primary-expression → wildcard-expression
    if(token == L"_")
    {
        next(token);
        return nodeFactory->createIdentifier(L"_");
    }
    
    // primary-expression → closure-expression
    //TODO : not implemeneted
    
    // primary-expression → implicit-member-expression
    //TODO : not implemeneted
    
    
    unexpected(token);
    return NULL;
}
/*
 GRAMMAR OF A PARENTHESIZED EXPRESSION

‌ parenthesized-expression → (expression-element-list opt)
‌ expression-element-list → expression-element | expression-element,expression-element-list
‌ expression-element → expression | identifier:expression
*/
ExpressionNode* Parser::parseParenthesizedExpression()
{
    Token token;
    ParenthesizedExpression* ret = nodeFactory->createParenthesizedExpression();
    match(L"(");
    while(!match(L")"))
    {
        next(token);
        if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::_ && match(L":"))
        {
            //identifier:expression
            ExpressionNode* expr = parseExpression();
            ret->append(token.token, expr);
            continue;
        }
        //rollback and parse exception
        tokenizer->restore(token);
        ExpressionNode* expr = parseExpression();
        ret->append(expr);
    }
    return ret;
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
    next(token);
    Identifier* self = nodeFactory->createIdentifier(L"self");
    if(token == L".")
    {
        expect_identifier(token);
        Identifier* field = nodeFactory->createIdentifier(token.token);
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
    next(token);
    Identifier* super = nodeFactory->createIdentifier(L"super");
    if(token == L".")
    {
        expect_identifier(token);
        Identifier* field = nodeFactory->createIdentifier(token.token);
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
    Identifier* ret = nodeFactory->createIdentifier(token.token);
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
    next(token);
    // literal-expression → literal
    if(token.type == TokenType::Integer || token.type == TokenType::Float || token.type == TokenType::String)
    {
        tokenizer->restore(token);
        return parseLiteral();
    }
    // literal-expression → array-literal dictionary-literal

    if(token.type == TokenType::OpenBracket)
    {
        next(token);
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
        peek(token);
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
    next(token);
    ExpressionNode* ret = NULL;
    switch(token.type)
    {
        case TokenType::Integer:
            ret = nodeFactory->createInteger(token.token);
            break;
        case TokenType::String:
            ret = nodeFactory->createString(token.token);
            break;
        case TokenType::Float:
            ret = nodeFactory->createFloat(token.token);
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

static inline bool isBinaryExpr(const Token& token)
{
    if(token.type == TokenType::Identifier)
    {
        return token.identifier.keyword == Keyword::Is || token.identifier.keyword == Keyword::As;
    }
    if(token.type != TokenType::Operator)
        return false;
    if(token.operators.type == OperatorType::InfixBinary)
        return true;
    if(token == L"=")
        return true;
    if(token == L"?")
        return true;
    return false;
}
/**
 * Rotate the AST tree if required
 */
static int rotateRequired(OperatorNode* lhs, OperatorNode* rhs)
{
    if(lhs->getPrecedence() == rhs->getPrecedence())
    {
        //only right associativity is required because it's left associativity
        return lhs->getAssociativity() == Associativity::Right;
    }
    return lhs->getPrecedence() < rhs->getPrecedence();
}
/**
 * Sort the AST tree by precedence, if the precedence is the same, sort by associativity
 */
static OperatorNode* sortExpression(OperatorNode* op)
{
    OperatorNode* root = op;
    OperatorNode* c;
    if(op->numChildren() != 2)
        return op;
    Node* left = op->get(0);
    Node* right = op->get(1);
    if(left && (c = dynamic_cast<OperatorNode*>(left)))
    {
        left = c = sortExpression(c);
        if(rotateRequired(c, root))
        {
            //make c the root
            Node* r = c->get(1);
            c->set(1, root);
            root->set(0, r);
            root = c;
            //sort new right child
            Node* r2 = sortExpression(static_cast<OperatorNode*>(c->get(1)));
            c->set(1, r2);
            return root;
        }
    }
    //sort old right child
    c = dynamic_cast<OperatorNode*>(right);
    if(c != NULL)
        sortExpression(c);
    return root;
}
/*
 expression → prefix-expression binary-expressions opt
‌ expression-list → expression | expression,expression-list
*/
ExpressionNode* Parser::parseExpression()
{
    Token token;
    ExpressionNode* ret = parsePrefixExpression();
    peek(token);
    
    //‌ binary-expressions → binary-expressionbinary-expressionsopt
    if(isBinaryExpr(token))
    {
        ret = parseBinaryExpression(ret);
        for(bool succ = peek(token); succ && isBinaryExpr(token); succ = peek(token))
        {
            ret = parseBinaryExpression(ret);
        }
    }
    if(OperatorNode* op = dynamic_cast<OperatorNode*>(ret))
    {
        //sort the tree by associativity and precedence
        ret = sortExpression(op);
    }
    return ret;
}
/*
  GRAMMAR OF A BINARY EXPRESSION
 
 ‌ binary-expression → binary-operatorprefix-expression
 ‌ binary-expression → assignment-operatorprefix-expression
 ‌ binary-expression → conditional-operatorprefix-expression
 ‌ binary-expression → type-casting-operator
 */
ExpressionNode* Parser::parseBinaryExpression(ExpressionNode* lhs)
{
    Token token;
    next(token);
    if(token.type == TokenType::Identifier)
    {
        if(token.identifier.keyword == Keyword::Is)
        {
            TypeNode* typeNode = parseType();
            ExpressionNode* ret = nodeFactory->createTypeCheck(lhs, typeNode);
            return ret;
        }
        else if(token.identifier.keyword == Keyword::As)
        {
            TypeNode* typeNode = parseType();
            ExpressionNode* ret = nodeFactory->createTypeCast(lhs, typeNode);
            return ret;
        }
    }
    if(token.type == TokenType::Operator)
    {
        if(token == L"=")
        {
            ExpressionNode* rhs = parsePrefixExpression();
            ExpressionNode* ret = nodeFactory->createAssignment(lhs, rhs);
            return ret;
        }
        if(token == L"?")
        {
            // binary-expression → conditional-operatorprefix-expression
            ExpressionNode* expr = parseExpression();
            ExpressionNode* ret = nodeFactory->createConditionalOperator(lhs, expr, NULL);
            return ret;
        }
        
        if(token.operators.type == OperatorType::InfixBinary)
        {
            Operator* op = symbolRegistry->getOperator(token.token);
            tassert(token, op != NULL);
            ExpressionNode* rhs = parsePrefixExpression();
            BinaryOperator* ret = nodeFactory->createBinary(op->name, op->associativity, op->precedence);
            ret->setLHS(lhs);
            ret->setRHS(rhs);
            return ret;
        }
    }
    unexpected(token);
    return NULL;
}

ClosureExpression* Parser::parseClosureExpression()
{
    Token token;
    tassert(token, 0);
    return NULL;
}

TypeNode* Parser::parseType()
{
    Token token;
    tassert(token, false);
    return NULL;//TODO not implemented
}