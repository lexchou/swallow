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
  GRAMMAR OF A PATTERN
 
 ‌ pattern → wildcard-pattern type-annotation opt
 ‌ pattern → identifier-pattern type-annotation opt
 ‌ pattern → value-binding-pattern
 ‌ pattern → tuple-pattern type-annotation opt
 ‌ pattern → enum-case-pattern
 ‌ pattern → type-casting-pattern
 ‌ pattern → expression-pattern
*/
Pattern* Parser::parsePattern()
{
    Token token;
    expect_next(token);
    if(token.type == TokenType::Identifier)
    {
        switch(token.identifier.keyword)
        {
            //‌ pattern → wildcard-pattern type-annotationopt
            // pattern → identifier-pattern type-annotationopt
            case Keyword::_:
            {
                Identifier* ret = nodeFactory->createIdentifier(token.token);
                if((flags & UNDER_VAR) == 0)//type annotation is not parsed when it's inside a let/var
                {
                    if(match(L":"))
                    {
                        TypeNode* type = parseTypeAnnotation();
                        ret->setType(type);
                    }
                }
                return ret;
            }
            // pattern → value-binding-pattern
            case Keyword::Var:
            {
                VarBinding* let = nodeFactory->createVarBinding();
                Pattern* binding = parsePattern();
                let->setBinding(binding);
                return let;
            }
            case Keyword::Let:
            {
                LetBinding* let = nodeFactory->createLetBinding();
                Pattern* binding = parsePattern();
                let->setBinding(binding);
                return let;
            }
            default:
                break;
        }
    }
    
    restore(token);
    if(token.type == TokenType::OpenParen)
    {
        //pattern → tuple-pattern type-annotation opt
        Tuple* ret = static_cast<Tuple*>(parseTuple());
        if(flags & (UNDER_LET | UNDER_VAR))
        {
            //type-annotation only exists under let/var statement
            if(match(L":"))
            {
                TypeNode* type = parseTypeAnnotation();
                ret->setType(type);
            }
        }
        return ret;
    }
    if(this->flags & UNDER_SWITCH_CASE)
    {
        //the following patterns are only exists in switch/case statement
        // pattern → enum-case-pattern
        if(token.type == TokenType::Attribute || token == L".")
        {
            return parseEnumPattern();
        }
        // pattern → type-casting-pattern
        if(token.getKeyword() == Keyword::Is)
        {
            return parseTypeCastingPattern();
        }
    }
    //‌ pattern → expression-pattern
    Pattern* ret = parseExpression();
    if(this->flags & UNDER_SWITCH_CASE)
    {
        if(predicate(L"as"))
        {
            restore(token);
            delete ret;
            return parseTypeCastingPattern();
        }
    }
    return ret;
}

/*
  “enum-case-pattern → type-identifier opt . enum-case-name tuple-pattern opt
*/
Pattern* Parser::parseEnumPattern()
{
    Token token;
    expect(L".");
    expect_identifier(token);
    EnumCasePattern* ret = nodeFactory->createEnumCasePattern(token.token);


    if(predicate(L"("))
    {
        Tuple* t = static_cast<Tuple*>(parseTuple());
        ret->setAssociatedBinding(t);

    }
    return ret;
}
/*
  type-casting-pattern → is-pattern  as-pattern
 ‌ is-pattern → is type
 ‌ as-pattern → pattern as type
*/
Pattern* Parser::parseTypeCastingPattern()
{
    Token token;
    if(match(Keyword::Is))
    {
        //is-pattern → is type
        TypeNode* type = parseType();
        return nodeFactory->createTypeCheck(NULL, type);
    }
    // as-pattern → pattern as type
    Pattern* pat = parsePattern();
    expect(Keyword::As);
    TypeNode* type = parseType();
    return nodeFactory->createTypeCheck(pat, type);
}
/*
  tuple-pattern → (tuple-pattern-element-list opt)
 ‌ tuple-pattern-element-list → tuple-pattern-element | tuple-pattern-element,tuple-pattern-element-list
 ‌ tuple-pattern-element → pattern
 */
Pattern* Parser::parseTuple()
{
    Tuple* ret = nodeFactory->createTuple();
    expect(L"(");
    if(!predicate(L")"))
    {
        do
        {
            Pattern* pattern = parsePattern();
            ret->add(pattern);
        }while(match(L","));
        
    }
    expect(L")");
    return ret;
}
