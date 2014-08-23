#include "Parser.h"
#include "tokenizer/Tokenizer.h"
#include "ast/NodeFactory.h"
#include "ast/ast.h"
#include "parser_details.h"
#include "swift_errors.h"

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
DeclarationPtr Parser::parseDeclaration()
{
    Token token;
    std::vector<AttributePtr> attrs;
    if(predicate(L"@"))
        parseAttributes(attrs);
    int specifiers = 0;
    if(flags & (UNDER_CLASS | UNDER_STRUCT | UNDER_ENUM | UNDER_PROTOCOL |UNDER_EXTENSION))
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
            attrs.clear();
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
DeclarationPtr Parser::parseImport(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Import, token);
    ImportPtr ret = nodeFactory->createImport(token.state);
    ret->setAttributes(attrs);
    expect_next(token);
    tassert(token, token.type == TokenType::Identifier, Errors::E_EXPECT_IDENTIFIER, token.token);
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
DeclarationPtr Parser::parseLet(const std::vector<AttributePtr>& attrs, int specifiers)
{
    Token token;
    Flags flag(this, UNDER_LET);
    expect(Keyword::Let, token);
    ConstantsPtr ret = nodeFactory->createConstants(token.state);
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    do
    {
        PatternPtr pattern = parsePattern();
        ExpressionPtr initializer = NULL;
        if(match(L"="))
        {
            initializer = parseExpression();
        }

        ConstantPtr constant = nodeFactory->createConstant(*pattern->getSourceInfo());
        constant->setAttributes(attrs);
        constant->setSpecifiers(specifiers);
        constant->name = pattern;
        constant->initializer = initializer;
        ret->add(constant);
    }while(match(L","));
    return ret;
}


VariablePtr Parser::parseVariableDeclaration()
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
    if(match(L"="))
    {
        Flags flags(this, SUPPRESS_TRAILING_CLOSURE);
        val = parseExpression();
    }
    VariablePtr ret = nodeFactory->createVariable(*key->getSourceInfo());
    ret->setName(key);
    ret->setTypeAttributes(attrs);
    ret->setDeclaredType(type);
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
DeclarationPtr Parser::parseVar(const std::vector<AttributePtr>& attrs, int specifiers)
{
    Token token;
    expect(Keyword::Var, token);
    Flags flags(this, UNDER_VAR);
    //try read it as pattern-initializer-list
    VariablesPtr ret = nodeFactory->createVariables(token.state);
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    VariablePtr var = parseVariableDeclaration();
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
                    std::pair<CodeBlockPtr, CodeBlockPtr> r = parseGetterSetterKeywordBlock();
                    var->setGetter(r.first);
                    var->setSetter(r.second);
                }
                else
                {
                    //  variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-block
                    std::pair<CodeBlockPtr, std::pair<std::wstring, CodeBlockPtr> > r = parseGetterSetterBlock();
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
                CodeBlockPtr getter = nodeFactory->createCodeBlock(token.state);
                var->setGetter(getter);
                do
                {
                    StatementPtr st = parseStatement();
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
void Parser::parseWillSetClause(const VariablePtr& variable, bool opt)
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
    CodeBlockPtr cb = parseCodeBlock();
    variable->setWillSet(cb);
}
/*
 ‌ didSet-clause → attributes opt didSet setter-name opt code-block
*/
void Parser::parseDidSetClause(const VariablePtr& variable, bool opt)
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
    CodeBlockPtr cb = parseCodeBlock();
    variable->setDidSet(cb);
}

/*
 ‌ willSet-didSet-block → {willSet-clause didSet-clause opt}
 ‌ willSet-didSet-block → {didSet-clause willSet-clause}
 */
void Parser::parseWillSetDidSetBlock(const VariablePtr& variable)
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
std::pair<CodeBlockPtr, std::pair<std::wstring, CodeBlockPtr> > Parser::parseGetterSetterBlock()
{
    Token token;
    Attributes attrs;
    parseAttributes(attrs);
    peek(token);
    CodeBlockPtr getter = NULL;
    std::pair<std::wstring, CodeBlockPtr> setter = std::make_pair(std::wstring(), CodeBlockPtr());
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
    parseAttributes(attributes);
    CodeBlockPtr getter = NULL;
    CodeBlockPtr setter = NULL;
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
DeclarationPtr Parser::parseTypealias(const Attributes& attrs)
{
    Token token;
    expect(Keyword::Typealias, token);
    expect_identifier(token);
    expect(L"=");
    TypeNodePtr type = parseType();
    TypeAliasPtr ret = nodeFactory->createTypealias(token.state);
    ret->setAttributes(attrs);
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
DeclarationPtr Parser::parseFunc(const std::vector<AttributePtr>& attrs, int specifiers)
{
    Token token;
    expect(Keyword::Func, token);
    FunctionDefPtr ret = nodeFactory->createFunction(token.state);
    ret->setAttributes(attrs);
    ret->setSpecifiers(specifiers);
    expect_next(token);
    if(token.type != TokenType::Identifier && token.type != TokenType::Operator)
        unexpected(token);
    ret->setName(token.token);
    if(predicate(L"<"))
    {
        GenericParametersPtr params = this->parseGenericParameters();
        ret->setGenericParameters(params);
    }
    do
    {
        ParametersPtr parameters = parseParameterClause();
        ret->addParameters(parameters);
    }while(predicate(L"("));
    if(match(L"->"))
    {
        //function-result → ->attributes opt type
        Attributes retAttrs;
        parseAttributes(retAttrs);
        TypeNodePtr retType = parseType();
        ret->setReturnTypeAttributes(retAttrs);
        ret->setReturnType(retType);
    }
    if((UNDER_PROTOCOL & flags) == 0)
    {
        CodeBlockPtr body = parseCodeBlock();
        ret->setBody(body);
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
ParametersPtr Parser::parseParameterClause()
{
    Token token;
    expect(L"(", token);
    ParametersPtr ret = nodeFactory->createParameters(token.state);
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
        tassert(token, token.type = TokenType::Identifier, Errors::E_EXPECT_IDENTIFIER, token.token);
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
        ParameterPtr param = nodeFactory->createParameter(token.state);
        expect(L":");
        Attributes attrs;
        parseAttributes(attrs);
        TypeNodePtr type = parseType();
        ExpressionPtr def = NULL;
        if(match(L"="))
        {
            peek(token);
            def = parseExpression();
            tassert(token, def != NULL, Errors::E_EXPECT_EXPRESSION, token.token);
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
DeclarationPtr Parser::parseEnum(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Enum);
    Flags flag(this);
    flags += UNDER_ENUM;
    expect_identifier(token);
    if(match(L":"))
    {
        //this is a raw-value-style-enum
        TypeIdentifierPtr baseType = parseTypeIdentifier();
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
DeclarationPtr Parser::parseRawValueEnum(const std::vector<AttributePtr>& attrs, const std::wstring& name, const TypeIdentifierPtr& baseType)
{
    Token token;
    expect(L"{", token);
    EnumDefPtr ret = nodeFactory->createEnum(token.state);
    ret->setAttributes(attrs);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(name);
    ret->setIdentifier(typeId);
    ret->addParent(baseType);
    while(!predicate(L"}"))
    {
        if(match(Keyword::Case))
        {
            do
            {
                expect_identifier(token);
                ExpressionPtr val = NULL;
                if(match(L"="))
                {
                    val = parseLiteral();
                }
                ret->addConstant(token.token, val);
            }while(match(L","));
        }
        else
        {
            DeclarationPtr decl = parseDeclaration();
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
DeclarationPtr Parser::parseUnionEnum(const std::vector<AttributePtr>& attrs, const std::wstring& name)
{
    Token token;
    expect(L"{", token);
    EnumDefPtr ret = nodeFactory->createEnum(token.state);
    ret->setAttributes(attrs);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(name);
    ret->setIdentifier(typeId);

    while(!predicate(L"}"))
    {
        if(match(Keyword::Case))
        {
            do
            {
                expect_identifier(token);
                TupleTypePtr associatedType = NULL;
                if(predicate(L"("))
                {
                    associatedType = parseTupleType();
                }
                ret->addAssociatedType(token.token, associatedType);
            }while(match(L","));
        }
        else
        {
            DeclarationPtr decl = parseDeclaration();
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
DeclarationPtr Parser::parseStruct(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Struct);
    StructDefPtr ret = nodeFactory->createStruct(token.state);
    ret->setAttributes(attrs);
    expect_identifier(token);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(token.token);
    ret->setIdentifier(typeId);

    if(predicate(L"<"))
    {
        GenericParametersPtr params = parseGenericParameters();
        ret->setGenericParameters(params);
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
    
    expect(L"{");
    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
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
DeclarationPtr Parser::parseClass(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Class, token);
    ClassDefPtr ret = nodeFactory->createClass(token.state);
    ret->setAttributes(attrs);
    expect_identifier(token);
    TypeIdentifierPtr typeId = nodeFactory->createTypeIdentifier(token.state);
    typeId->setName(token.token);
    ret->setIdentifier(typeId);
    if(predicate(L"<"))
    {
        GenericParametersPtr params = parseGenericParameters();
        ret->setGenericParameters(params);
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
    
    expect(L"{");
    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
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
DeclarationPtr Parser::parseProtocol(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Protocol);
    ProtocolDefPtr ret = nodeFactory->createProtocol(token.state);
    ret->setAttributes(attrs);
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
DeclarationPtr Parser::parseInit(const std::vector<AttributePtr>& attrs)
{
    Token token;
    bool convenience = match(L"convenience");
    expect(Keyword::Init, token);
    InitializerDefPtr ret = nodeFactory->createInitializer(token.state);
    ret->setAttributes(attrs);
    if(predicate(L"<"))
    {
        GenericParametersPtr params = this->parseGenericParameters();
        ret->setGenericParameters(params);   
    }
    ParametersPtr parameters = parseParameterClause();
    CodeBlockPtr body = parseCodeBlock();
    ret->setConvenience(convenience);
    ret->setParameters(parameters);
    ret->setBody(body);
    return ret;
}
/*
  GRAMMAR OF A DEINITIALIZER DECLARATION
 
 ‌ deinitializer-declaration → attributes opt deinit code-block
*/
DeclarationPtr Parser::parseDeinit(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Deinit, token);
    DeinitializerDefPtr ret = nodeFactory->createDeinitializer(token.state);
    ret->setAttributes(attrs);
    CodeBlockPtr body = parseCodeBlock();
    ret->setBody(body);
    return ret;
}
/*
 “GRAMMAR OF AN EXTENSION DECLARATION
 
 ‌ extension-declaration → extension type-identifier type-inheritance-clause opt extension-body
 ‌ extension-body → {declarationsopt}”
*/
DeclarationPtr Parser::parseExtension(const std::vector<AttributePtr>& attrs)
{
    Token token;
    Flags flags(this, UNDER_EXTENSION);
    expect(Keyword::Extension, token);
    TypeIdentifierPtr id = parseTypeIdentifier();
    ExtensionDefPtr ret = nodeFactory->createExtension(token.state);
    ret->setAttributes(attrs);
    ret->setIdentifier(id);
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
    while(!predicate(L"}"))
    {
        DeclarationPtr decl = parseDeclaration();
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
DeclarationPtr Parser::parseSubscript(const std::vector<AttributePtr>& attrs)
{
    Token token;
    expect(Keyword::Subscript, token);
    ParametersPtr params = parseParameterClause();
    expect(L"->");
    Attributes typeAttrs;
    parseAttributes(typeAttrs);
    TypeNodePtr retType = parseType();
    SubscriptDefPtr ret = nodeFactory->createSubscript(token.state);
    ret->setAttributes(attrs);
    ret->setParameters(params);
    ret->setReturnType(retType);
    ret->setReturnTypeAttributes(typeAttrs);
    
    if(flags & UNDER_PROTOCOL)
    {
        std::pair<CodeBlockPtr, CodeBlockPtr> accessors = parseGetterSetterKeywordBlock();
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
DeclarationPtr Parser::parseOperator(const std::vector<AttributePtr>& attrs)
{
    Token token;
    OperatorType::T type = OperatorType::_;
    expect(Keyword::Operator, token);
    OperatorDefPtr op = nodeFactory->createOperator(token.state);
    op->setAttributes(attrs);
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
    tassert(token, token.type == TokenType::Operator, Errors::E_EXPECT_OPERATOR, token.token);
    op->setName(token.token);
    expect(L"{");
    while(!match(L"}"))
    {
        expect_next(token);
        switch(token.getKeyword())
        {
            case Keyword::Precedence:
                expect_next(token);
                tassert(token, token.type == TokenType::Integer, Errors::E_EXPECT_INTEGER_PRECEDENCE, token.token);
                tassert(token, token.number.value >= 0 && token.number.value <= 255, Errors::E_INVALID_OPERATOR_PRECEDENCE, token.token);
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
