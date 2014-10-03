#include "Parser.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
using namespace Swift;


TypeNodePtr Parser::parseTypeAnnotation()
{
    Attributes attrs;
    if(predicate(L"@"))
    {
        parseAttributes(attrs);
    }
    TypeNodePtr ret = parseType();
    ret->setAttributes(attrs);
    return ret;
}
TypeNodePtr Parser::parseType()
{
    Token token;
    TypeNodePtr ret = NULL;
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
            TupleTypePtr argType = std::dynamic_pointer_cast<TupleType>(ret);
            if(!argType)
            {
                //wrap ret as a tuple type
                argType = nodeFactory->createTupleType(*ret->getSourceInfo());
                argType->add(false, L"", ret);
            }
            TypeNodePtr retType = parseType();
            FunctionTypePtr func = nodeFactory->createFunctionType(token.state);
            func->setArgumentsType(argType);
            func->setReturnType(retType);
            ret = func;
            continue;
        }
        if(token == TokenType::OpenBracket)
        {
            expect(L"]");
            ArrayTypePtr array = nodeFactory->createArrayType(token.state);
            array->setInnerType(ret);
            ret = array;
            continue;
        }
        if(token == L"?")
        {
            //optional-type → type?
            OptionalTypePtr type = nodeFactory->createOptionalType(token.state);
            type->setInnerType(ret);
            ret = type;
            continue;
        }
        if(token == L"!")
        {
            //implicitly-unwrapped-optional-type → type!
            ImplicitlyUnwrappedOptionalPtr type = nodeFactory->createImplicitlyUnwrappedOptional(token.state);
            type->setInnerType(ret);
            ret = type;
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
 ‌ tuple-type-element → attributes opt inout opt type | inout opt element-name type-annotation
 ‌ element-name → identifier
 */
TupleTypePtr Parser::parseTupleType()
{
    Token token, token2;
    expect(L"(", token);
    TupleTypePtr ret = nodeFactory->createTupleType(token.state);
    Attributes attributes;
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
                TypeNodePtr type = parseType();
                type->setAttributes(attributes);
                ret->add(inout, token.token, type);
            }
            else
            {
                restore(token);
                TypeNodePtr type = parseType();
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
/*
  GRAMMAR OF A TYPE IDENTIFIER
 
 ‌ type-identifier → type-name generic-argument-clause opt | type-name generic-argument-clause opt . type-identifier
 ‌ type-name → identifier
*/
TypeIdentifierPtr Parser::parseTypeIdentifier()
{
    Token token;
    expect_identifier(token);
    TypeIdentifierPtr ret = nodeFactory->createTypeIdentifier(token.state);
    ret->setName(token.token);
    if(match(L"<"))
    {
        do
        {
            TypeNodePtr arg = parseType();
            ret->addGenericArgument(arg);
        }while(match(L","));
        
        expect(L">");
    }
    if(match(L"."))
    {
        //read next
        TypeIdentifierPtr next = parseTypeIdentifier();
        ret->setNestedType(next);
    }
    return ret;
}
ProtocolCompositionPtr Parser::parseProtocolComposition()
{
    Token token;
    expect(L"protocol", token);
    expect(L"<");
    ProtocolCompositionPtr ret = nodeFactory->createProtocolComposition(token.state);
    if(!predicate(L">"))
    {
        do
        {
            TypeIdentifierPtr protocol = parseTypeIdentifier();
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

GenericParametersDefPtr Parser::parseGenericParametersDef()
{
    Token token;
    expect(L"<", token);
    GenericParametersDefPtr ret = nodeFactory->createGenericParametersDef(token.state);
    // ‌ generic-parameter-list → generic-parameter | generic-parameter,generic-parameter-list
    do
    {
        // generic-parameter → type-name
        // generic-parameter → type-name:type-identifier
        //‌ generic-parameter → type-name:protocol-composition-type
        expect_identifier(token);
        std::wstring typeName = token.token;
        TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
        typeId->setName(typeName);
        ret->addGenericType(typeId);
        if(match(L":"))
        {
            TypeIdentifierPtr expectedType = parseTypeIdentifier();
            
            GenericConstraintDefPtr c = nodeFactory->createGenericConstraintDef(token.state);
            typeId = nodeFactory->createTypeIdentifier(token.state);
            typeId->setName(typeName);
            c->setIdentifier(typeId);
            c->setConstraintType(GenericConstraintDef::DerivedFrom);
            c->setExpectedType(expectedType);
            ret->addConstraint(c);
        }
    } while (match(L","));
    // ‌ requirement-clause → where requirement-list
    if(match(Keyword::Where))
    {
        // requirement-list → requirement | requirement,requirement-list
        do
        {
            TypeIdentifierPtr type = parseTypeIdentifier();
            // requirement → conformance-requirement | same-type-requirement
            if(match(L":"))
            {
                // conformance-requirement → type-identifier:type-identifier
                // conformance-requirement → type-identifier:protocol-composition-type
                //predicate(<#(const wchar_t*)token#>)

                if (predicate(L"protocol"))
                {
                    ProtocolCompositionPtr protocols = this->parseProtocolComposition();

                    for(const TypeNodePtr& protocol : *protocols)
                    {
                        GenericConstraintDefPtr c = nodeFactory->createGenericConstraintDef(*type->getSourceInfo());
                        c->setIdentifier(type);
                        TypeIdentifierPtr expectedType = parseTypeIdentifier();
                        c->setExpectedType(expectedType);
                        c->setConstraintType(GenericConstraintDef::DerivedFrom);
                        ret->addConstraint(c);
                    }
                }
                else
                {
                    GenericConstraintDefPtr c = nodeFactory->createGenericConstraintDef(*type->getSourceInfo());
                    c->setConstraintType(GenericConstraintDef::DerivedFrom);
                    c->setIdentifier(type);
                    TypeIdentifierPtr expectedType = parseTypeIdentifier();
                    c->setExpectedType(expectedType);
                    ret->addConstraint(c);
                }
            }
            else if(match(L"=="))
            {
                // same-type-requirement → type-identifier==type-identifier
                GenericConstraintDefPtr c = nodeFactory->createGenericConstraintDef(*type->getSourceInfo());
                ret->addConstraint(c);
                c->setIdentifier(type);
                TypeIdentifierPtr expectedType = parseTypeIdentifier();
                c->setConstraintType(GenericConstraintDef::EqualsTo);
                c->setExpectedType(expectedType);
            }
        } while (match(L","));

    }
    expect(L">");
    return ret;
}

