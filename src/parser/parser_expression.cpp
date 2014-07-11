#include "parser.h"
#include "parser_details.h"
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

Expression* Parser::parseFloat()
{
    Token token;
    expect_next(token);
    Expression* ret = nodeFactory->createFloat(token.state, token.token);
    return ret;
}
Expression* Parser::parseInteger()
{
    Token token;
    expect_next(token);
    Expression* ret = nodeFactory->createInteger(token.state, token.token);
    return ret;
}
Expression* Parser::parseString()
{
    Token token;
    expect_next(token);
    Expression* ret = nodeFactory->createString(token.state, token.token);
    return ret;
}
/*
 GRAMMAR OF A PREFIX EXPRESSION
 prefix-expression → prefix-operator opt postfix-expression
 prefix-expression → in-out-expression
 in-out-expression → & identifier
 */
Expression* Parser::parsePrefixExpression()
{
    Token token;
    if(match(L"&"))
    {
        //in-out-expression → & identifier
        expect_identifier(token);
        Identifier* identifier = nodeFactory->createIdentifier(token.state, token.token);
        InOutParameter* ret = nodeFactory->createInOutParameter(token.state, identifier);
        return ret;
    }
    
    expect_next(token);
    if(symbolRegistry->isPrefixOperator(token.token))
    {
        Expression* postfixExpression = parsePostfixExpression();
        UnaryOperator* op = nodeFactory->createUnary(token.state, token.token, token.operators.type);
        op->setOperand(postfixExpression);
        return op;
    }
    restore(token);
    Expression* ret = parsePostfixExpression();
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
Expression* Parser::parsePostfixExpression()
{
    Token token;
    // postfix-expression → primary-expression
    Expression* ret =  parsePrimaryExpression();
    while(peek(token))
    {
        if(token.type == TokenType::Operator)
        {
            if(token == L".")
            {
                expect_next(token);//skip .
                expect_next(token);//read identifier
                tassert(token, token.type == TokenType::Identifier);
                
                // postfix-expression → initializer-expression
                if(token == L"init")
                {
                    ret = nodeFactory->createInitializerReference(token.state, ret);
                    continue;
                }
                // postfix-expression → postfix-self-expression
                if(token.identifier.keyword == Keyword::Self)
                {
                    ret = nodeFactory->createSelfExpression(token.state, ret);
                    continue;
                }
                // postfix-expression → dynamic-type-expression
                if(token.identifier.keyword == Keyword::DynamicType)
                {
                    ret = nodeFactory->createDynamicType(token.state, ret);
                    continue;
                }
                tassert(token, token.identifier.keyword == Keyword::_);
                // postfix-expression → explicit-member-expression
                Identifier* field = nodeFactory->createIdentifier(token.state, token.token);
                ret = nodeFactory->createMemberAccess(token.state, ret, field);
                continue;
            }
            if(token == L"!")
            {
                // postfix-expression → forced-value-expression
                expect_next(token);
                ret = nodeFactory->createForcedValue(token.state, ret);
                continue;
            }
            // postfix-expression → postfix-expression postfix-operator
            if(symbolRegistry->isPostfixOperator(token.token))
            {
                expect_next(token);
                UnaryOperator* postfix = nodeFactory->createUnary(token.state, token.token, OperatorType::PostfixUnary);
                postfix->setOperand(ret);
                ret = postfix;
                continue;
            }
            break;
        }
        if(token.type == TokenType::Optional && token.operators.type == OperatorType::PostfixUnary)
        {
            //? used as post unary operator will treated as optional chaining expression, not ternary expression
            // postfix-expression → optional-chaining-expression
            expect_next(token);
            ret = nodeFactory->createOptionalChaining(token.state, ret);
            continue;
        }
        // postfix-expression → function-call-expression
        if(token.type == TokenType::OpenParen || token.type == TokenType::OpenBrace)
        {
            bool suppressTrailingClosure = (this->flags & SUPPRESS_TRAILING_CLOSURE) != 0;
            if(!(token.type == TokenType::OpenBrace && suppressTrailingClosure))
            {
                FunctionCall* call = parseFunctionCallExpression();
                call->setFunction(ret);
                ret = call;
                continue;
            }
        }
        // postfix-expression → subscript-expression
        if(token.type == TokenType::OpenBracket)
        {
            // subscript-expression → postfix-expression[expression-list]
            match(L"[", token);
            Expression* expr = parseExpression();
            SubscriptAccess* subscript = nodeFactory->createSubscriptAccess(token.state, ret, expr);
            while(match(L","))
            {
                if(predicate(L"]"))
                    break;
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
    FunctionCall* ret = nodeFactory->createFunctionCall(tokenizer->save());
    if(predicate(L"("))
    {
        ParenthesizedExpression* p = static_cast<ParenthesizedExpression*>(this->parseParenthesizedExpression());
        ret->setArguments(p);
    }
    if(predicate(L"{"))
    {
        Closure* closure = parseClosureExpression();
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
Expression* Parser::parsePrimaryExpression()
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
            case Keyword::Self:
                // primary-expression → self-expression
                return parseSelfExpression();
            case Keyword::Super:
                // primary-expression → superclass-expression
                return parseSuperExpression();
            default:
                break;
        }
        Identifier* identifier = parseIdentifier();
        return identifier;
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
    
    // primary-expression → parenthesized-expression
    if(token.type == TokenType::OpenParen)
        return parseParenthesizedExpression();
    // primary-expression → wildcard-expression
    if(token == L"_")
    {
        expect_next(token);
        return nodeFactory->createIdentifier(token.state, L"_");
    }
    
    // primary-expression → closure-expression
    if(token.type == TokenType::OpenBrace)
    {
        return parseClosureExpression();
    }
    
    // primary-expression → implicit-member-expression
    if(token == L".")
    {
        expect_next(token);
        expect_identifier(token);
        Identifier* field = nodeFactory->createIdentifier(token.state, token.token);
        return nodeFactory->createMemberAccess(token.state, NULL, field);
    }
    return NULL;
}

bool Parser::isGenericArgument()
{
    /*
    if(!(this->flags & ENABLE_GENERIC))
        return false;
    */
    Token token, t;
    if(!match(L"<", token))
    {
        restore(token);
        return false;
    }
    int nestedLevel = 1;
    bool ret = false;
    while(true)
    {
        if(!next(t))
            break;
        if(t == L"<")
            nestedLevel ++;
        else if(t == L">")
        {
            nestedLevel --;
            if(nestedLevel == 0)
            {
                ret = true;
                break;
            }
        }
        else if(t.type == TokenType::OpenBrace)
            break;
    }
    restore(token);
    return ret;
}
/*
  GRAMMAR OF A GENERIC ARGUMENT CLAUSE
 
 ‌ generic-argument-clause → <generic-argument-list>
 ‌ generic-argument-list → generic-argument | generic-argument,generic-argument-list
 ‌ generic-argument → type
*/
GenericArgument* Parser::parseGenericArgument()
{
    Token token;
    expect(L"<", token);
    GenericArgument* ret = nodeFactory->createGenericArgument(token.state);
    do
    {
        TypeNode* argument = parseType();
        ret->addArgument(argument);
    } while(match(L","));
    expect(L">");
    return ret;
}



/*
 GRAMMAR OF A PARENTHESIZED EXPRESSION
 
 ‌ parenthesized-expression → (expression-element-list opt)
 ‌ expression-element-list → expression-element | expression-element,expression-element-list
 ‌ expression-element → expression | identifier:expression
 */
Expression* Parser::parseParenthesizedExpression()
{
    Token token;
    match(L"(", token);
    ParenthesizedExpression* ret = nodeFactory->createParenthesizedExpression(token.state);
    if(!predicate(L")"))
    {
        parseExpressionItem(ret);
    }
    while(match(L","))
    {
        if(predicate(L")"))
            break;
        parseExpressionItem(ret);
    }
    match(L")");
    return ret;
}

void Parser::parseExpressionItem(ParenthesizedExpression* parent)
{
    Token token;
    expect_next(token);
    if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::_ && match(L":"))
    {
        //identifier:expression
        Expression* expr = parseExpression();
        parent->append(token.token, expr);
        return;
    }
    //rollback and parse exception
    restore(token);
    Expression* expr = parseExpression();
    parent->append(expr);
}

/*
 GRAMMAR OF A SELF EXPRESSION
 
 ‌ self-expression → self
 ‌ self-expression → self.identifier
 ‌ self-expression → self[expression]
 ‌ self-expression → self.init
 */
Expression* Parser::parseSelfExpression()
{
    Token token;
    expect(Keyword::Self);
    expect_next(token);
    Identifier* self = nodeFactory->createIdentifier(token.state, L"self");
    if(token == L".")
    {
        expect_next(token);
        tassert(token, token.type == TokenType::Identifier);
        if(token.identifier.keyword != Keyword::_ && token.identifier.keyword != Keyword::Init)
            unexpected(token);
        Identifier* field = nodeFactory->createIdentifier(token.state, token.token);
        MemberAccess* ret = nodeFactory->createMemberAccess(token.state, self, field);
        return ret;
    }
    else if(token == L"[")
    {
        Expression* expr = this->parseExpression();
        SubscriptAccess* sub = nodeFactory->createSubscriptAccess(token.state, self, expr);
        return sub;
    }
    else
    {
        restore(token);
        return self;
    }
}
/*
 superclass-expression → superclass-method-expression superclass-subscript-expression  superclass-initializer-expression
 ‌ superclass-method-expression → super.identifier
 ‌ superclass-subscript-expression → super[expression]
 ‌ superclass-initializer-expression → super.init
 */
Expression* Parser::parseSuperExpression()
{
    Token token;
    expect(Keyword::Super, token);
    Identifier* super = nodeFactory->createIdentifier(token.state, L"super");
    expect_next(token);
    if(token == L".")
    {
        expect_next(token);
        if(token.type != TokenType::Identifier)
            unexpected(token);
        if(token.identifier.keyword != Keyword::_ && token.identifier.keyword != Keyword::Init)
            unexpected(token);
        Identifier* field = nodeFactory->createIdentifier(token.state, token.token);
        MemberAccess* ret = nodeFactory->createMemberAccess(token.state, super, field);
        return ret;
    }
    else if(token == L"[")
    {
        Expression* expr = this->parseExpression();
        expect(L"]", token);
        SubscriptAccess* sub = nodeFactory->createSubscriptAccess(token.state, super, expr);
        return sub;
    }
    unexpected(token);
    return NULL;
}

Identifier* Parser::parseIdentifier()
{
    Token token;
    expect_identifier(token);
    Identifier* ret = nodeFactory->createIdentifier(token.state, token.token);
    
    if(isGenericArgument())
    {
        GenericArgument* arg = static_cast<GenericArgument*>(parseGenericArgument());
        ret->setGenericArgument(arg);
    }
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
Expression* Parser::parseLiteralExpression()
{
    Token token;
    expect_next(token);
    // literal-expression → literal
    if(token.type == TokenType::Integer || token.type == TokenType::Float || token.type == TokenType::String)
    {
        restore(token);
        return parseLiteral();
    }
    // literal-expression → array-literal dictionary-literal
    
    if(token.type == TokenType::OpenBracket)
    {
        expect_next(token);
        if(token.type == TokenType::CloseBracket)
        {
            //[] detected, empty array
            return nodeFactory->createArrayLiteral(token.state);
        }
        if(token.type == TokenType::Colon)
        {
            //[: detected, empty dictionary
            expect(L"]", token);
            return nodeFactory->createDictionaryLiteral(token.state);
        }
        restore(token);
        //check if there's a colon after an expression
        
        Expression* tmp = parseExpression();
        tassert(token, tmp != NULL);
        peek(token);
        if(token.type == TokenType::Comma || token.type == TokenType::CloseBracket)//array
        {
            ArrayLiteral* array = nodeFactory->createArrayLiteral(token.state);
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
            DictionaryLiteral* dict = nodeFactory->createDictionaryLiteral(token.state);
            Expression* key = tmp;
            Expression* value = parseExpression();
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
                return nodeFactory->createCompilecConstant(token.state, L"__FILE__", fileName);
            case Keyword::Line:
            {
                std::wstringstream ss;
                ss<<token.state.column;
                return nodeFactory->createCompilecConstant(token.state, L"__LINE__", ss.str());
            }
            case Keyword::Column:
            {
                std::wstringstream ss;
                ss<<token.state.line;
                return nodeFactory->createCompilecConstant(token.state, L"__COLUMN__", ss.str());
            }
            case Keyword::Function:
                return nodeFactory->createCompilecConstant(token.state, L"__FUNCTION__", functionName);
            default:
                break;
        }
    }
    
    unexpected(token);
    return NULL;
}
Expression* Parser::parseLiteral()
{
    Token token;
    expect_next(token);
    Expression* ret = NULL;
    switch(token.type)
    {
        case TokenType::Integer:
            ret = nodeFactory->createInteger(token.state, token.token);
            break;
        case TokenType::String:
            ret = nodeFactory->createString(token.state, token.token);
            break;
        case TokenType::Float:
            ret = nodeFactory->createFloat(token.state, token.token);
            break;
        default:
            unexpected(token);
            break;
    }
    return ret;
}
std::pair<Expression*, Expression*> Parser::parseDictionaryLiteralItem()
{
    Expression* key = parseExpression();
    expect(L":");
    Expression* value = parseExpression();
    return std::make_pair(key, value);
}

static inline bool isBinaryExpr(const Token& token)
{
    if(token.type == TokenType::Identifier)
    {
        return token.identifier.keyword == Keyword::Is || token.identifier.keyword == Keyword::As;
    }
    if(token.type == TokenType::Optional && token.operators.type == OperatorType::InfixBinary)
        return true;
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
static int rotateRequired(Operator* lhs, Operator* rhs)
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
static Operator* sortExpression(Operator* op)
{
    Operator* root = op;
    Operator* c;
    if(op->numChildren() != 2)
        return op;
    Node* left = op->get(0);
    Node* right = op->get(1);
    if(left && (c = dynamic_cast<Operator*>(left)) && c->numChildren() == 2)
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
            Node* r2 = sortExpression(static_cast<Operator*>(c->get(1)));
            c->set(1, r2);
            return root;
        }
    }
    //sort old right child
    c = dynamic_cast<Operator*>(right);
    if(c != NULL)
        sortExpression(c);
    return root;
}
/*
 expression → prefix-expression binary-expressions opt
 ‌ expression-list → expression | expression,expression-list
 */
Expression* Parser::parseExpression()
{
    Token token;
    Expression* ret = parsePrefixExpression();
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
    if(Operator* op = dynamic_cast<Operator*>(ret))
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
Expression* Parser::parseBinaryExpression(Expression* lhs)
{
    Token token;
    expect_next(token);
    if(token.type == TokenType::Identifier)
    {
        if(token.identifier.keyword == Keyword::Is)
        {
            TypeNode* typeNode = parseType();
            Expression* ret = nodeFactory->createTypeCheck(token.state, lhs, typeNode);
            return ret;
        }
        else if(token.identifier.keyword == Keyword::As)
        {
            bool optional = false;
            Token t;
            if(match(L"?", t))
            {
                if(t.operators.type == OperatorType::PostfixUnary)
                {
                    optional = true;
                }
                else
                    restore(t);
            }
            
            TypeNode* typeNode = parseType();
            
            TypeCast* ret = nodeFactory->createTypeCast(token.state, lhs, typeNode);
            ret->setOptional(optional);
            return ret;
        }
    }
    if(token.type == TokenType::Operator)
    {
        if(token == L"=")
        {
            Expression* rhs = parsePrefixExpression();
            Expression* ret = nodeFactory->createAssignment(token.state, lhs, rhs);
            return ret;
        }
        
        if(token.operators.type == OperatorType::InfixBinary)
        {
            OperatorInfo* op = symbolRegistry->getOperator(token.token);
            tassert(token, op != NULL);
            Expression* rhs = parsePrefixExpression();
            int precedence = op->precedence.infix > 0 ? op->precedence.infix : 100;
            BinaryOperator* ret = nodeFactory->createBinary(token.state, op->name, op->associativity, precedence);
            ret->setLHS(lhs);
            ret->setRHS(rhs);
            return ret;
        }
    }
    if(token.type == TokenType::Optional)
    {
        // binary-expression → conditional-operator prefix-expression
        Expression* expr = parseExpression();
        expect(L":");
        Expression* expr2 = parsePrefixExpression();
        Expression* ret = nodeFactory->createConditionalOperator(token.state, lhs, expr, expr2);
        return ret;
    }
    unexpected(token);
    return NULL;
}
/*
  GRAMMAR OF A CLOSURE EXPRESSION
 
 ‌ closure-expression → {closure-signature opt statements}
 ‌ closure-signature → parameter-clause function-result opt in
 ‌ closure-signature → identifier-list function-result opt in
 ‌ closure-signature → capture-list parameter-clause function-result opt in
 ‌ closure-signature → capture-list identifier-list function-result opt in
 ‌ closure-signature → capture-list in
 ‌ capture-list → [capture-specifier expression]
 ‌ capture-specifier → weak  unowned  unowned(safe) unowned(unsafe)
*/
Closure* Parser::parseClosureExpression()
{
    Token token;
    expect(L"{", token);
    Closure* ret = nodeFactory->createClosure(token.state);
    TokenizerState s = tokenizer->save();
    bool hasSignature = false;
    //look for keyword "in"
    while(next(token))
    {
        Keyword::T keyword = token.getKeyword();
        if(keyword == Keyword::In)
        {
            hasSignature = true;
            break;
        }
        if(token == L"}")
            break;
    }
    tokenizer->restore(s);
    if(hasSignature)
    {
        if(match(L"["))
        {
            //read capture-list
            expect_next(token);
            Keyword::T k = token.getKeyword();
            tassert(token, k == Keyword::Weak || k == Keyword::Unowned || k == Keyword::Unowned_safe || k == Keyword::Unowned_unsafe);
            switch(token.getKeyword())
            {
                case Keyword::Weak:
                    ret->setCaptureSpecifier(Closure::Weak);
                    break;
                case Keyword::Unowned:
                    ret->setCaptureSpecifier(Closure::Unowned);
                    break;
                case Keyword::Unowned_safe:
                    ret->setCaptureSpecifier(Closure::Unowned_Safe);
                    break;
                case Keyword::Unowned_unsafe:
                    ret->setCaptureSpecifier(Closure::Unowned_Unsafe);
                    break;
                default:
                    unexpected(token);
                    break;
            }
            Expression* capture = parseExpression();
            ret->setCapture(capture);
            expect(L"]");
        }
        if(predicate(L"("))
        {
            Parameters* params = parseParameterClause();
            ret->setParameters(params);
        }
        else
        {
            Parameters* params = nodeFactory->createParameters(tokenizer->save());
            do
            {
                expect_identifier(token);
                Parameter* param = nodeFactory->createParameter(token.state);
                param->setLocalName(token.token);
                params->addParameter(param);
            } while (match(L","));
            ret->setParameters(params);
        }
        if(match(L"->"))
        {
            TypeNode* t = parseType();
            ret->setReturnType(t);
        }
        expect(Keyword::In);
    }
    while(!predicate(L"}"))
    {
        Statement* s = parseStatement();
        ret->addStatement(s);
    }
    expect(L"}");
    return ret;
}

