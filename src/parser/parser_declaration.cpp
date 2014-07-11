#include "parser.h"
#include "ast/node.h"
#include "tokenizer/tokenizer.h"
#include "symbol-registry.h"
#include "ast/node-factory.h"
#include "ast/ast.h"
#include "parser_details.h"
#include <cstdlib>
#include <stack>
#include <sstream>
#include <iostream>
using namespace Swift;


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
Declaration* Parser::parseDeclaration()
{
    Token token;
    std::vector<Attribute*> attrs;
    if(predicate(L"@"))
        parseAttributes(attrs);
    int specifiers = 0;
    if(flags & (UNDER_CLASS | UNDER_STRUCT | UNDER_ENUM | UNDER_PROTOCOL))
    {
        //read declaration specifiers
        specifiers = parseDeclarationSpecifiers();
    }
    expect_next(token);
    restore(token);
    if(token.type != TokenType::Identifier)
        unexpected(token);
    switch(token.identifier.keyword)
    {
        case Keyword::Import://declaration → import-declaration
            return parseImport(attrs);
        case Keyword::Let://declaration → constant-declaration
            return parseLet(attrs, specifiers);
        case Keyword::Var://declaration → variable-declaration
            return parseVar(attrs, specifiers);
        case Keyword::Typealias://declaration → typealias-declaration    ‌
            return parseTypealias(attrs);
        case Keyword::Func://declaration → function-declaration
            return parseFunc(attrs, specifiers);
        case Keyword::Enum://declaration → enum-declaration
            return parseEnum(attrs);
        case Keyword::Struct://declaration → struct-declaration
            return parseStruct(attrs);
        case Keyword::Class://declaration → class-declaration
            return parseClass(attrs);
        case Keyword::Protocol://declaration → protocol-declaration
            return parseProtocol(attrs);
        case Keyword::Init://declaration → initializer-declaration
        case Keyword::Convenience:
            return parseInit(attrs);
        case Keyword::Deinit://declaration → deinitializer-declaration
            return parseDeinit(attrs);
        case Keyword::Extension://declaration → extension-declaration
            return parseExtension(attrs);
        case Keyword::Subscript://declaration → subscript-declaration
            return parseSubscript(attrs);
        case Keyword::Operator://declaration → operator-declaration
            return parseOperator(attrs);
        default:
            //destroy attributes before reporting an issue
            std::vector<Attribute*>::iterator iter = attrs.begin();
            for(; iter != attrs.end(); iter++)
            {
                Attribute* attr = *iter;
                delete attr;
            }
            verifyDeclarationSpecifiers(token, specifiers, 0);
            unexpected(token);
            return NULL;
    }

    return NULL;
}
// declaration-specifiers → declaration-specifier declaration-specifiers opt
// declaration-specifier → class  mutating  nonmutating  override  static  unowned  unowned(safe) unowned(unsafe)  weak”

int Parser::parseDeclarationSpecifiers()
{
    //class  mutating  nonmutating  override  static  unowned  unowned(safe) unowned(unsafe)  weak”
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
                ret |= TypeSpecifier::Class;
                continue;
            case Keyword::Mutating:
                ret |= TypeSpecifier::Mutating;
                continue;
            case Keyword::Nonmutating:
                ret |= TypeSpecifier::NonMutating;
                continue;
            case Keyword::Override:
                ret |= TypeSpecifier::Override;
                continue;
            case Keyword::Static:
                ret |= TypeSpecifier::Static;
                continue;
            case Keyword::Unowned:
                ret |= TypeSpecifier::Unowned;
                if(match(L"("))
                {
                    expect_identifier(token);
                    if(token == L"safe")
                        ret |= TypeSpecifier::Unowned_Safe;
                    else if(token == L"unsafe")
                        ret |= TypeSpecifier::Unowned_Unsafe;
                    else
                        unexpected(token);
                    expect(L")");
                }
                break;
            case Keyword::Weak:
                ret |= TypeSpecifier::Weak;
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
Declaration* Parser::parseImport(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Import, token);
    Import* ret = nodeFactory->createImport(token.state, attrs);
    expect_next(token);
    tassert(token, token.type == TokenType::Identifier);
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
Declaration* Parser::parseLet(const std::vector<Attribute*>& attrs, int specifiers)
{
    Token token;
    Flags flag(this, UNDER_LET);
    expect(Keyword::Let, token);
    Constant* ret = nodeFactory->createConstant(token.state, attrs, specifiers);
    do
    {
        Pattern* pattern = parsePattern();
        Expression* initializer = NULL;
        if(match(L"="))
        {
            initializer = parseExpression();
        }
        ret->add(pattern, initializer);
    }while(match(L","));
    return ret;
}


Variable* Parser::parseVariableDeclaration()
{
    Token token;
    Attributes attrs;
    
    Pattern* key = parsePattern();
    TypeNode* type = NULL;
    Expression* val = NULL;
    if(match(L":"))
    {
        type = parseTypeAnnotation();
    }
    if(match(L"="))
    {
        Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
        val = parseExpression();
    }
    Variable* ret = nodeFactory->createVariable(*key->getSourceInfo());
    ret->setName(key);
    ret->setTypeAttributes(attrs);
    ret->setType(type);
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
Declaration* Parser::parseVar(const std::vector<Attribute*>& attrs, int specifiers)
{
    Token token;
    expect(Keyword::Var, token);
    Flags flags(this, UNDER_VAR);
    //try read it as pattern-initializer-list
    Variables* ret = nodeFactory->createVariables(token.state, attrs, specifiers);
    Variable* var = parseVariableDeclaration();
    ret->addVariable(var);
    var->setSpecifiers(specifiers);
    if(predicate(L","))
    {
        while(match(L","))
        {
            var = parseVariableDeclaration();
            var->setSpecifiers(specifiers);
            ret->addVariable(var);
        }
        return ret;
    }

    if(!match(L"{"))
        return ret;
    peek(token);
    if(token.type != TokenType::CloseBrace)
    {
        Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
        switch(token.getKeyword())
        {
            case Keyword::Get:
            case Keyword::Set:
                if(this->flags & UNDER_PROTOCOL)
                {
                    // variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-keyword-block
                    //no code block for getter/setter for protocol
                    std::pair<CodeBlock*, CodeBlock*> r = parseGetterSetterKeywordBlock();
                    var->setGetter(r.first);
                    var->setSetter(r.second);
                }
                else
                {
                    //  variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-block
                    std::pair<CodeBlock*, std::pair<std::wstring, CodeBlock*> > r = parseGetterSetterBlock();
                    var->setGetter(r.first);
                    var->setSetterName(r.second.first);
                    var->setSetter(r.second.second);
                }
                break;
            case Keyword::WillSet:
            case Keyword::DidSet:
                //  variable-declaration → variable-declaration-head variable-name type-annotation initializer opt willSet-didSet-block
                parseWillSetDidSetBlock(var);
                break;
            default:
                //‌ variable-declaration → variable-declaration-head variable-name type-annotation code-block
                CodeBlock* getter = nodeFactory->createCodeBlock(token.state);
                var->setGetter(getter);
                do
                {
                    Statement* st = parseStatement();
                    getter->addStatement(st);
                }while(!predicate(L"}"));
                break;
        }
        
    }
    expect(L"}");
    return ret;
}

/*
willSet-clause → attributes opt willSet setter-name opt code-block
*/
void Parser::parseWillSetClause(Variable* variable, bool opt)
{
    Token token;
    if(!peek(token))
        return;
    if(opt && token.type == TokenType::CloseBrace)
        return;
    Attributes attrs;
    parseAttributes(attrs);
    expect(Keyword::WillSet);
    if(match(L"("))
    {
        expect_identifier(token);
        variable->setWillSetSetter(token.token);
        expect(L")");
    }
    CodeBlock* cb = parseCodeBlock();
    variable->setWillSet(cb);
}
/*
 ‌ didSet-clause → attributes opt didSet setter-name opt code-block
*/
void Parser::parseDidSetClause(Variable* variable, bool opt)
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
        variable->setDidSetSetter(token.token);
        expect(L")");
    }
    CodeBlock* cb = parseCodeBlock();
    variable->setDidSet(cb);
}

/*
 ‌ willSet-didSet-block → {willSet-clause didSet-clause opt}
 ‌ willSet-didSet-block → {didSet-clause willSet-clause}
 */
void Parser::parseWillSetDidSetBlock(Variable* variable)
{
    Token token;
    Attributes attrs;

    parseAttributes(attrs);
    if(predicate(L"willSet"))
    {
        //willSet-didSet-block → {willSet-clause didSet-clause opt}
        parseWillSetClause(variable, false);
        parseDidSetClause(variable, true);
        return;
    }
    if(predicate(L"didSet"))
    {
        parseDidSetClause(variable, false);
        parseWillSetClause(variable, true);
    }
}

/*
 ‌ getter-setter-block → {getter-clause setter-clauseopt}
 ‌ getter-setter-block → {setter-clause getter-clause}
 ‌ getter-clause → attributes opt get code-block
 ‌ setter-clause → attributes opt set setter-name opt code-block
 ‌ setter-name → (identifier)
*/
std::pair<CodeBlock*, std::pair<std::wstring, CodeBlock*> > Parser::parseGetterSetterBlock()
{
    Token token;
    std::vector<Attribute*> attrs;
    parseAttributes(attrs);
    peek(token);
    CodeBlock* getter = NULL;
    std::pair<std::wstring, CodeBlock*> setter = std::make_pair(std::wstring(), (CodeBlock*)NULL);
    if(token.getKeyword() == Keyword::Get)
    {
        expect_next(token);
        getter = parseCodeBlock();
        getter->setAttributes(attrs);
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
        expect(Keyword::Get);
        getter = parseCodeBlock();
        getter->setAttributes(attrs);
    }
    return std::make_pair(getter, setter);
}
/*
 ‌ setter-clause → attributes opt set setter-name opt code-block
*/
std::pair<std::wstring, CodeBlock*> Parser::parseSetterClause()
{
    Token token;
    std::vector<Attribute*> attrs;
    expect(Keyword::Set);
    std::wstring name;
    if(match(L"("))
    {
        expect_identifier(token);
        expect(L")");
        name = token.token;
    }
    CodeBlock* setter = parseCodeBlock();
    return std::make_pair(name, setter);
}
/*
 ‌ getter-setter-keyword-block → {getter-keyword-clause setter-keyword-clause opt}
 ‌ getter-setter-keyword-block → {setter-keyword-clause getter-keyword-clause}
 ‌ getter-keyword-clause → attributes opt get
 ‌ setter-keyword-clause → attributes opt set
*/
std::pair<CodeBlock*, CodeBlock*> Parser::parseGetterSetterKeywordBlock()
{
    Token token;
    Attributes attributes;
    parseAttributes(attributes);
    CodeBlock* getter = NULL;
    CodeBlock* setter = NULL;
    if(match(Keyword::Get, token))
    {
        getter = nodeFactory->createCodeBlock(token.state);
        getter->setAttributes(attributes);
        parseAttributes(attributes);
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
        expect(Keyword::Get, token);
        getter = nodeFactory->createCodeBlock(token.state);
        getter->setAttributes(attributes);
    }
    return std::make_pair(getter, setter);
}

/*
 “GRAMMAR OF A TYPE ALIAS DECLARATION
 
 ‌ typealias-declaration → typealias-head typealias-assignment
 ‌ typealias-head → typealias typealias-name
 ‌ typealias-name → identifier
 ‌ typealias-assignment → =type
*/
Declaration* Parser::parseTypealias(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Typealias, token);
    expect_identifier(token);
    expect(L"=");
    TypeNode* type = parseType();
    TypeAlias* ret = nodeFactory->createTypealias(token.state, attrs);
    ret->setName(token.token);
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
Declaration* Parser::parseFunc(const std::vector<Attribute*>& attrs, int specifiers)
{
    Token token;
    expect(Keyword::Func, token);
    FunctionDef* ret = nodeFactory->createFunction(token.state, attrs, specifiers);
    expect_next(token);
    if(token.type != TokenType::Identifier && token.type != TokenType::Operator)
        unexpected(token);
    ret->setName(token.token);
    if(predicate(L"<"))
    {
        GenericParameters* params = this->parseGenericParameters();
        ret->setGenericParameters(params);
    }
    do
    {
        Parameters* parameters = parseParameterClause();
        ret->addParameters(parameters);
    }while(predicate(L"("));
    if(match(L"->"))
    {
        //function-result → ->attributes opt type
        Attributes retAttrs;
        parseAttributes(retAttrs);
        TypeNode* retType = parseType();
        ret->setReturnTypeAttributes(retAttrs);
        ret->setReturnType(retType);
    }
    CodeBlock* body = parseCodeBlock();
    ret->setBody(body);
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
Parameters* Parser::parseParameterClause()
{
    Token token;
    expect(L"(", token);
    Parameters* ret = nodeFactory->createParameters(token.state);
    if(match(L")"))
        return ret;
    do
    {
        bool inout = match(Keyword::Inout);
        Parameter::Accessibility accessibility = Parameter::None;
        expect_next(token);
        if(token.type == TokenType::Identifier)
        {
            if(token.identifier.keyword == Keyword::Var)
            {
                expect_next(token);
                accessibility = Parameter::Variable;
            }
            else if(token.identifier.keyword == Keyword::Let)
            {
                expect_next(token);
                accessibility = Parameter::Constant;
            }
        }
        bool shorthandExternalName = false;
        if(token == L"#")
        {
            shorthandExternalName = true;
            expect_next(token);
        }
        tassert(token, token.type = TokenType::Identifier);
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
        Parameter* param = nodeFactory->createParameter(token.state);
        expect(L":");
        Attributes attrs;
        parseAttributes(attrs);
        TypeNode* type = parseType();
        Expression* def = NULL;
        if(match(L"="))
        {
            peek(token);
            def = parseExpression();
            tassert(token, def != NULL);
        }
        param->setInout(inout);
        param->setAccessibility(accessibility);
        param->setExternalName(name);
        param->setLocalName(localName);
        param->setShorthandExternalName(shorthandExternalName);
        param->setTypeAttributes(attrs);
        param->setType(type);
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
Declaration* Parser::parseEnum(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Enum);
    Flags flag(this);
    flags += UNDER_ENUM;
    expect_identifier(token);
    if(predicate(L"<"))
    {
        //TODO: read generic-parameter-clause
    }
    if(match(L":"))
    {
        //this is a raw-value-style-enum
        TypeIdentifier* baseType = parseTypeIdentifier();
        return parseRawValueEnum(attrs, token.token, baseType);
    }
    else
    {
        return parseUnionEnum(attrs, token.token);
    }
}
/*
 ‌ raw-value-style-enum → enum-name generic-parameter-clause opt : type-identifier{raw-value-style-enum-members opt}
 ‌ raw-value-style-enum-members → raw-value-style-enum-member raw-value-style-enum-members opt
 ‌ raw-value-style-enum-member → declaration | raw-value-style-enum-case-clause
 ‌ raw-value-style-enum-case-clause → attributes opt case raw-value-style-enum-case-list
 ‌ raw-value-style-enum-case-list → raw-value-style-enum-case | raw-value-style-enum-case,raw-value-style-enum-case-list
 ‌ raw-value-style-enum-case → enum-case-name raw-value-assignment opt
 ‌ raw-value-assignment → =literal
*/
Declaration* Parser::parseRawValueEnum(const std::vector<Attribute*>& attrs, const std::wstring& name, TypeIdentifier* baseType)
{
    Token token;
    expect(L"{", token);
    EnumDef* ret = nodeFactory->createEnum(token.state, attrs);
    ret->setIdentifier(nodeFactory->createTypeIdentifier(token.state, name));
    ret->addParent(baseType);
    while(!predicate(L"}"))
    {
        if(match(Keyword::Case))
        {
            do
            {
                expect_identifier(token);
                Expression* val = NULL;
                if(match(L"="))
                {
                    val = parseLiteral();
                }
                ret->addConstant(token.token, val);
            }while(match(L","));
        }
        else
        {
            Declaration* decl = parseDeclaration();
            ret->addDeclaration(decl);
        }
    }
    expect(L"}");
    return ret;
}
/*
 ‌ union-style-enum → enum-name generic-parameter-clause opt{union-style-enum-members opt}
 ‌ union-style-enum-members → union-style-enum-member union-style-enum-members opt
 ‌ union-style-enum-member → declaration | union-style-enum-case-clause
 ‌ union-style-enum-case-clause → attributes opt case union-style-enum-case-list
 ‌ union-style-enum-case-list → union-style-enum-case | union-style-enum-case,union-style-enum-case-list
 ‌ union-style-enum-case → enum-case-name tuple-type opt
*/
Declaration* Parser::parseUnionEnum(const std::vector<Attribute*>& attrs, const std::wstring& name)
{
    Token token;
    expect(L"{", token);
    EnumDef* ret = nodeFactory->createEnum(token.state, attrs);
    ret->setIdentifier(nodeFactory->createTypeIdentifier(token.state, name));
    while(!predicate(L"}"))
    {
        if(match(Keyword::Case))
        {
            do
            {
                expect_identifier(token);
                TupleType* associatedType = NULL;
                if(predicate(L"("))
                {
                    associatedType = parseTupleType();
                }
                ret->addAssociatedType(token.token, associatedType);
            }while(match(L","));
        }
        else
        {
            Declaration* decl = parseDeclaration();
            ret->addDeclaration(decl);
        }
    }
    expect(L"}");
    return ret;
}

/*
 “GRAMMAR OF A STRUCTURE DECLARATION
 
 ‌ struct-declaration → attributes opt struct struct-name generic-parameter-clause opt type-inheritance-clause opt struct-body
 ‌ struct-name → identifier
 ‌ struct-body → {declarations opt}”
*/
Declaration* Parser::parseStruct(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Struct);
    StructDef* ret = nodeFactory->createStruct(token.state, attrs);
    expect_identifier(token);
    ret->setIdentifier(nodeFactory->createTypeIdentifier(token.state, token.token));
    if(predicate(L"<"))
    {
        GenericParameters* params = parseGenericParameters();
        ret->setGenericParameters(params);
    }
    
    
    if(match(L":"))
    {
        do
        {
            TypeIdentifier* protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    
    Flags f(this);
    flags += UNDER_STRUCT;
    
    expect(L"{");
    while(!predicate(L"}"))
    {
        Declaration* decl = parseDeclaration();
        ret->addDeclaration(decl);
    }
    expect(L"}");
    return ret;
}
/*
 “GRAMMAR OF A CLASS DECLARATION
 
 ‌ class-declaration → attributes opt class class-name generic-parameter-clause opt type-inheritance-clause opt class-body
 ‌ class-name → identifier
 ‌ class-body → {declarations opt}”
 
*/
Declaration* Parser::parseClass(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Class, token);
    ClassDef* ret = nodeFactory->createClass(token.state, attrs);
    expect_identifier(token);
    ret->setIdentifier(nodeFactory->createTypeIdentifier(token.state, token.token));
    if(predicate(L"<"))
    {
        GenericParameters* params = parseGenericParameters();
        ret->setGenericParameters(params);
    }
    if(match(L":"))
    {
        do
        {
            TypeIdentifier* protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    
    Flags f(this);
    f += UNDER_CLASS;
    
    expect(L"{");
    while(!predicate(L"}"))
    {
        Declaration* decl = parseDeclaration();
        ret->addDeclaration(decl);
    }
    expect(L"}");
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
Declaration* Parser::parseProtocol(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Struct);
    ProtocolDef* ret = nodeFactory->createProtocol(token.state, attrs);
    expect_identifier(token);
    ret->setIdentifier(nodeFactory->createTypeIdentifier(token.state, token.token));
    if(predicate(L"<"))
    {
        //TODO: parse generic parameter clause
    }
    if(match(L":"))
    {
        do
        {
            TypeIdentifier* protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    expect(L"{");
    
    Flags f(this);
    f += UNDER_PROTOCOL;
    
    
    while(!predicate(L"}"))
    {
        Declaration* decl = parseDeclaration();
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
Declaration* Parser::parseInit(const std::vector<Attribute*>& attrs)
{
    Token token;
    bool convenience = match(L"convenience");
    expect(Keyword::Init, token);
    if(predicate(L"<"))
    {
        //TODO: generic-parameter-clause
    }
    Parameters* parameters = parseParameterClause();
    CodeBlock* body = parseCodeBlock();
    InitializerDef* ret = nodeFactory->createInitializer(token.state, attrs);
    ret->setConvenience(convenience);
    ret->setParameters(parameters);
    ret->setBody(body);
    return ret;
}
/*
  GRAMMAR OF A DEINITIALIZER DECLARATION
 
 ‌ deinitializer-declaration → attributes opt deinit code-block
*/
Declaration* Parser::parseDeinit(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Deinit, token);
    DeinitializerDef* ret = nodeFactory->createDeinitializer(token.state, attrs);
    CodeBlock* body = parseCodeBlock();
    ret->setBody(body);
    return ret;
}
/*
 “GRAMMAR OF AN EXTENSION DECLARATION
 
 ‌ extension-declaration → extension type-identifier type-inheritance-clause opt extension-body
 ‌ extension-body → {declarationsopt}”
*/
Declaration* Parser::parseExtension(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Extension, token);
    TypeIdentifier* id = parseTypeIdentifier();
    ExtensionDef* ret = nodeFactory->createExtension(token.state, attrs);
    ret->setIdentifier(id);
    if(match(L":"))
    {
        // type-inheritance-list → type-identifier  type-identifier,type-inheritance-list
        do
        {
            TypeIdentifier* protocol = parseTypeIdentifier();
            ret->addParent(protocol);
        }while(match(L","));
    }
    expect(L"{");
    while(!predicate(L"}"))
    {
        Declaration* decl = parseDeclaration();
        ret->addDeclaration(decl);
    }
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
Declaration* Parser::parseSubscript(const std::vector<Attribute*>& attrs)
{
    Token token;
    expect(Keyword::Subscript, token);
    Parameters* params = parseParameterClause();
    expect(L"->");
    Attributes typeAttrs;
    parseAttributes(typeAttrs);
    TypeNode* retType = parseType();
    SubscriptDef* ret = nodeFactory->createSubscript(token.state, attrs);
    ret->setParameters(params);
    ret->setReturnType(retType);
    ret->setReturnTypeAttributes(typeAttrs);
    
    if(flags & UNDER_PROTOCOL)
    {
        std::pair<CodeBlock*, CodeBlock*> accessors = parseGetterSetterKeywordBlock();
        ret->setGetter(accessors.first);
        ret->setSetter(accessors.second);
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
            std::pair<CodeBlock*, std::pair<std::wstring, CodeBlock*> > accessors = parseGetterSetterBlock();
            ret->setGetter(accessors.first);
            ret->setSetter(accessors.second.second);
            ret->setSetterName(accessors.second.first);
            expect(L"}");
        }
        else
        {
            tokenizer->restore(s);
            CodeBlock* cb = parseCodeBlock();
            ret->setGetter(cb);
        }
    }
    return ret;
}
/*
  GRAMMAR OF AN OPERATOR DECLARATION
 
 ‌ operator-declaration → prefix-operator-declaration | postfix-operator-declaration | infix-operator-declaration
 ‌ prefix-operator-declaration → operator prefix operator{}
 ‌ postfix-operator-declaration → operator postfix operator{}
 ‌ infix-operator-declaration → operator infix operator{infix-operator-attributes opt}
 ‌ infix-operator-attributes → precedence-clause opt associativity-clause opt
 ‌ precedence-clause → precedence precedence-level
 ‌ precedence-level → Digit 0 through 255
 ‌ associativity-clause → associativity associativity
 ‌ associativity → left  right  none
*/
Declaration* Parser::parseOperator(const std::vector<Attribute*>& attrs)
{
    Token token;
    OperatorType::T type = OperatorType::_;
    expect(Keyword::Operator, token);
    OperatorDef* op = nodeFactory->createOperator(token.state, attrs);
    expect_next(token);
    switch(token.getKeyword())
    {
        case Keyword::Prefix:
            type = OperatorType::PrefixUnary;
            break;
        case Keyword::Postfix:
            type = OperatorType::PostfixUnary;
            break;
        case Keyword::Infix:
            type = OperatorType::InfixBinary;
            break;
        default:
            unexpected(token);
            break;
    }
    expect_next(token);
    tassert(token, token.type == TokenType::Operator);
    op->setName(token.token);
    expect(L"{");
    while(!match(L"}"))
    {
        expect_next(token);
        switch(token.getKeyword())
        {
            case Keyword::Precedence:
                expect_next(token);
                tassert(token, token.type == TokenType::Integer);
                tassert(token, token.number.value >= 0 && token.number.value <= 255);
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
            default:
                unexpected(token);
        }
    }
    return op;
}
