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
 
 ‌ pattern → wildcard-pattern type-annotationopt
 ‌ pattern → identifier-pattern type-annotationopt
 ‌ pattern → value-binding-pattern
 ‌ pattern → tuple-patterntype-annotationopt
 ‌ pattern → enum-case-pattern
 ‌ pattern → type-casting-pattern
 ‌ pattern → expression-pattern
*/
Pattern* Parser::parsePattern()
{
    Token token;
    next(token);
    if(token.type == TokenType::Identifier)
    {
        if(token.identifier.keyword == Keyword::_)
        switch(token.identifier.keyword)
        {
            //‌ pattern → wildcard-pattern type-annotationopt
            // pattern → identifier-pattern type-annotationopt
            //TODO : read type-annotation
            case Keyword::_:
                return nodeFactory->createIdentifier(token.token);
                break;
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
    
    tokenizer->restore(token);
    if(token.type == TokenType::OpenParen)
    {
        return parseTuple();
    }
    if(!(this->flags & UNDER_SWITCH_CASE))
    {
        unexpected(token);
        return NULL;
    }
    //the following patterns are only exists in switch/case statement
    // pattern → enum-case-pattern
    if(token.type == TokenType::Attribute || token == L".")
    {
        return parseEnumPattern();
    }
    // pattern → type-casting-pattern
    if(token.type == TokenType::Identifier)
    {
        return parseTypeCastingPattern();
    }
    //‌ pattern → expression-pattern
    return parseExpression();
}

/*
  “enum-case-pattern → type-identifier opt . enum-case-name tuple-pattern opt
*/
Pattern* Parser::parseEnumPattern()
{
    Token token;
    tassert(token, NULL);
    return NULL;
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
