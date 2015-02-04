/* parser_statement.cpp --
 *
 * Copyright (c) 2014, Lex Chou <lex at chou dot it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Swallow nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "parser/Parser.h"
#include "parser/Parser_Details.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
#include "common/Errors.h"
#include "tokenizer/Tokenizer.h"
#include <cassert>
using namespace Swallow;

/*
 statement → expression;opt
‌ statement → declaration;opt
‌ statement → loop-statement;opt
‌ statement → branch-statement;opt
‌ statement → labeled-statement
‌ statement → control-transfer-statement;opt
‌ statements → statementstatementsopt
*/
StatementPtr Parser::parseStatement()
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
                case Keyword::Prefix:
                case Keyword::Postfix:
                case Keyword::Infix:
                case Keyword::Lazy:
                case Keyword::Static:
                case Keyword::Mutating:
                case Keyword::Nonmutating:
                case Keyword::Override:
                    return parseDeclaration();
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
                default:
                    break;
            }
        }
        else if(token == L"@")
            return parseDeclaration();
    }
    ExpressionPtr ret = parseExpression();
    match(L";");
    return ret;
}
StatementPtr Parser::parseLoopStatement()
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
StatementPtr Parser::parseForLoop()
{
    //check if it's a for-in loop
    Token token, t;
    expect(Keyword::For, token);
    bool forIn = false;
    for(;;)
    {
        expect_next(t);
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
StatementPtr Parser::parseForIn()
{
    Token token;
    expect(Keyword::For, token);
    ForInLoopPtr ret = nodeFactory->createForInLoop(token.state);
    Flags flags(this);
    flags += SUPPRESS_TRAILING_CLOSURE;
    PatternPtr loopVars = parsePattern();
    expect(Keyword::In);
    ExpressionPtr container = parseExpression();
    flags -= SUPPRESS_TRAILING_CLOSURE;
    CodeBlockPtr codeBlock = parseCodeBlock();
    if(TypedPatternPtr var = std::dynamic_pointer_cast<TypedPattern>(loopVars))
    {
        ret->setLoopVars(var->getPattern());
        ret->setDeclaredType(var->getDeclaredType());
    }
    else
    {
        ret->setLoopVars(loopVars);
    }
    ret->setContainer(container);
    ret->setCodeBlock(codeBlock);
    return ret;
}
StatementPtr Parser::parseForStatement()
{
    std::vector<ExpressionPtr> inits;

    //‌ for-statement → forfor-initopt;expressionopt;expressionoptcode-block
    //‌ for-statement → for(for-initopt;expressionopt;expressionopt)code-block
    Token token;
    expect(Keyword::For, token);
    Flags flags(this);
    flags += SUPPRESS_TRAILING_CLOSURE;
    ForLoopPtr ret = nodeFactory->createForLoop(token.state);
    bool parenthesized = match(L"(");
    if(!predicate(L";", token))
    {
        if(token.getKeyword() == Keyword::Let || token.getKeyword() == Keyword::Var)
        {
            //read value bindings
            Attributes attrs;
            DeclarationPtr init = parseVar(attrs, 0);
            if(std::dynamic_pointer_cast<ComputedProperty>(init))
            {
                tassert(token, false, Errors::E_COMPUTED_PROPERTY_CANNOT_BE_DECLARED_UNDER_FOR_LOOP, L"");
                return nullptr;
            }
            else if(ValueBindingsPtr initializer = std::dynamic_pointer_cast<ValueBindings>(init))
            {
                ret->setInitializer(initializer);
            }
            else
            {
                assert(0 && "Invalid variable declaration on for loop");
            }
        }
        else
        {
            //read initializer list
            do
            {
                if (predicate(L";"))
                    break;
                ExpressionPtr init = parseExpression();
                ret->addInit(init);
            }
            while (match(L","));
        }
    }
    expect(L";");
    if(!predicate(L";"))
    {
        ExpressionPtr cond = parseExpression();
        ret->setCondition(cond);
    }
    expect(L";");
    ExpressionPtr step = NULL;
    if(!predicate(L"{"))
        step = parseExpression();
    ret->setStep(step);
    
    if(parenthesized)
        expect(L")");
    CodeBlockPtr codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    return ret;
    
    
    //TODO: for-in-statement → forpatterninexpressioncode-block
}
/*
  GRAMMAR OF A WHILE STATEMENT
 
 ‌ while-statement → whilewhile-conditioncode-block
 ‌ while-condition → expression | declaration
*/
StatementPtr Parser::parseWhileLoop()
{
    Token token;
    Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
    expect(Keyword::While, token);
    WhileLoopPtr ret = nodeFactory->createWhileLoop(token.state);
    ExpressionPtr condition = parseConditionExpression();
    //TODO: The condition can also be an optional binding declaration, as discussed in Optional Binding.
    ret->setCondition(condition);
    CodeBlockPtr codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    return ret;
}
//“while-condition → expression  declaration”
ExpressionPtr Parser::parseConditionExpression()
{
    Token token;
    expect_next(token);
    if(token.type == TokenType::Identifier && token.identifier.keyword != Keyword::_)
    {
        Keyword::T keyword = token.identifier.keyword;
        tassert(token, keyword == Keyword::Var || keyword == Keyword::Let, Errors::E_EXPECTED_EXPRESSION_VAR_OR_LET_IN_A_CONDITION_1, L"if");
        ValueBindingPatternPtr value = nodeFactory->createValueBindingPattern(token.state);
        value->setReadOnly(keyword == Keyword::Let);
        PatternPtr binding = parsePattern();
        value->setBinding(binding);
        if(binding->getNodeType() == NodeType::TypedPattern)
        {
            TypedPatternPtr tpattern = std::static_pointer_cast<TypedPattern>(binding);
            value->setBinding(tpattern->getPattern());
            value->setDeclaredType(tpattern->getDeclaredType());
        }
        if (!match(L"=", token))
        {
            tassert(token, false, Errors::E_VARIABLE_BINDING_IN_A_CONDITION_REQUIRES_AN_INITIALIZER);
        }
        AssignmentPtr assignment = nodeFactory->createAssignment(token.state);
        ExpressionPtr expr = parseExpression();
        assignment->setLHS(value);
        assignment->setRHS(expr);
        return assignment;
    }
    else
    {
        restore(token);
        return parseExpression();
    }
}
/*
 GRAMMAR OF A DO-WHILE STATEMENT

‌ do-while-statement → docode-blockwhilewhile-condition
*/
StatementPtr Parser::parseDoLoop()
{
    Token token;
    Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
    expect(Keyword::Do, token);
    DoLoopPtr ret = nodeFactory->createDoLoop(token.state);
    CodeBlockPtr codeBlock = parseCodeBlock();
    ret->setCodeBlock(codeBlock);
    expect(Keyword::While);
    ExpressionPtr condition = parseConditionExpression();
    ret->setCondition(condition);
    return ret;
}
/*
  GRAMMAR OF AN IF STATEMENT
 
 ‌ if-statement → ifif-conditioncode-blockelse-clauseopt
 ‌ if-condition → expression  declaration
 ‌ else-clause → elsecode-block  elseif-statement
 */
IfStatementPtr Parser::parseIf()
{
    Token token;
    expect(Keyword::If, token);
    IfStatementPtr ret = nodeFactory->createIf(token.state);
    {
        Flags flags(this);
        flags += SUPPRESS_TRAILING_CLOSURE;
        ExpressionPtr cond = parseConditionExpression();
        ret->setCondition(cond);
    }
    CodeBlockPtr thenPart = parseCodeBlock();
    ret->setThen(thenPart);
    if(match(L"else"))
    {
        Token token;
        expect_next(token);
        restore(token);
        if(token.type == TokenType::OpenBrace)
        {
            CodeBlockPtr elsePart = parseCodeBlock();
            ret->setElse(elsePart);
        }
        else if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::If)
        {
            IfStatementPtr elseIf = parseIf();
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
StatementPtr Parser::parseSwitch()
{
    Token token;
    Flags flags(this, UNDER_SWITCH_CASE | SUPPRESS_TRAILING_CLOSURE);
    
    expect(Keyword::Switch, token);
    SwitchCasePtr ret = nodeFactory->createSwitch(token.state);
    {
        Flags f(flags);
        f += SUPPRESS_TRAILING_CLOSURE;
        ExpressionPtr controlExpr = parseExpression();
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
            CaseStatementPtr caseCond = nodeFactory->createCase(token.state);
            CodeBlockPtr codeBlock = nodeFactory->createCodeBlock(token.state);
            caseCond->setCodeBlock(codeBlock);
            do
            {
                PatternPtr pattern = parsePattern();
                ExpressionPtr guard = NULL;
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
            CaseStatementPtr caseCond = nodeFactory->createCase(token.state);
            CodeBlockPtr codeBlock = nodeFactory->createCodeBlock(token.state);
            caseCond->setCodeBlock(codeBlock);
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
void Parser::parseSwitchStatements(const CaseStatementPtr& case_)
{
    Token token;
    Flags flags(this);
    flags -= SUPPRESS_TRAILING_CLOSURE;
    CodeBlockPtr codeBlock = case_->getCodeBlock();
    while(true)
    {
        expect_next(token);
        restore(token);
        Keyword::T k = token.getKeyword();
        if(token.type == TokenType::CloseBrace || (k == Keyword::Case || k == Keyword::Default))
        {
            break;
        }
        StatementPtr st = parseStatement();
        codeBlock->addStatement(st);
    }
        
}
/*
 GRAMMAR OF A BREAK STATEMENT

‌ break-statement → breaklabel-nameopt
*/
StatementPtr Parser::parseBreak()
{
    Token token;
    expect(Keyword::Break, token);
    BreakStatementPtr ret = nodeFactory->createBreak(token.state);
    
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
StatementPtr Parser::parseContinue()
{
    Token token;
    expect(Keyword::Continue, token);
    ContinueStatementPtr ret = nodeFactory->createContinue(token.state);
    
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
StatementPtr Parser::parseFallthrough()
{
    Token token;
    expect(Keyword::Fallthrough, token);
    FallthroughStatementPtr ret = nodeFactory->createFallthrough(token.state);
    return ret;
}
/*
 GRAMMAR OF A RETURN STATEMENT

‌ return-statement → returnexpressionopt
*/
StatementPtr Parser::parseReturn()
{
    Token token;
    expect(Keyword::Return, token);
    ReturnStatementPtr ret = nodeFactory->createReturn(token.state);
    
    if(peek(token))
    {
        //try to read an expression
        try {
            ExpressionPtr expr = parseExpression();
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
StatementPtr Parser::parseLabeledStatement()
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
                StatementPtr statement = parseSwitch();
                LabeledStatementPtr ret = nodeFactory->createLabel(token.state);
                ret->setLabel(label);
                ret->setStatement(statement);
                return ret;
            }
            case Keyword::For:
            case Keyword::Do:
            case Keyword::While:
            {
                StatementPtr statement = parseLoopStatement();
                LabeledStatementPtr ret = nodeFactory->createLabel(token.state);
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

CodeBlockPtr Parser::parseCodeBlock()
{
    Token token;
    Flags flags(this);
    flags -= SUPPRESS_TRAILING_CLOSURE;
    expect(L"{", token);
    CodeBlockPtr ret = nodeFactory->createCodeBlock(token.state);
    //ENTER_CONTEXT(TokenizerContextUnknown);
    while(!match(L"}"))
    {
        StatementPtr st = parseStatement();
        if(st != NULL)
            ret->addStatement(st);
    }
    return ret;
}
