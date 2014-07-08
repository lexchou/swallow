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

Node* Parser::parseType(const wchar_t* code)
{
    tokenizer->set(code);
    return parseType();
}
TypeNode* Parser::parseTypeAnnotation()
{
    Attributes attrs;
    if(predicate(L"@"))
    {
        parseAttributes(attrs);
    }
    TypeNode* ret = parseType();
    ret->setAttributes(attrs);
    return ret;
}
TypeNode* Parser::parseType()
{
    Token token;
    TypeNode* ret = NULL;
    expect_next(token);
    restore(token);

    if(token == TokenType::OpenParen)
    {
        ret = parseTupleType();
    }
    else if(token.type == TokenType::Identifier && token.identifier.keyword == Keyword::_)
    {
        ret = parseTypeIdentifier();
    }
    else if(token.getKeyword() == Keyword::Protocol)
    {
        ret = parseProtocolComposition();
    }
    else
    {
        unexpected(token);
    }
    do
    {
        if(!next(token))
            break;
        
        //type chaining
        if(token == L"->")
        {
            //function-type → type->type
            TypeNode* retType = parseType();
            ret = nodeFactory->createFunctionType(token.state, ret, retType);
            continue;
        }
        if(token == TokenType::OpenBracket)
        {
            expect(L"]");
            ret = nodeFactory->createArrayType(token.state, ret);
            continue;
        }
        if(token == L"?")
        {
            //optional-type → type?
            ret = nodeFactory->createOptionalType(token.state, ret);
            continue;
        }
        if(token == L"!")
        {
            //implicitly-unwrapped-optional-type → type!
            ret = nodeFactory->createImplicitlyUnwrappedOptional(token.state, ret);
            continue;
        }
        // ‌ metatype-type → type.Type  type.Protocol
        //TODO meta type is not supported
        restore(token);
    }while(false);
    return ret;
}

/*
 GRAMMAR OF A TUPLE TYPE
 
 ‌ tuple-type → (tuple-type-bodyopt)
 ‌ tuple-type-body → tuple-type-element-list...opt
 ‌ tuple-type-element-list → tuple-type-element  tuple-type-element,tuple-type-element-list
 ‌ tuple-type-element → attributesoptinoutopttype inoutoptelement-nametype-annotation
 ‌ element-name → identifier
 */
TupleType* Parser::parseTupleType()
{
    Token token, token2;
    expect(L"(", token);
    TupleType* ret = nodeFactory->createTupleType(token.state);
    std::vector<Attribute*> attributes;
    if(!predicate(L")"))
    {
        do
        {
            //‌ tuple-type-element → attributes opt inout opt type inoutoptelement-nametype-annotation
            attributes.clear();
            bool inout = false;
            if(predicate(L"@"))
            {
                parseAttributes(attributes);
            }
            inout = match(Keyword::Inout);
            expect_next(token);
            if(attributes.empty() && token.type == TokenType::Identifier && token.identifier.keyword == Keyword::_ && peek(token2) && token2.type == TokenType::Colon)
            {
                //type-annotation → :attributes opt type
                if(predicate(L"@"))
                {
                    parseAttributes(attributes);
                }
                expect(L":");
                TypeNode* type = parseType();
                type->setAttributes(attributes);
                ret->add(inout, token.token, type);
            }
            else
            {
                restore(token);
                TypeNode* type = parseType();
                type->setAttributes(attributes);
                ret->add(inout, L"", type);
            }
        }while(match(L","));
        if(match(L"..."))
        {
            ret->setVariadicParameters(true);
        }
    }
    
    expect(L")");
    return ret;
}
TypeIdentifier* Parser::parseTypeIdentifier()
{
    Token token;
    match_identifier(token);
    TypeIdentifier* ret = nodeFactory->createTypeIdentifier(token.state, token.token);
    if(match(L"<"))
    {
        do
        {
            TypeNode* arg = parseType();
            ret->addGenericArgument(arg);
        }while(match(L","));
        
        expect(L">");
    }
    return ret;
}
ProtocolComposition* Parser::parseProtocolComposition()
{
    Token token;
    expect(L"protocol", token);
    expect(L"<");
    ProtocolComposition* ret = nodeFactory->createProtocolComposition(token.state);
    if(!predicate(L">"))
    {
        do
        {
            TypeIdentifier* protocol = parseTypeIdentifier();
            ret->addProtocol(protocol);
        }while(match(L","));
    }
    expect(L">");
    return ret;
}

/*
  GRAMMAR OF A GENERIC PARAMETER CLAUSE
 
 ‌ generic-parameter-clause → <generic-parameter-list requirement-clause opt>
 ‌ generic-parameter-list → generic-parameter | generic-parameter,generic-parameter-list
 ‌ generic-parameter → type-name
 ‌ generic-parameter → type-name:type-identifier
 ‌ generic-parameter → type-name:protocol-composition-type
 ‌ requirement-clause → where requirement-list
 ‌ requirement-list → requirement | requirement,requirement-list
 ‌ requirement → conformance-requirement | same-type-requirement
 ‌ conformance-requirement → type-identifier:type-identifier
 ‌ conformance-requirement → type-identifier:protocol-composition-type
 ‌ same-type-requirement → type-identifier==type-identifier
 GRAMMAR OF A GENERIC ARGUMENT CLAUSE
 
 ‌ generic-argument-clause → <generic-argument-list>
 ‌ generic-argument-list → generic-argument | generic-argument,generic-argument-list
 ‌ generic-argument → type
 */

GenericParameters* Parser::parseGenericParameters()
{
    Token token;
    expect(L"<", token);
    GenericParameters* ret = nodeFactory->createGenericParameters(token.state);
    // ‌ generic-parameter-list → generic-parameter | generic-parameter,generic-parameter-list
    do
    {
        // generic-parameter → type-name
        // generic-parameter → type-name:type-identifier
        //‌ generic-parameter → type-name:protocol-composition-type
        expect_identifier(token);
        std::wstring typeName = token.token;
        ret->addGenericType(nodeFactory->createTypeIdentifier(token.state, typeName));
        if(match(L":"))
        {
            do
            {
                TypeIdentifier* expectedType = parseTypeIdentifier();
                
                GenericConstraint* c = nodeFactory->createGenericConstraint(token.state);
                c->setIdentifier(nodeFactory->createTypeIdentifier(token.state, typeName));
                c->setConstraintType(GenericConstraint::DerivedFrom);
                c->addExpectedType(expectedType);
                ret->addConstraint(c);
                
            } while (match(L","));
            
        }
    } while (match(L","));
    // ‌ requirement-clause → where requirement-list
    if(match(Keyword::Where))
    {
        // requirement-list → requirement | requirement,requirement-list
        do
        {
            TypeIdentifier* type = parseTypeIdentifier();
            // requirement → conformance-requirement | same-type-requirement
            GenericConstraint* c = nodeFactory->createGenericConstraint(*type->getSourceInfo());
            ret->addConstraint(c);
            c->setIdentifier(type);
            if(match(L":"))
            {
                // conformance-requirement → type-identifier:type-identifier
                // conformance-requirement → type-identifier:protocol-composition-type
                c->setConstraintType(GenericConstraint::DerivedFrom);
                do
                {
                    TypeIdentifier* expectedType = parseTypeIdentifier();
                    c->addExpectedType(expectedType);
                } while (match(L","));
            }
            else if(match(L"=="))
            {
                // same-type-requirement → type-identifier==type-identifier
                TypeIdentifier* expectedType = parseTypeIdentifier();
                c->setConstraintType(GenericConstraint::EqualsTo);
                c->addExpectedType(expectedType);
            }
        } while (match(L","));

    }
    expect(L">");
    return ret;
}

