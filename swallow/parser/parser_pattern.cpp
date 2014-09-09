#include "Parser.h"
#include "parser_details.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
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
PatternPtr Parser::parsePattern()
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
                TypedPatternPtr ret = nodeFactory->createTypedPattern(token.state);
                IdentifierPtr id = nodeFactory->createIdentifier(token.state);
                id->setIdentifier(token.token);
                ret->setPattern(id);
                if((flags & UNDER_CASE) == 0)//type annotation is not parsed when it's inside a let/var
                {
                    if(match(L":"))
                    {
                        TypeNodePtr type = parseTypeAnnotation();
                        ret->setDeclaredType(type);
                    }
                }
                return ret;
            }
            // pattern → value-binding-pattern
            case Keyword::Var:
            case Keyword::Let:
            {
                ValueBindingPatternPtr ret = nodeFactory->createValueBindingPattern(token.state);
                PatternPtr binding = parsePattern();
                if(TypedPatternPtr p = std::dynamic_pointer_cast<TypedPattern>(binding))
                {
                    ret->setBinding(p->getPattern());
                    ret->setDeclaredType(p->getDeclaredType());
                }
                else
                {
                    ret->setBinding(binding);
                }
                ret->setReadOnly(token.identifier.keyword == Keyword::Let);

                return ret;
            }
            default:
                break;
        }
    }
    
    restore(token);
    if(token.type == TokenType::OpenParen)
    {
        //pattern → tuple-pattern type-annotation opt
        TuplePtr ret = std::static_pointer_cast<Tuple>(parseTuple());
        if(flags & (UNDER_LET | UNDER_VAR))
        {
            //type-annotation only exists under let/var statement
            if(match(L":"))
            {
                TypeNodePtr type = parseTypeAnnotation();
                ret->setDeclaredType(type);
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
    PatternPtr ret = parseExpression();
    if(this->flags & UNDER_SWITCH_CASE)
    {
        if(predicate(L"as"))
        {
            restore(token);
            ret = NULL;
            return parseTypeCastingPattern();
        }
    }
    return ret;
}

/*
  “enum-case-pattern → type-identifier opt . enum-case-name tuple-pattern opt
*/
PatternPtr Parser::parseEnumPattern()
{
    Token token;
    expect(L".");
    expect_identifier(token);
    EnumCasePatternPtr ret = nodeFactory->createEnumCasePattern(token.state);
    ret->setName(token.token);


    if(predicate(L"("))
    {
        TuplePtr t = std::static_pointer_cast<Tuple>(parseTuple());
        ret->setAssociatedBinding(t);

    }
    return ret;
}
/*
  type-casting-pattern → is-pattern  as-pattern
 ‌ is-pattern → is type
 ‌ as-pattern → pattern as type
*/
PatternPtr Parser::parseTypeCastingPattern()
{
    Token token;
    if(match(Keyword::Is, token))
    {
        //is-pattern → is type
        TypeNodePtr type = parseType();
        TypeCheckPtr ret = nodeFactory->createTypeCheck(token.state);
        ret->setDeclaredType(type);
        return ret;
    }
    // as-pattern → pattern as type
    PatternPtr pat = parsePattern();
    expect(Keyword::As, token);
    TypeNodePtr type = parseType();
    TypeCastPtr ret = nodeFactory->createTypeCast(token.state);
    ret->setLHS(pat);
    ret->setDeclaredType(type);
    return ret;
}
/*
  tuple-pattern → (tuple-pattern-element-list opt)
 ‌ tuple-pattern-element-list → tuple-pattern-element | tuple-pattern-element,tuple-pattern-element-list
 ‌ tuple-pattern-element → pattern
 */
PatternPtr Parser::parseTuple()
{
    Token token;
    expect(L"(", token);
    TuplePtr ret = nodeFactory->createTuple(token.state);
    if(!predicate(L")"))
    {
        do
        {
            PatternPtr pattern = parsePattern();
            ret->add(pattern);
        }while(match(L","));
        
    }
    expect(L")");
    return ret;
}
