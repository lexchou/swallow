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
#include "swift_errors.h"
using namespace Swift;


/*
 statement → expression;opt
‌ statement → declaration;opt
‌ statement → loop-statement;opt
‌ statement → branch-statement;opt
‌ statement → labeled-statement
‌ statement → control-transfer-statement;opt
‌ statements → statementstatementsopt
*/
Statement* Parser::parseStatement()
{
    Token token;
    if(peek(token))
    {
        if(token.type == TokenType::Identifier)
        {
            switch(token.getKeyword())
            {
                    //‌ statement → loop-statement;opt
                case Keyword::For:
                    return parseForLoop();
                case Keyword::While:
                    return parseWhileLoop();
                case Keyword::Do:
                    return parseDoLoop();
                    //‌ statement → branch-statement;opt
                case Keyword::If:
                    return parseIf();
                case Keyword::Switch:
                    return parseSwitch();
                    //‌ statement → control-transfer-statement;opt
                case Keyword::Break:
                    return parseBreak();
                case Keyword::Continue:
                    return parseContinue();
                case Keyword::Fallthrough:
                    return parseFallthrough();
                case Keyword::Return:
                    return parseReturn();
                    break;
                case Keyword::_:
                    expect_next(token);
                    //‌ statement → labeled-statement
                    if(predicate(L":"))
                    {
                        restore(token);
                        return parseLabeledStatement();
                    }
                    restore(token);
                    break;
                case Keyword::Import:
                case Keyword::Let:
                case Keyword::Var:
                case Keyword::Typealias:
                case Keyword::Func:
                case Keyword::Class:
                case Keyword::Enum:
                case Keyword::Struct:
                case Keyword::Protocol:
                case Keyword::Extension:
                case Keyword::Operator:
                    return parseDeclaration();
                default:
                    break;
            }
        }
        else if(token == L"@")
            return parseDeclaration();
    }
    Expression* ret = parseExpression();
    match(L";");
    return ret;
}
Statement* Parser::parseLoopStatement()
{
    Token token;
    if(peek(token))
    {
        switch(token.getKeyword())
        {
                //‌ statement → loop-statement;opt
            case Keyword::For:
                return parseForLoop();
            case Keyword::While:
                return parseWhileLoop();
            case Keyword::Do:
                return parseDoLoop();
            default:
                break;
        }
    }
    unexpected(token);
    return NULL;
}
/*
 GRAMMAR OF A FOR STATEMENT

‌ for-statement → forfor-initopt;expressionopt;expressionoptcode-block
‌ for-statement → for(for-initopt;expressionopt;expressionopt)code-block
‌ for-init → variable-declaration | expression-list
 for-in-statement → for pattern in expression code-block
*/
Statement* Parser::parseForLoop()
{
    //check if it's a for-in loop
    Token token, t;
    expect(Keyword::For, token);
    bool forIn = false;
    for(;;)
    {
        next(t);
        if(t.getKeyword() == Keyword::In)
        {
            forIn = true;
            break;
        }
        if(t.type == TokenType::OpenBrace || t.type == TokenType::Semicolon)
        {
            break;
        }
    }
    restore(token);
    if(forIn)
        return parseForIn();
    else
        return parseForStatement();
}
/*
 for-in-statement → for pattern in expression code-block
*/
Statement* Parser::parseForIn()
{
    Token token;
    expect(Keyword::For, token);
    ForInLoop* ret = nodeFactory->createForInLoop(token.state);
    Flags flags(this);
    flags += SUPPRESS_TRAILING_CLOSURE;
    Pattern* loopVars = parsePattern();
    expect(Keyword::In);
    Expression* container = parseExpression();
    flags -= SUPPRESS_TRAILING_CLOSURE;
    CodeBlock* codeBlock = parseCodeBlock();
    ret->setLoopVars(loopVars);
    ret->setContainer(container);
    ret->setCodeBlock(codeBlock);
    return ret;
}
Statement* Parser::parseForStatement()
{
    std::vector<ExpressionNode*> inits;

    //‌ for-statement → forfor-initopt;expressionopt;expressionoptcode-block
    //‌ for-statement → for(for-initopt;expressionopt;expressionopt)code-block
    Token token;
    expect(Keyword::For, token);
    Flags flags(this);
    flags += SUPPRESS_TRAILING_CLOSURE;
    ForLoop* ret = nodeFactory->createForLoop(token.state);
    bool parenthesized = match(L"(");
    if(!predicate(L";"))
    {
        do
        {
            if(predicate(L";"))
                break;
            Expression* init = parseExpression();
            ret->addInit(init);
        }
        while(match(L","));

    }
    expect(L";");
    if(!predicate(L";"))
    {
        Expression* cond = parseExpression();
        ret->setCondition(cond);
    }
    expect(L";");
    Expression* step = NULL;
    if(!predicate(L"{"))
        step = parseExpression();
    ret->setStep(step);
    
    if(parenthesized)
        expect(L")");
    CodeBlock* codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    return ret;
    
    
    //TODO: for-in-statement → forpatterninexpressioncode-block
}
/*
  GRAMMAR OF A WHILE STATEMENT
 
 ‌ while-statement → whilewhile-conditioncode-block
 ‌ while-condition → expression | declaration
*/
Statement* Parser::parseWhileLoop()
{
    Token token;
    Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
    expect(Keyword::While, token);
    WhileLoop* ret = nodeFactory->createWhileLoop(token.state);
    Expression* condition = parseConditionExpression();
    //TODO: The condition can also be an optional binding declaration, as discussed in Optional Binding.
    ret->setCondition(condition);
    CodeBlock* codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    return ret;
}
//“while-condition → expression  declaration”
Expression* Parser::parseConditionExpression()
{
    return parseExpression();
}
/*
 GRAMMAR OF A DO-WHILE STATEMENT

‌ do-while-statement → docode-blockwhilewhile-condition
*/
Statement* Parser::parseDoLoop()
{
    Token token;
    Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
    expect(Keyword::Do, token);
    DoLoop* ret = nodeFactory->createDoLoop(token.state);
    CodeBlock* codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    expect(Keyword::While);
    Expression* condition = parseConditionExpression();
    ret->setCondition(condition);
    return ret;
}
/*
  GRAMMAR OF AN IF STATEMENT
 
 ‌ if-statement → ifif-conditioncode-blockelse-clauseopt
 ‌ if-condition → expression  declaration
 ‌ else-clause → elsecode-block  elseif-statement
 */
Statement* Parser::parseIf()
{
    Token token;
    expect(Keyword::If, token);
    IfStatement* ret = nodeFactory->createIf(token.state);
    {
        Flags flags(this);
        flags += SUPPRESS_TRAILING_CLOSURE;
        Expression* cond = parseConditionExpression();
        ret->setCondition(cond);
    }
    CodeBlock* thenPart = parseCodeBlock();
    ret->setThen(thenPart);
    if(match(L"else"))
    {
        Token token;
        expect_next(token);
        restore(token);
        if(token.type == TokenType::OpenBrace)
        {
            CodeBlock* elsePart = parseCodeBlock();
            ret->setElse(elsePart);
        }
        else if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::If)
        {
            IfStatement* elseIf = static_cast<IfStatement*>(parseIf());
            ret->setElse(elseIf);
        }
        else
        {
            unexpected(token);
        }
    }
    return ret;
}
/*
  GRAMMAR OF A SWITCH STATEMENT
 
 ‌ switch-statement → switchexpression{switch-casesopt}
 ‌ switch-cases → switch-case switch-cases opt
 ‌ switch-case → case-labelstatements  |default-labelstatements
 ‌ switch-case → case-label;  default-label;
 ‌ case-label → casecase-item-list:
 ‌ case-item-list → patternguard-clauseopt  patternguard-clauseopt,case-item-list
 ‌ default-label → default:
 ‌ guard-clause → whereguard-expression
 ‌ guard-expression → expression
*/
Statement* Parser::parseSwitch()
{
    Token token;
    Flags flags(this, UNDER_SWITCH_CASE | SUPPRESS_TRAILING_CLOSURE);
    
    expect(Keyword::Switch, token);
    SwitchCase* ret = nodeFactory->createSwitch(token.state);
    {
        Flags f(flags);
        f += SUPPRESS_TRAILING_CLOSURE;
        Expression* controlExpr = parseExpression();
        ret->setControlExpression(controlExpr);
    }
    expect(L"{");
    // ‌ switch-cases → switch-case switch-cases opt
    while(!predicate(L"}"))
    {
        //switch-case → case-label statements  |default-label statements
        expect_next(token);
        tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_CASE, token.token);
        // case-label → casecase-item-list:
        if(token.identifier.keyword == Keyword::Case)
        {
            Flags fcase(flags);
            fcase += UNDER_CASE;
            
            //“case-item-list → pattern guard-clause opt | pattern guard-clause opt , case-item-list”
            CaseStatement* caseCond = nodeFactory->createCase(token.state);
            do
            {
                Pattern* pattern = parsePattern();
                Expression* guard = NULL;
                if(match(L"where"))
                    guard = parseExpression();
                caseCond->addCondition(pattern, guard);
            }
            while(match(L","));
            expect(L":");
            ret->addCase(caseCond);
            parseSwitchStatements(caseCond);
            
        }
        else if(token.identifier.keyword == Keyword::Default)
        {
            expect(L":");
            CaseStatement* caseCond = nodeFactory->createCase(token.state);
            parseSwitchStatements(caseCond);
            ret->setDefaultCase(caseCond);
        }
        else
        {
            tassert(token, false, Errors::E_EXPECT_CASE, token.token);
        }
        
    }
    expect(L"}");
    
    return ret;
}
void Parser::parseSwitchStatements(CaseStatement* case_)
{
    Token token;
    Flags flags(this);
    flags -= SUPPRESS_TRAILING_CLOSURE;
    while(true)
    {
        expect_next(token);
        restore(token);
        Keyword::T k = token.getKeyword();
        if(token.type == TokenType::CloseBrace || (k == Keyword::Case || k == Keyword::Default))
        {
            break;
        }
        Statement* st = parseStatement();
        case_->addStatement(st);
    }
        
}
/*
 GRAMMAR OF A BREAK STATEMENT

‌ break-statement → breaklabel-nameopt
*/
Statement* Parser::parseBreak()
{
    Token token;
    expect(Keyword::Break, token);
    BreakStatement* ret = nodeFactory->createBreak(token.state);
    
    if(match_identifier(token) && token.identifier.keyword == Keyword::_)
    {
        //TODO: check if the token is the same line as the break keyword
        ret->setLoop(token.token);
    }
    else
    {
        restore(token);
    }
    return ret;
}
/*
  GRAMMAR OF A CONTINUE STATEMENT
 
 ‌ continue-statement → continuelabel-nameopt
*/
Statement* Parser::parseContinue()
{
    Token token;
    expect(Keyword::Continue, token);
    ContinueStatement* ret = nodeFactory->createContinue(token.state);
    
    if(match_identifier(token) && token.identifier.keyword == Keyword::_)
    {
        //TODO: check if the token is the same line as the continue keyword
        ret->setLoop(token.token);
    }
    else
    {
        restore(token);
    }
    return ret;
    
}
/*
  GRAMMAR OF A FALLTHROUGH STATEMENT
 
 ‌ fallthrough-statement → fallthrough
*/
Statement* Parser::parseFallthrough()
{
    Token token;
    expect(Keyword::Fallthrough, token);
    FallthroughStatement* ret = nodeFactory->createFallthrough(token.state);
    return ret;
}
/*
 GRAMMAR OF A RETURN STATEMENT

‌ return-statement → returnexpressionopt
*/
Statement* Parser::parseReturn()
{
    Token token;
    expect(Keyword::Return, token);
    ReturnStatement* ret = nodeFactory->createReturn(token.state);
    
    if(peek(token))
    {
        //try to read an expression
        try {
            Expression* expr = parseExpression();
            ret->setExpression(expr);
        } catch (...) {
            //TODO: clean all node created during parsing the expression
        }
    }
    return ret;
}
/*
 GRAMMAR OF A LABELED STATEMENT

‌ labeled-statement → statement-label loop-statement | statement-label switch-statement
‌ statement-label → label-name:
‌ label-name → identifier
*/
Statement* Parser::parseLabeledStatement()
{
    Token token;
    expect_identifier(token);
    std::wstring label = token.token;
    expect(L":");
    expect_next(token);
    if(token.type == TokenType::Identifier)
    {
        restore(token);
        switch(token.identifier.keyword)
        {
            case Keyword::Switch:
            {
                Statement* statement = parseSwitch();
                LabeledStatement* ret = nodeFactory->createLabel(token.state);
                ret->setLabel(label);
                ret->setStatement(statement);
                return ret;
            }
            case Keyword::For:
            case Keyword::Do:
            case Keyword::While:
            {
                Statement* statement = parseLoopStatement();
                LabeledStatement* ret = nodeFactory->createLabel(token.state);
                ret->setLabel(label);
                ret->setStatement(statement);
                return ret;
            }
            default:
                break;
        }
    }
    unexpected(token);
    return NULL;
}

CodeBlock* Parser::parseCodeBlock()
{
    Token token;
    Flags flags(this);
    flags -= SUPPRESS_TRAILING_CLOSURE;
    expect(L"{", token);
    CodeBlock* ret = nodeFactory->createCodeBlock(token.state);
    while(!match(L"}"))
    {
        Statement* st = parseStatement();
        if(st != NULL)
            ret->addStatement(st);
    }
    return ret;
}
