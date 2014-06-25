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
                    next(token);
                    //‌ statement → labeled-statement
                    if(predicate(L":"))
                        return parseLabeledStatement();
                    tokenizer->restore(token);
                    break;
                default:
                    break;
            }
        }
        
    }
    ExpressionNode* ret = parseExpression();
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
 for-in-statement → forpatterninexpressioncode-block
*/
Statement* Parser::parseForLoop()
{
    std::vector<ExpressionNode*> inits;
    ForLoop* ret = nodeFactory->createForLoop();

    //‌ for-statement → forfor-initopt;expressionopt;expressionoptcode-block
    //‌ for-statement → for(for-initopt;expressionopt;expressionopt)code-block

    expect(Keyword::For);
    bool parenthesized = match(L"(");
    if(!predicate(L";"))
    {
        while(match(L","))
        {
            if(predicate(L";"))
                break;
            ExpressionNode* init = parseExpression();
            ret->addInit(init);
        }
    }
    expect(L";");
    if(!predicate(L";"))
    {
        ExpressionNode* cond = parseExpression();
        ret->setCondition(cond);
    }
    expect(L";");
    ExpressionNode* step = parseExpression();
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
    expect(Keyword::While);
    WhileLoop* ret = nodeFactory->createWhileLoop();
    ExpressionNode* condition = parseConditionExpression();
    //TODO: The condition can also be an optional binding declaration, as discussed in Optional Binding.
    ret->setCondition(condition);
    CodeBlock* codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    return ret;
}
//“while-condition → expression  declaration”
ExpressionNode* Parser::parseConditionExpression()
{
    return parseExpression();
}
/*
 GRAMMAR OF A DO-WHILE STATEMENT

‌ do-while-statement → docode-blockwhilewhile-condition
*/
Statement* Parser::parseDoLoop()
{
    expect(Keyword::Do);
    DoLoop* ret = nodeFactory->createDoLoop();
    CodeBlock* codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    expect(Keyword::While);
    ExpressionNode* condition = parseConditionExpression();
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
    expect(Keyword::If);
    IfStatement* ret = nodeFactory->createIf();
    ExpressionNode* cond = parseConditionExpression();
    ret->setCondition(cond);
    CodeBlock* thenPart = parseCodeBlock();
    ret->setThen(thenPart);
    if(match(L"else"))
    {
        Token token;
        next(token);
        tokenizer->restore(token);
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
    expect(Keyword::Switch);
    SwitchCase* ret = nodeFactory->createSwitch();
    ExpressionNode* controlExpr = parseExpression();
    ret->setControlExpression(controlExpr);
    expect(L"{");
    // ‌ switch-cases → switch-case switch-cases opt
    while(!predicate(L"}"))
    {
        //switch-case → case-label statements  |default-label statements
        next(token);
        tassert(token, token.type == TokenType::Identifier);
        // case-label → casecase-item-list:
        if(token.identifier.keyword == Keyword::Case)
        {
            //“case-item-list → pattern guard-clause opt | pattern guard-clause opt , case-item-list”
            do
            {
                ExpressionNode* pattern = parsePattern();
                ExpressionNode* guard = NULL;
                if(match(L"where"))
                    guard = parseExpression();
                CaseStatement* caseCond = nodeFactory->createCase();
                caseCond->setCondition(pattern);
                caseCond->setGuard(guard);
                parseSwitchStatements(caseCond);
                ret->addCase(caseCond);
            }
            while(match(L","));
            
        }
        else if(token.identifier.keyword == Keyword::Default)
        {
            CaseStatement* caseCond = nodeFactory->createCase();
            caseCond->setCondition(NULL);
            parseSwitchStatements(caseCond);
            ret->setDefaultCase(caseCond);
        }
        else
        {
            unexpected(token);
        }
        
    }
    expect(L"}");
    
    return ret;
}
void Parser::parseSwitchStatements(CaseStatement* case_)
{
    Token token;
    while(true)
    {
        next(token);
        if(token.type == TokenType::CloseBrace)
            break;
        Keyword::T k = token.getKeyword();
        if(k == Keyword::Case || k == Keyword::Default)
        {
            tokenizer->restore(token);
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
    expect(Keyword::Break);
    BreakStatement* ret = nodeFactory->createBreak();
    
    if(match_identifier(token) && token.identifier.keyword == Keyword::_)
    {
        //TODO: check if the token is the same line as the break keyword
        ret->setLoop(token.token);
    }
    else
    {
        tokenizer->restore(token);
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
    expect(Keyword::Continue);
    ContinueStatement* ret = nodeFactory->createContinue();
    
    if(match_identifier(token) && token.identifier.keyword == Keyword::_)
    {
        //TODO: check if the token is the same line as the continue keyword
        ret->setLoop(token.token);
    }
    else
    {
        tokenizer->restore(token);
    }
    return ret;
    
}
/*
  GRAMMAR OF A FALLTHROUGH STATEMENT
 
 ‌ fallthrough-statement → fallthrough
*/
Statement* Parser::parseFallthrough()
{
    expect(Keyword::Fallthrough);
    FallthroughStatement* ret = nodeFactory->createFallthrough();
    return ret;
}
/*
 GRAMMAR OF A RETURN STATEMENT

‌ return-statement → returnexpressionopt
*/
Statement* Parser::parseReturn()
{
    Token token;
    expect(Keyword::Continue);
    ReturnStatement* ret = nodeFactory->createReturn();
    
    if(peek(token))
    {
        //try to read an expression
        try {
            ExpressionNode* expr = parseExpression();
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
    next(token);
    if(token.type == TokenType::Identifier)
    {
        switch(token.identifier.keyword)
        {
            case Keyword::Switch:
            {
                Statement* statement = parseSwitch();
                LabeledStatement* ret = nodeFactory->createLabel();
                ret->setLabel(label);
                ret->setStatement(statement);
                return ret;
            }
            case Keyword::For:
            case Keyword::Do:
            case Keyword::While:
            {
                Statement* statement = parseLoopStatement();
                LabeledStatement* ret = nodeFactory->createLabel();
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
    expect(L"{");
    CodeBlock* ret = nodeFactory->createCodeBlock();
    while(!match(L"}"))
    {
        Statement* st = parseStatement();
        ret->addStatement(st);
    }
    return ret;
}
