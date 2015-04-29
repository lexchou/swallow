/* Parser_Declaration.cpp --
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
#include "tokenizer/Tokenizer.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
#include "parser/Parser_Details.h"
#include "common/Errors.h"
#include <cassert>

using namespace Swallow;


/*
  GRAMMAR OF A DECLARATION
 
 ‌ declaration → import-declaration
 ‌ declaration → constant-declaration
 ‌ declaration → variable-declaration
 ‌ declaration → typealias-declaration
 ‌ declaration → function-declaration
 ‌ declaration → enum-declaration
 ‌ declaration → struct-declaration
 ‌ declaration → class-declaration
 ‌ declaration → protocol-declaration
 ‌ declaration → initializer-declaration
 ‌ declaration → deinitializer-declaration
 ‌ declaration → extension-declaration
 ‌ declaration → subscript-declaration
 ‌ declaration → operator-declaration
 ‌ declarations → declaration declarations opt
 ‌ declaration-specifiers → declaration-specifier declaration-specifiersopt
 ‌ declaration-specifier → class  mutating  nonmutating  override  static  unowned  unowned(safe) unowned(unsafe)  weak”
*/
DeclarationPtr Parser::parseDeclaration()
{
    Token token;
    std::vector<AttributePtr> attrs;
    if(predicate(L"@"))
        parseAttributes(attrs);
    int modifiers = parseDeclarationModifiers();

    expect_next(token);
    restore(token);
    if(token.type != TokenType::Identifier)
        unexpected(token);
    switch(token.identifier.keyword)
    {
        case Keyword::Import://declaration → import-declaration
            return parseImport(attrs);
        case Keyword::Let://declaration → constant-declaration
            return parseLet(attrs, modifiers);
        case Keyword::Var://declaration → variable-declaration
            return parseVar(attrs, modifiers);
        case Keyword::Typealias://declaration → typealias-declaration    ‌
            return parseTypealias(attrs, modifiers);
        case Keyword::Func://declaration → function-declaration
            return parseFunc(attrs, modifiers);
        case Keyword::Enum://declaration → enum-declaration
            return parseEnum(attrs, modifiers);
        case Keyword::Struct://declaration → struct-declaration
            return parseStruct(attrs, modifiers);
        case Keyword::Class://declaration → class-declaration
            return parseClass(attrs, modifiers);
        case Keyword::Protocol://declaration → protocol-declaration
            return parseProtocol(attrs, modifiers);
        case Keyword::Init://declaration → initializer-declaration
        case Keyword::Convenience:
            return parseInit(attrs, modifiers);
        case Keyword::Deinit://declaration → deinitializer-declaration
            return parseDeinit(attrs, modifiers);
        case Keyword::Extension://declaration → extension-declaration
            return parseExtension(attrs, modifiers);
        case Keyword::Subscript://declaration → subscript-declaration
            return parseSubscript(attrs, modifiers);
        case Keyword::Operator://declaration → operator-declaration
            return parseOperator(attrs, modifiers);
        default:
            //destroy attributes before reporting an issue
            attrs.clear();
            verifyDeclarationSpecifiers(token, modifiers, 0);
            unexpected(token);
            return NULL;
    }

    return NULL;
}
// declaration-specifiers → declaration-specifier declaration-specifiers opt
// declaration-specifier → class  mutating  nonmutating  override  static  unowned  unowned(safe) unowned(unsafe)  weak”

int Parser::parseDeclarationModifiers()
{

    Token token, t;
    int ret = 0;
    while(match_identifier(token))
    {
        switch(token.identifier.keyword)
        {
            case Keyword::Class:
                //check next token
                peek(t);
                if(t.getKeyword() == Keyword::_)
                {
                    //rollback
                    restore(token);
                    break;
                }
                ret |= DeclarationModifiers::Class;
                continue;
            case Keyword::Convenience:
                ret |= DeclarationModifiers::Convenience;
                continue;
            case Keyword::Dynamic:
                ret |= DeclarationModifiers::Dynamic;
                continue;
            case Keyword::Final:
                ret |= DeclarationModifiers::Final;
                continue;
            case Keyword::Infix:
                ret |= DeclarationModifiers::Infix;
                continue;
            case Keyword::Lazy:
                ret |= DeclarationModifiers::Lazy;
                continue;
            case Keyword::Mutating:
                ret |= DeclarationModifiers::Mutating;
                continue;
            case Keyword::Nonmutating:
                ret |= DeclarationModifiers::NonMutating;
                continue;
            case Keyword::Optional:
                ret |= DeclarationModifiers::Optional;
                continue;
            case Keyword::Override:
                ret |= DeclarationModifiers::Override;
                continue;
            case Keyword::Postfix:
                ret |= DeclarationModifiers::Postfix;
                continue;
            case Keyword::Prefix:
                ret |= DeclarationModifiers::Prefix;
                continue;
            case Keyword::Required:
                ret |= DeclarationModifiers::Required;
                continue;
            case Keyword::Static:
                ret |= DeclarationModifiers::Static;
                continue;
            case Keyword::Unowned:
                ret |= DeclarationModifiers::Unowned;
                if(match(L"("))
                {
                    expect_identifier(token);
                    if(token == L"safe")
                        ret |= DeclarationModifiers::Unowned_Safe;
                    else if(token == L"unsafe")
                        ret |= DeclarationModifiers::Unowned_Unsafe;
                    else
                        unexpected(token);
                    expect(L")");
                }
                break;
            case Keyword::Weak:
                ret |= DeclarationModifiers::Weak;
                continue;
            case Keyword::Internal:
                tassert(token, (ret & DeclarationModifiers::AccessModifiers) == 0, Errors::E_DUPLICATE_MODIFIER);
                ret = DeclarationModifiers::Internal;
                if(match(L"("))
                {
                    expect(L"set");
                    expect(L")");
                    ret = DeclarationModifiers::Internal_Set;
                }
                continue;
            case Keyword::Private:
                tassert(token, (ret & DeclarationModifiers::AccessModifiers) == 0, Errors::E_DUPLICATE_MODIFIER);
                ret = DeclarationModifiers::Private;
                if(match(L"("))
                {
                    expect(L"set");
                    expect(L")");
                    ret = DeclarationModifiers::Private_Set;
                }
                continue;
            case Keyword::Public:
                tassert(token, (ret & DeclarationModifiers::AccessModifiers) == 0, Errors::E_DUPLICATE_MODIFIER);
                ret = DeclarationModifiers::Public;
                if(match(L"("))
                {
                    expect(L"set");
                    expect(L")");
                    ret = DeclarationModifiers::Public_Set;
                }
                continue;
            default:
                restore(token);
                break;
        }
        break;
    }
    return ret;
}
void Parser::verifyDeclarationSpecifiers(const Token& token, int actualSpecifiers, int expectedSpecifiers)
{
    
}
/*
  GRAMMAR OF AN IMPORT DECLARATION
 
 ‌ import-declaration → attributesoptimportimport-kindoptimport-path
 ‌ import-kind → typealias  struct  class  enum protocol  var  func
 ‌ import-path → import-path-identifier | import-path-identifier.import-path
 ‌ import-path-identifier → identifier | operator”
*/
DeclarationPtr Parser::parseImport(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Import, token);
    ImportPtr ret = nodeFactory->createImport(token.state);
    ret->setAttributes(attrs);
    expect_next(token);
    tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_IDENTIFIER_1, token.token);
    Import::Kind kind = Import::_;
    switch(token.identifier.keyword)
    {
        case Keyword::_:
            restore(token);
            break;
        case Keyword::Typealias:
            kind = Import::Typealias;
            break;
        case Keyword::Struct:
            kind = Import::Struct;
            break;
        case Keyword::Class:
            kind = Import::Class;
            break;
        case Keyword::Enum:
            kind = Import::Enum;
            break;
        case Keyword::Protocol:
            kind = Import::Protocol;
            break;
        case Keyword::Var:
            kind = Import::Var;
            break;
        case Keyword::Func:
            kind = Import::Func;
            break;
        default:
            unexpected(token);
            break;
    }
    std::wstring path;
    int n = 0;
    do
    {
        expect_next(token);
        if(token.type != TokenType::Identifier && token.type != TokenType::Operator)
            unexpected(token);
        if(n)
            path += L".";
        path += token.token;
        n++;
    } while (match(L"."));
    ret->setPath(path);
    ret->setKind(kind);
    return ret;
}
/*
  GRAMMAR OF A CONSTANT DECLARATION
 
 ‌ constant-declaration → attributes opt declaration-specifiers opt let pattern-initializer-list
 ‌ pattern-initializer-list → pattern-initializer | pattern-initializer,pattern-initializer-list
 ‌ pattern-initializer → pattern initializer opt
 ‌ initializer → =expression
*/
DeclarationPtr Parser::parseLet(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    Flags flag(this, UNDER_LET);
    expect(Keyword::Let, token);
    ValueBindingsPtr ret = nodeFactory->createValueBindings(token.state);
    ret->setReadOnly(true);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    do
    {
        PatternPtr pattern = parsePattern();
        ExpressionPtr initializer = NULL;
        if(match(L"="))
        {
            initializer = parseExpression();
        }

        ValueBindingPtr constant = nodeFactory->createValueBinding(*pattern->getSourceInfo());
        constant->setAttributes(attrs);
        constant->setModifiers(modifiers);
        constant->setName(pattern);
        TypedPatternPtr typedPattern = std::dynamic_pointer_cast<TypedPattern>(pattern);
        if(typedPattern)
        {
            //promote typed pattern
            constant->setDeclaredType(typedPattern->getDeclaredType());
            constant->setName(typedPattern->getPattern());
        }
        constant->setInitializer(initializer);
        ret->add(constant);
    }while(match(L","));
    return ret;
}


ValueBindingPtr Parser::parseVariableDeclaration()
{
    Token token;
    Attributes attrs;
    
    PatternPtr key = parsePattern();
    TypeNodePtr type = NULL;
    ExpressionPtr val = NULL;
    if(match(L":"))
    {
        type = parseTypeAnnotation();
    }
    else if(TypedPatternPtr typedKey = std::dynamic_pointer_cast<TypedPattern>(key))
    {
        type = typedKey->getDeclaredType();
        key = typedKey->getPattern();
    }
    if(match(L"="))
    {
        Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
        val = parseExpression();
    }
    ValueBindingPtr ret = nodeFactory->createValueBinding(*key->getSourceInfo());
    if(TypedPatternPtr p = std::dynamic_pointer_cast<TypedPattern>(key))
    {
        ret->setName(p->getPattern());
        ret->setDeclaredType(p->getDeclaredType());
    }
    else
    {
        ret->setName(key);
        ret->setDeclaredType(type);
    }
    ret->setTypeAttributes(attrs);
    ret->setInitializer(val);
    return ret;
}
/*
  GRAMMAR OF A VARIABLE DECLARATION
 
 ‌ variable-declaration → variable-declaration-head pattern-initializer-list
 ‌ variable-declaration → variable-declaration-head variable-name type-annotation code-block
 ‌ variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-block
  variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-keyword-block
 ‌ variable-declaration → variable-declaration-head variable-name type-annotation initializer opt willSet-didSet-block
 ‌ variable-declaration-head → attributes opt declaration-specifiers opt var
 ‌ variable-name → identifier
*/
DeclarationPtr Parser::parseVar(const std::vector<AttributePtr>& attrs, int modifiers)
{

    Token token;
    expect(Keyword::Var, token);
    Flags flags(this, UNDER_VAR);
    //try read it as pattern-initializer-list
    ValueBindingsPtr ret = nodeFactory->createValueBindings(token.state);
    ret->setReadOnly(false);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    ValueBindingPtr var = parseVariableDeclaration();
    var->setModifiers(modifiers);
    ret->add(var);
    if(predicate(L","))
    {
        while(match(L","))
        {
            var = parseVariableDeclaration();
            var->setModifiers(modifiers);
            ret->add(var);
        }
        return ret;
    }

    if(!match(L"{"))
        return ret;

    ENTER_CONTEXT(TokenizerContextComputedProperty);

    peek(token);
    IdentifierPtr name = std::dynamic_pointer_cast<Identifier>(var->getName());
    tassert(token, name != nullptr, Errors::E_GETTER_SETTER_CAN_ONLY_BE_DEFINED_FOR_A_SINGLE_VARIABLE, L"");


    ComputedPropertyPtr prop = nodeFactory->createComputedProperty(*var->getSourceInfo());
    prop->setAttributes(attrs);
    prop->setModifiers(modifiers);
    prop->setTypeAttributes(var->getTypeAttributes());
    prop->setName(name->getIdentifier());
    prop->setDeclaredType(var->getDeclaredType());
    prop->setInitializer(var->getInitializer());
    var = nullptr;
    ret = nullptr;



    if(token.type != TokenType::CloseBrace)
    {
        Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
        switch(token.getKeyword())
        {
            case Keyword::Get:
            case Keyword::Set:
                if(this->flags & (UNDER_PROTOCOL | DECLARATION_ONLY))
                {
                    // variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-keyword-block
                    //no code block for getter/setter for protocol
                    std::pair<CodeBlockPtr, CodeBlockPtr> r = parseGetterSetterKeywordBlock();
                    prop->setGetter(r.first);
                    prop->setSetter(r.second);
                }
                else
                {
                    //  variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-block
                    std::pair<CodeBlockPtr, std::pair<std::wstring, CodeBlockPtr> > r = parseGetterSetterBlock();
                    prop->setGetter(r.first);
                    prop->setSetterName(r.second.first);
                    prop->setSetter(r.second.second);
                }
                break;
            case Keyword::WillSet:
            case Keyword::DidSet:
                //  variable-declaration → variable-declaration-head variable-name type-annotation initializer opt willSet-didSet-block
                parseWillSetDidSetBlock(prop);
                break;
            default:
                //‌ variable-declaration → variable-declaration-head variable-name type-annotation code-block
                CodeBlockPtr getter = nodeFactory->createCodeBlock(token.state);
                prop->setGetter(getter);
                do
                {
                    StatementPtr st = parseStatement();
                    getter->addStatement(st);
                }while(!predicate(L"}"));
                break;
        }
        
    }
    expect(L"}");
    return prop;
}

/*
willSet-clause → attributes opt willSet setter-name opt code-block
*/
void Parser::parseWillSetClause(const ComputedPropertyPtr& property, bool opt)
{
    Token token;
    if(!peek(token))
        return;
    if(opt && token.type == TokenType::CloseBrace)
        return;
    Attributes attrs;
    parseAttributes(attrs);

    ENTER_CONTEXT(TokenizerContextComputedProperty);

    expect(Keyword::WillSet);
    if(match(L"("))
    {
        expect_identifier(token);
        property->setWillSetSetter(token.token);
        expect(L")");
    }
    CodeBlockPtr cb = parseCodeBlock();
    property->setWillSet(cb);
}
/*
 ‌ didSet-clause → attributes opt didSet setter-name opt code-block
*/
void Parser::parseDidSetClause(const ComputedPropertyPtr& property, bool opt)
{
    Token token;
    if(!peek(token))
        return;
    if(opt && token.type == TokenType::CloseBrace)
        return;
    Attributes attrs;
    parseAttributes(attrs);
    expect(Keyword::DidSet);
    if(match(L"("))
    {
        expect_identifier(token);
        property->setDidSetSetter(token.token);
        expect(L")");
    }
    CodeBlockPtr cb = parseCodeBlock();
    property->setDidSet(cb);
}

/*
 ‌ willSet-didSet-block → {willSet-clause didSet-clause opt}
 ‌ willSet-didSet-block → {didSet-clause willSet-clause}
 */
void Parser::parseWillSetDidSetBlock(const ComputedPropertyPtr& property)
{
    Token token;
    Attributes attrs;

    ENTER_CONTEXT(TokenizerContextComputedProperty);

    parseAttributes(attrs);
    if(predicate(L"willSet"))
    {
        //willSet-didSet-block → {willSet-clause didSet-clause opt}
        parseWillSetClause(property, false);
        parseDidSetClause(property, true);
        return;
    }
    if(predicate(L"didSet"))
    {
        parseDidSetClause(property, false);
        parseWillSetClause(property, true);
    }
}

/*
 ‌ getter-setter-block → {getter-clause setter-clauseopt}
 ‌ getter-setter-block → {setter-clause getter-clause}
 ‌ getter-clause → attributes opt get code-block
 ‌ setter-clause → attributes opt set setter-name opt code-block
 ‌ setter-name → (identifier)
*/
std::pair<CodeBlockPtr, std::pair<std::wstring, CodeBlockPtr> > Parser::parseGetterSetterBlock()
{
    Token token;
    Attributes attrs;
    parseAttributes(attrs);

    ENTER_CONTEXT(TokenizerContextComputedProperty);
    int modifiers = parseAccessorModifiers();

    peek(token);
    CodeBlockPtr getter = NULL;
    std::pair<std::wstring, CodeBlockPtr> setter = std::make_pair(std::wstring(), CodeBlockPtr());
    if(token.getKeyword() == Keyword::Get)
    {
        expect_next(token);
        getter = parseCodeBlock();
        getter->setAttributes(attrs);
        modifiers = parseAccessorModifiers();
        peek(token);
        if(token == L"@" || token.getKeyword() == Keyword::Set)
        {
            setter = parseSetterClause();
        }
    }
    else if(token.getKeyword() == Keyword::Set)
    {
        setter = parseSetterClause();
        setter.second->setAttributes(attrs);
        parseAttributes(attrs);
        modifiers = parseAccessorModifiers();
        expect(Keyword::Get);
        getter = parseCodeBlock();
        getter->setAttributes(attrs);
    }
    return std::make_pair(getter, setter);
}
/*
 ‌ setter-clause → attributes opt set setter-name opt code-block
*/
std::pair<std::wstring, CodeBlockPtr> Parser::parseSetterClause()
{
    Token token;
    Attributes attrs;
    expect(Keyword::Set);
    std::wstring name;
    if(match(L"("))
    {
        expect_identifier(token);
        expect(L")");
        name = token.token;
    }
    CodeBlockPtr setter = parseCodeBlock();
    return std::make_pair(name, setter);
}
/*
 ‌ getter-setter-keyword-block → {getter-keyword-clause setter-keyword-clause opt}
 ‌ getter-setter-keyword-block → {setter-keyword-clause getter-keyword-clause}
 ‌ getter-keyword-clause → attributes opt get
 ‌ setter-keyword-clause → attributes opt set
*/
std::pair<CodeBlockPtr, CodeBlockPtr> Parser::parseGetterSetterKeywordBlock()
{
    Token token;
    Attributes attributes;
    CodeBlockPtr getter = NULL;
    CodeBlockPtr setter = NULL;

    parseAttributes(attributes);

    ENTER_CONTEXT(TokenizerContextComputedProperty);
    int modifiers = parseAccessorModifiers();

    if(match(Keyword::Get, token))
    {
        getter = nodeFactory->createCodeBlock(token.state);
        getter->setAttributes(attributes);
        parseAttributes(attributes);
        modifiers = parseAccessorModifiers();
        if(match(Keyword::Set, token))
        {
            setter = nodeFactory->createCodeBlock(token.state);
            setter->setAttributes(attributes);
        }
        else if(!attributes.empty())//attributes defined for setter but setter is not found
        {
            Token token;
            expect_next(token);
            unexpected(token);
        }
    }
    else if(match(Keyword::Set, token))
    {
        setter = nodeFactory->createCodeBlock(token.state);
        setter->setAttributes(attributes);
        
        parseAttributes(attributes);
        modifiers = parseAccessorModifiers();
        expect(Keyword::Get, token);
        getter = nodeFactory->createCodeBlock(token.state);
        getter->setAttributes(attributes);
    }
    return std::make_pair(getter, setter);
}
/*!
 * Parse declaration modifiers that only used by property/subscript accessors
 */
int Parser::parseAccessorModifiers()
{
    int ret = 0;
    Token token;
    while(next(token))
    {
        switch(token.getKeyword())
        {
            case Keyword::Nonmutating:
                ret |= DeclarationModifiers::NonMutating;
                break;
            case Keyword::Mutating:
                ret |= DeclarationModifiers::Mutating;
                break;
            default:
                restore(token);
                return ret;
                break;
        }
    }
    return ret;
}

/*
 “GRAMMAR OF A TYPE ALIAS DECLARATION
 
 ‌ typealias-declaration → typealias-head typealias-assignment
 ‌ typealias-head → typealias typealias-name
 ‌ typealias-name → identifier
 ‌ typealias-assignment → =type
*/
DeclarationPtr Parser::parseTypealias(const Attributes& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Typealias, token);
    expect_identifier(token);
    TypeNodePtr type = nullptr;
    TypeAliasPtr ret = nodeFactory->createTypealias(token.state);
    ret->setAttributes(attrs);
    ret->setName(token.token);
    ret->setModifiers(modifiers);

    if(match(L":", token))
    {
        //read constraint
        ProtocolCompositionPtr constraint = nodeFactory->createProtocolComposition(token.state);
        do
        {
            TypeIdentifierPtr p = this->parseTypeIdentifier();
            constraint->addProtocol(p);
        } while (match(L","));
        ret->setConstraint(constraint);
    }

    if(this->flags & (UNDER_PROTOCOL | DECLARATION_ONLY))
    {
        if (match(L"="))
        {
            type = parseType();
        }
    }
    else
    {
        expect(L"=");
        type = parseType();
    }
    ret->setType(type);
    return ret;
}
/*
  GRAMMAR OF A FUNCTION DECLARATION
 
 ‌ function-declaration → function-head function-name generic-parameter-clause opt function-signature function-body
 ‌ function-head → attributes opt declaration-specifiers opt func
 ‌ function-name → identifier | operator
 ‌ function-signature → parameter-clauses function-result opt
 ‌ function-result → ->attributes opt type
 ‌ function-body → code-block
*/
DeclarationPtr Parser::parseFunc(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Func, token);
    FunctionDefPtr ret = nodeFactory->createFunction(token.state);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    {
        ENTER_CONTEXT(TokenizerContextFunctionSignature);
        expect_next(token);
    }
    if(token.type != TokenType::Identifier && token.type != TokenType::Operator)
        unexpected(token);
    ret->setName(token.token);
    if(token.type == TokenType::Operator)
        ret->setKind(FunctionKindOperator);
    else
        ret->setKind(FunctionKindNormal);
    if(predicate(L"<"))
    {
        GenericParametersDefPtr params = this->parseGenericParametersDef();
        ret->setGenericParametersDef(params);
    }
    //parse parameters
    {
        ENTER_CONTEXT(TokenizerContextFunctionSignature);
        do
        {
            ParametersNodePtr parameters = parseParameterClause();
            ret->addParameters(parameters);
        } while (predicate(L"("));
    }
    if(match(L"->"))
    {
        //function-result → ->attributes opt type
        Attributes retAttrs;
        parseAttributes(retAttrs);
        TypeNodePtr retType = parseType();
        ret->setReturnTypeAttributes(retAttrs);
        ret->setReturnType(retType);
    }
    bool hasBody = true;
    if(flags & (UNDER_PROTOCOL | DECLARATION_ONLY))
        hasBody = false;
    if(hasBody)
    {
        ENTER_CONTEXT(TokenizerContextFunctionBody);
        CodeBlockPtr body = parseCodeBlock();
        ret->setBody(body);
    }
    else
    {
        ret->setBody(nodeFactory->createCodeBlock(*ret->getSourceInfo()));
    }
    return ret;
}

/*
 ‌ parameter-clauses → parameter-clause parameter-clauses opt
 ‌ parameter-clause → () | (parameter-list...opt)
 ‌ parameter-list → parameter | parameter,parameter-list
 ‌ parameter → inout opt let opt#opt parameter-name local-parameter-name opt type-annotation default-argument-clause opt
 ‌ parameter → inout opt var #opt parameter-name local-parameter-name opt type-annotation default-argument-clause opt
 ‌ parameter → attributes opt type
 ‌ parameter-name → identifier | _
 ‌ local-parameter-name → identifier | _
 ‌ default-argument-clause → =expression
*/
ParametersNodePtr Parser::parseParameterClause()
{
    Token token;
    expect(L"(", token);
    ParametersNodePtr ret = nodeFactory->createParameters(token.state);
    if(match(L")"))
        return ret;
    ENTER_CONTEXT(TokenizerContextFunctionSignature);
    do
    {
        bool inout = match(Keyword::Inout);
        ParameterNode::Accessibility accessibility = ParameterNode::None;
        expect_next(token);
        ParameterNodePtr param = nodeFactory->createParameter(token.state);
        if(token.type == TokenType::Identifier)
        {
            if(token.identifier.keyword == Keyword::Var)
            {
                expect_next(token);
                accessibility = ParameterNode::Variable;
            }
            else if(token.identifier.keyword == Keyword::Let)
            {
                expect_next(token);
                accessibility = ParameterNode::Constant;
            }
        }
        bool shorthandExternalName = false;
        if(token == L"#")
        {
            shorthandExternalName = true;
            expect_next(token);
        }
        tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_IDENTIFIER_1, token.token);
        std::wstring name = token.token;
        std::wstring localName;
        if(match_identifier(token))
        {
            localName = token.token;
        }
        else
        {
            localName = name;
            name.clear();
        }
        expect(L":");
        Attributes attrs;
        parseAttributes(attrs);
        TypeNodePtr type = parseType();
        ExpressionPtr def = NULL;
        if(match(L"="))
        {
            if(match(L"default", token))
            {
                def = nodeFactory->createDefaultValue(token.state);
            }
            else
            {
                peek(token);
                def = parseExpression();
                tassert(token, def != NULL, Errors::E_EXPECT_EXPRESSION_1, token.token);
            }

        }
        param->setInout(inout);
        param->setAccessibility(accessibility);
        param->setExternalName(name);
        param->setLocalName(localName);
        param->setShorthandExternalName(shorthandExternalName);
        param->setTypeAttributes(attrs);
        param->setDeclaredType(type);
        param->setDefaultValue(def);
        ret->addParameter(param);
    }while(match(L","));
    bool variadicParameters = match(L"...");
    ret->setVariadicParameters(variadicParameters);
    expect(L")");
    return ret;
}
/*
  GRAMMAR OF AN ENUMERATION DECLARATION
 
 ‌ enum-declaration → attributes opt union-style-enum | attributes opt raw-value-style-enum
 ‌ enum-name → identifier
 ‌ enum-case-name → identifier
 
*/
DeclarationPtr Parser::parseEnum(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Enum);
    Flags flag(this);
    flags += UNDER_ENUM;
    expect_identifier(token);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(token.token);
    GenericParametersDefPtr generic = nullptr;
    if(predicate(L"<"))
    {
        generic = this->parseGenericParametersDef();
    }
    EnumDefPtr ret = nodeFactory->createEnum(token.state);
    ret->setValueStyle(EnumDef::Undefined);
    ret->setIdentifier(typeId);
    ret->setGenericParametersDef(generic);
    ret->setModifiers(modifiers);

    if(match(L":"))
    {
        do
        {
            TypeIdentifierPtr baseType = this->parseTypeIdentifier();
            ret->addParent(baseType);
        } while(match(L","));
    }
    expect(L"{");

    ENTER_CONTEXT(TokenizerContextClass);

    while(!predicate(L"}"))
    {
        if(!match(Keyword::Case))
        {
            DeclarationPtr decl = parseDeclaration();
            ret->addDeclaration(decl);
            continue;
        }
        //parse cases
        do
        {
            expect_identifier(token);
            ExpressionPtr val = NULL;
            if(match(L"="))
            {
                ret->setValueStyle(EnumDef::RawValues);
                //the raw value must be a literal, but to offer accurate compilation error,
                //use expression here, and check if it's literal in semantic analyzing stage.
                val = parseExpression();
                ret->addCase(token.token, val);
            }
            else if(predicate(L"("))
            {
                TypeNodePtr associatedType = parseTupleType();
                ret->addCase(token.token, associatedType);
            }
            else
            {
                ret->addCase(token.token, nullptr);
            }
        }while(match(L","));
    }
    expect(L"}");
    ret->sortByPriority();
    return ret;
}


/*
 “GRAMMAR OF A STRUCTURE DECLARATION
 
 ‌ struct-declaration → attributes opt struct struct-name generic-parameter-clause opt type-inheritance-clause opt struct-body
 ‌ struct-name → identifier
 ‌ struct-body → {declarations opt}”
*/
DeclarationPtr Parser::parseStruct(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Struct, token);
    StructDefPtr ret = nodeFactory->createStruct(token.state);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    expect_identifier(token);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(token.token);
    ret->setIdentifier(typeId);

    if(predicate(L"<"))
    {
        GenericParametersDefPtr params = parseGenericParametersDef();
        ret->setGenericParametersDef(params);
    }
    
    
    if(match(L":"))
    {
        do
        {
            TypeIdentifierPtr protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    
    Flags f(this);
    flags += UNDER_STRUCT;

    ENTER_CONTEXT(TokenizerContextClass);

    expect(L"{");
    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
        ret->addDeclaration(decl);
        match(L";");
    }
    expect(L"}");
    ret->sortByPriority();
    return ret;
}
/*
 “GRAMMAR OF A CLASS DECLARATION
 
 ‌ class-declaration → attributes opt class class-name generic-parameter-clause opt type-inheritance-clause opt class-body
 ‌ class-name → identifier
 ‌ class-body → {declarations opt}”
 
*/
DeclarationPtr Parser::parseClass(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Class, token);
    ClassDefPtr ret = nodeFactory->createClass(token.state);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    expect_identifier(token);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(token.token);
    ret->setIdentifier(typeId);
    if(predicate(L"<"))
    {
        GenericParametersDefPtr params = parseGenericParametersDef();
        ret->setGenericParametersDef(params);
    }
    if(match(L":"))
    {
        do
        {
            TypeIdentifierPtr protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    
    Flags f(this);
    f += UNDER_CLASS;


    ENTER_CONTEXT(TokenizerContextClass);
    expect(L"{");
    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
        ret->addDeclaration(decl);
    }
    expect(L"}");
    //sort member declarations by priority
    ret->sortByPriority();
    return ret;
}
/*
 “GRAMMAR OF A PROTOCOL DECLARATION
 
 ‌ protocol-declaration → attributes opt protocol protocol-name type-inheritance-clause opt protocol-body
 ‌ protocol-name → identifier
 ‌ protocol-body → {protocol-member-declarations opt}
 ‌ protocol-member-declaration → protocol-property-declaration
 ‌ protocol-member-declaration → protocol-method-declaration
 ‌ protocol-member-declaration → protocol-initializer-declaration
 ‌ protocol-member-declaration → protocol-subscript-declaration
 ‌ protocol-member-declaration → protocol-associated-type-declaration
 ‌ protocol-member-declarations → protocol-member-declaration protocol-member-declarations opt
 ”
 
 “GRAMMAR OF A PROTOCOL PROPERTY DECLARATION
 
 ‌ protocol-property-declaration → variable-declaration-head variable-name type-annotation getter-setter-keyword-block
 ”
 “GRAMMAR OF A PROTOCOL METHOD DECLARATION
 
 ‌ protocol-method-declaration → function-head function-name generic-parameter-clause opt function-signature
 ”
 “GRAMMAR OF A PROTOCOL INITIALIZER DECLARATION
 
 ‌ protocol-initializer-declaration → initializer-head generic-parameter-clause opt parameter-clause
 ”
 “GRAMMAR OF A PROTOCOL SUBSCRIPT DECLARATION
 
 ‌ protocol-subscript-declaration → subscript-head subscript-result getter-setter-keyword-block”
 
 “GRAMMAR OF A PROTOCOL ASSOCIATED TYPE DECLARATION
 
 ‌ protocol-associated-type-declaration → typealias-head type-inheritance-clause opt typealias-assignment opt
 ‌”
 

*/
DeclarationPtr Parser::parseProtocol(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Protocol);
    ProtocolDefPtr ret = nodeFactory->createProtocol(token.state);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    expect_identifier(token);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(token.token);
    ret->setIdentifier(typeId);
    if(match(L":"))
    {
        do
        {
            TypeIdentifierPtr protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    expect(L"{");
    
    Flags f(this);
    f += UNDER_PROTOCOL;


    ENTER_CONTEXT(TokenizerContextClass);
    
    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
        ret->addDeclaration(decl);
    }
    expect(L"}");
    return ret;
}
/*
  GRAMMAR OF AN INITIALIZER DECLARATION
 
 ‌ initializer-declaration → initializer-head generic-parameter-clause opt parameter-clause initializer-body
 ‌ initializer-head → attributes opt convenience opt init
 ‌ initializer-body → code-block
 
*/
DeclarationPtr Parser::parseInit(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Init, token);
    bool failable = false, implicitFailable = false;
    next(token);
    if(token == L"?" || token == L"!")
    {
        failable = true;
        implicitFailable = token == L"!";
    }
    else
        restore(token);
    InitializerDefPtr ret = nodeFactory->createInitializer(token.state);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    ret->setFailable(failable);
    ret->setImplicitFailable(implicitFailable);
    if(predicate(L"<"))
    {
        GenericParametersDefPtr params = this->parseGenericParametersDef();
        ret->setGenericParametersDef(params);
    }
    ParametersNodePtr parameters = parseParameterClause();
    ret->setParameters(parameters);

    if((UNDER_PROTOCOL | DECLARATION_ONLY) & flags)
    {
        ret->setBody(nodeFactory->createCodeBlock(*ret->getSourceInfo()));
    }
    else
    {
        ENTER_CONTEXT(TokenizerContextFunctionBody);
        CodeBlockPtr body = parseCodeBlock();
        ret->setBody(body);
    }
    return ret;
}
/*
  GRAMMAR OF A DEINITIALIZER DECLARATION
 
 ‌ deinitializer-declaration → attributes opt deinit code-block
*/
DeclarationPtr Parser::parseDeinit(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Deinit, token);
    DeinitializerDefPtr ret = nodeFactory->createDeinitializer(token.state);
    ret->setAttributes(attrs);
    ret->setModifiers(modifiers);
    if(DECLARATION_ONLY & flags)
    {
        ret->setBody(nodeFactory->createCodeBlock(*ret->getSourceInfo()));
    }
    else
    {
        CodeBlockPtr body = parseCodeBlock();
        ret->setBody(body);
    }
    return ret;
}
/*
 “GRAMMAR OF AN EXTENSION DECLARATION
 
 ‌ extension-declaration → extension type-identifier type-inheritance-clause opt extension-body
 ‌ extension-body → {declarationsopt}”
*/
DeclarationPtr Parser::parseExtension(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    Flags flags(this, UNDER_EXTENSION);
    expect(Keyword::Extension, token);
    TypeIdentifierPtr id = parseTypeIdentifier();
    ExtensionDefPtr ret = nodeFactory->createExtension(token.state);
    ret->setAttributes(attrs);
    ret->setIdentifier(id);
    ret->setModifiers(modifiers);
    if(match(L":"))
    {
        // type-inheritance-list → type-identifier  type-identifier,type-inheritance-list
        do
        {
            TypeIdentifierPtr protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    expect(L"{");

    ENTER_CONTEXT(TokenizerContextClass);

    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
        ret->addDeclaration(decl);
    }
    expect(L"}");
    return ret;
}
/*
 “GRAMMAR OF A SUBSCRIPT DECLARATION
 
 ‌ subscript-declaration → subscript-head subscript-result code-block
 ‌ subscript-declaration → subscript-head subscript-result getter-setter-block
 ‌ subscript-declaration → subscript-head subscript-result getter-setter-keyword-block
 ‌ subscript-head → attributes opt subscript parameter-clause
 ‌ subscript-result → ->attributes opt type
 ”
*/
DeclarationPtr Parser::parseSubscript(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    expect(Keyword::Subscript, token);
    ParametersNodePtr params = parseParameterClause();
    expect(L"->");
    Attributes typeAttrs;
    parseAttributes(typeAttrs);
    TypeNodePtr retType = parseType();
    SubscriptDefPtr ret = nodeFactory->createSubscript(token.state);
    ret->setAttributes(attrs);
    ret->setParameters(params);
    ret->setReturnType(retType);
    ret->setReturnTypeAttributes(typeAttrs);
    ret->setModifiers(modifiers);

    ENTER_CONTEXT(TokenizerContextComputedProperty);
    if(flags & (UNDER_PROTOCOL | DECLARATION_ONLY))
    {
        if(match(L"{"))
        {
            std::pair<CodeBlockPtr, CodeBlockPtr> accessors = parseGetterSetterKeywordBlock();
            expect(L"}");
            ret->setGetter(accessors.first);
            ret->setSetter(accessors.second);
        }
    }
    else
    {
        Token token;
        TokenizerState s = tokenizer->save();
        expect(L"{");


        expect_next(token);
        restore(token);
        if(token.getKeyword() == Keyword::Set || token.getKeyword() == Keyword::Get)
        {
            std::pair<CodeBlockPtr, std::pair<std::wstring, CodeBlockPtr> > accessors = parseGetterSetterBlock();
            ret->setGetter(accessors.first);
            ret->setSetter(accessors.second.second);
            ret->setSetterName(accessors.second.first);
            expect(L"}");
        }
        else
        {
            tokenizer->restore(s);
            CodeBlockPtr cb = parseCodeBlock();
            ret->setGetter(cb);
        }
    }
    return ret;
}
/*
  GRAMMAR OF AN OPERATOR DECLARATION

  operator-declaration → prefix-operator-declaration | postfix-operator-declaration | infix-operator-declaration­
  prefix-operator-declaration → prefix operator operator{}
  postfix-operator-declaration → postfix operator operator{}
  infix-operator-declaration → infix operator operator { infix-operator-attributes opt}
  infix-operator-attributes → precedence-clause opt associativity-clause opt
  precedence-clause → precedence precedence-level
  precedence-level → A decimal integer between 0 and 255, inclusive
  associativity-clause → associativity associativity­
  associativity → left  right  none

*/
DeclarationPtr Parser::parseOperator(const std::vector<AttributePtr>& attrs, int modifiers)
{
    Token token;
    //validate modifiers
    if((modifiers & (DeclarationModifiers::Prefix | DeclarationModifiers::Postfix | DeclarationModifiers::Infix)) == 0)
    {
        tassert(token, false, Errors::E_OPERATOR_MUST_BE_DECLARED_AS_PREFIX_POSTFIX_OR_INFIX);
        return nullptr;
    }
    expect(Keyword::Operator);

    ENTER_CONTEXT(TokenizerContextOperator);

    OperatorType::T type = OperatorType::_;
    OperatorDefPtr op = nodeFactory->createOperator(token.state);
    op->setAttributes(attrs);

    if(modifiers & DeclarationModifiers::Prefix)
        type = OperatorType::PrefixUnary;
    else if(modifiers & DeclarationModifiers::Postfix)
        type = OperatorType::PostfixUnary;
    else if(modifiers & DeclarationModifiers::Infix)
        type = OperatorType::InfixBinary;
    else
        assert(0 && "Should never happen here");
    expect_next(token);
    tassert(token, token.type == TokenType::Operator, Errors::E_EXPECT_OPERATOR_1, token.token);
    op->setName(token.token);
    op->setType(type);
    expect(L"{");
    while(!match(L"}"))
    {
        expect_next(token);
        switch(token.getKeyword())
        {
            case Keyword::Precedence:
                expect_next(token);
                tassert(token, token.type == TokenType::Integer, Errors::E_EXPECT_INTEGER_PRECEDENCE, token.token);
                op->setPrecedence(token.number.value);
                break;
            case Keyword::Associativity:
                expect_next(token);
                switch(token.getKeyword())
                {
                    case Keyword::None:
                        op->setAssociativity(Associativity::None);
                        break;
                    case Keyword::Right:
                        op->setAssociativity(Associativity::Right);
                        break;
                    case Keyword::Left:
                        op->setAssociativity(Associativity::Left);
                        break;
                    default:
                        unexpected(token);
                        break;
                }
                break;
            case Keyword::Assignment:
                op->setAssignment(true);
                break;
            default:
                unexpected(token);
        }
    }
    return op;
}
