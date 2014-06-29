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
 ‌ declarations → declarationdeclarationsopt
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
    if((flags & UNDER_CLASS) || (flags & UNDER_PROTOCOL))
    {
        //read declaration specifiers
        specifiers = parseDeclarationSpecifiers();
    }
    match_identifier(token);
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
// declaration-specifiers → declaration-specifierdeclaration-specifiersopt
// declaration-specifier → class  mutating  nonmutating  override  static  unowned  unowned(safe) unowned(unsafe)  weak”

int Parser::parseDeclarationSpecifiers()
{
    //class  mutating  nonmutating  override  static  unowned  unowned(safe) unowned(unsafe)  weak”
    Token token;
    int ret = 0;
    while(match_identifier(token))
    {
        switch(token.identifier.keyword)
        {
            case Keyword::Class:
                ret |= SPECIFIER_CLASS;
                continue;
            case Keyword::Mutating:
                ret |= SPECIFIER_MUTATING;
                continue;
            case Keyword::Nonmutating:
                ret |= SPECIFIER_NONMUTATING;
                continue;
            case Keyword::Override:
                ret |= SPECIFIER_OVERRIDE;
                continue;
            case Keyword::Static:
                ret |= SPECIFIER_STATIC;
                continue;
            case Keyword::Unowned:
                ret |= SPECIFIER_UNOWNED;
                if(match(L"("))
                {
                    expect_identifier(token);
                    if(token == L"safe")
                        ret |= SPECIFIER_UNOWNED_SAFE;
                    else if(token == L"unsafe")
                        ret |= SPECIFIER_UNOWNED_UNSAFE;
                    else
                        unexpected(token);
                    expect(L")");
                }
                break;
            case Keyword::Weak:
                ret |= SPECIFIER_WEAK;
                continue;
            default:
                tokenizer->restore(token);
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
    expect(Keyword::Import);
    next(token);
    tassert(token, token.type == TokenType::Identifier);
    Import::Kind kind = Import::_;
    switch(token.identifier.keyword)
    {
        case Keyword::_:
            tokenizer->restore(token);
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
        next(token);
        if(token.type != TokenType::Identifier && token.type != TokenType::Operator)
            unexpected(token);
        if(n)
            path += L".";
        path += token.token;
        n++;
    } while (match(L"."));
    Import* ret = nodeFactory->createImport(attrs);
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
    Constant* ret = nodeFactory->createConstant(attrs, specifiers);
    expect(Keyword::Let);
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
    expect(L":");
    if(predicate(L"@"))
    {
        parseAttributes(attrs);
    }
    TypeNode* type = parseType();
    
    Variable* ret = nodeFactory->createVariable();
    ret->setName(key);
    ret->setTypeAttributes(attrs);
    ret->setType(type);
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
    expect(Keyword::Var);
    //try read it as pattern-initializer-list
    Variables* ret = nodeFactory->createVariables(attrs, specifiers);
    Variable* var = parseVariableDeclaration();
    ret->addVariable(var);
    if(predicate(L","))
    {
        while(match(L","))
        {
            var = parseVariableDeclaration();
            ret->addVariable(var);
        }
        return ret;
    }

    if(!match(L"{"))
        return ret;
    peek(token);
    if(token.type != TokenType::CloseBrace)
    {
        switch(token.getKeyword())
        {
            case Keyword::Get:
            case Keyword::Set:
                if(flags & UNDER_PROTOCOL)
                {
                    // variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-keyword-block
                    //no code block for getter/setter for protocol
                    parseGetterSetterKeywordBlock(var);
                }
                else
                {
                    //  variable-declaration → variable-declaration-head variable-name type-annotation getter-setter-block
                    parseGetterSetterBlock(var);
                }
                break;
            case Keyword::WillSet:
            case Keyword::DidSet:
                //  variable-declaration → variable-declaration-head variable-name type-annotation initializer opt willSet-didSet-block
                parseWillSetDidSetBlock(var);
                break;
            default:
                //‌ variable-declaration → variable-declaration-head variable-name type-annotation code-block
                CodeBlock* getter = nodeFactory->createCodeBlock();
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
 ‌ willSet-didSet-block → {willSet-clause didSet-clause opt}
 ‌ willSet-didSet-block → {didSet-clause willSet-clause}
 ‌ willSet-clause → attributes opt willSet setter-name opt code-block
 ‌ didSet-clause → attributes opt didSet setter-name opt code-block
*/

void Parser::parseWillSetDidSetBlock(Variable* variable)
{
    Token token;
    Attributes attrs;

    parseAttributes(attrs);
    if(match(Keyword::WillSet))
    {
        //willSet-didSet-block → {willSet-clause didSet-clause opt}
        CodeBlock* willSet = parseCodeBlock();
        willSet->setAttributes(attrs);
        attrs.clear();
        
        parseAttributes(attrs);
        variable->setWillSet(willSet);
        if(match(Keyword::DidSet))
        {
            if(match_identifier(token))
                variable->setSetterName(token.token);
            CodeBlock* didSet = parseCodeBlock();
            didSet->setAttributes(attrs);
            variable->setDidSet(didSet);
        }
        else if(!attrs.empty())
            expect(Keyword::DidSet);
        return;
    }
    if(match(Keyword::DidSet))
    {
        //willSet-didSet-block → {didSet-clause willSet-clause}
        if(match_identifier(token))
            variable->setSetterName(token.token);
        CodeBlock* didSet = parseCodeBlock();
        didSet->setAttributes(attrs);
        variable->setDidSet(didSet);
        parseAttributes(attrs);
        expect(Keyword::WillSet);
        CodeBlock* willSet = parseCodeBlock();
        variable->setWillSet(willSet);
        willSet->setAttributes(attrs);
    }
}

/*
 ‌ getter-setter-block → {getter-clause setter-clauseopt}
 ‌ getter-setter-block → {setter-clause getter-clause}
 ‌ getter-clause → attributes opt get code-block
 ‌ setter-clause → attributes opt set setter-name opt code-block
 ‌ setter-name → (identifier)
*/
void Parser::parseGetterSetterBlock(Variable* variable)
{
    Token token;
    std::vector<Attribute*> attrs;
    parseAttributes(attrs);
    peek(token);
    if(token.getKeyword() == Keyword::Get)
    {
        next(token);
        CodeBlock* getter = parseCodeBlock();
        variable->setGetter(getter);
        getter->setAttributes(attrs);
        peek(token);
        if(token == L"@" || token.getKeyword() == Keyword::Set)
        {
            parseSetterClause(variable);
        }
    }
    else if(token.getKeyword() == Keyword::Set)
    {
        parseSetterClause(variable);
        variable->getSetter()->setAttributes(attrs);
        parseAttributes(attrs);
        expect(Keyword::Get);
        CodeBlock* getter = parseCodeBlock();
        variable->setGetter(getter);
        getter->setAttributes(attrs);
    }
}
/*
 ‌ setter-clause → attributes opt set setter-name opt code-block
*/
void Parser::parseSetterClause(Variable* variable)
{
    Token token;
    std::vector<Attribute*> attrs;
    expect(Keyword::Set);
    if(match(L"("))
    {
        expect_identifier(token);
        expect(L")");
        variable->setSetterName(token.token);
    }
    CodeBlock* setter = parseCodeBlock();
    variable->setSetter(setter);
}
/*
 ‌ getter-setter-keyword-block → {getter-keyword-clause setter-keyword-clause opt}
 ‌ getter-setter-keyword-block → {setter-keyword-clause getter-keyword-clause}
 ‌ getter-keyword-clause → attributes opt get
 ‌ setter-keyword-clause → attributes opt set
*/
void Parser::parseGetterSetterKeywordBlock(Variable* variable)
{
    Attributes attributes;
    parseAttributes(attributes);
    if(match(Keyword::Get))
    {
        CodeBlock* getter = nodeFactory->createCodeBlock();
        getter->setAttributes(attributes);
        variable->setGetter(getter);

        
        parseAttributes(attributes);
        if(match(Keyword::Set))
        {
            CodeBlock* setter = nodeFactory->createCodeBlock();
            setter->setAttributes(attributes);
            variable->setSetter(setter);
        }
    }
    else if(match(Keyword::Set))
    {
        CodeBlock* setter = nodeFactory->createCodeBlock();
        setter->setAttributes(attributes);
        variable->setSetter(setter);
        
        parseAttributes(attributes);
        expect(Keyword::Get);
        CodeBlock* getter = nodeFactory->createCodeBlock();
        getter->setAttributes(attributes);
        variable->setGetter(getter);
    }

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
    expect(Keyword::Typealias);
    expect_identifier(token);
    expect(L"=");
    TypeNode* type = parseType();
    TypeAlias* ret = nodeFactory->createTypealias(attrs);
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
 ‌ parameter-clauses → parameter-clause parameter-clauses opt
 ‌ parameter-clause → ()  (parameter-list...opt)
 ‌ parameter-list → parameter | parameter,parameter-list
 ‌ parameter → inout opt let opt#opt parameter-name local-parameter-name opt type-annotation default-argument-clause opt
 ‌ parameter → inout opt var #opt parameter-name local-parameter-name opt type-annotation default-argument-clause opt
 ‌ parameter → attributes opt type
 ‌ parameter-name → identifier  _
 ‌ local-parameter-name → identifier  _
 ‌ default-argument-clause → =expression”
*/
Declaration* Parser::parseFunc(const std::vector<Attribute*>& attrs, int specifiers)
{
    
}
/*
  GRAMMAR OF AN ENUMERATION DECLARATION
 
 ‌ enum-declaration → attributes opt union-style-enum | attributes opt raw-value-style-enum
 ‌ union-style-enum → enum-name generic-parameter-clause opt{union-style-enum-members opt}
 ‌ union-style-enum-members → union-style-enum-member union-style-enum-members opt
 ‌ union-style-enum-member → declaration | union-style-enum-case-clause
 ‌ union-style-enum-case-clause → attributes opt case union-style-enum-case-list
 ‌ union-style-enum-case-list → union-style-enum-case | union-style-enum-case,union-style-enum-case-list
 ‌ union-style-enum-case → enum-case-name tuple-type opt
 ‌ enum-name → identifier
 ‌ enum-case-name → identifier
 ‌ raw-value-style-enum → enum-name generic-parameter-clause opt : type-identifier{raw-value-style-enum-members opt}
 ‌ raw-value-style-enum-members → raw-value-style-enum-member raw-value-style-enum-members opt
 ‌ raw-value-style-enum-member → declaration | raw-value-style-enum-case-clause
 ‌ raw-value-style-enum-case-clause → attributes opt case raw-value-style-enum-case-list
 ‌ raw-value-style-enum-case-list → raw-value-style-enum-case | raw-value-style-enum-case,raw-value-style-enum-case-list
 ‌ raw-value-style-enum-case → enum-case-name raw-value-assignment opt
 ‌ raw-value-assignment → =literal
 
*/
Declaration* Parser::parseEnum(const std::vector<Attribute*>& attrs)
{
    
}
/*
 “GRAMMAR OF A STRUCTURE DECLARATION
 
 ‌ struct-declaration → attributes opt struct struct-name generic-parameter-clause opt type-inheritance-clause opt struct-body
 ‌ struct-name → identifier
 ‌ struct-body → {declarations opt}”
*/
Declaration* Parser::parseStruct(const std::vector<Attribute*>& attrs)
{
    
}
/*
 “GRAMMAR OF A CLASS DECLARATION
 
 ‌ class-declaration → attributes opt class class-name generic-parameter-clause opt type-inheritance-clause opt class-body
 ‌ class-name → identifier
 ‌ class-body → {declarations opt}”
 
*/
Declaration* Parser::parseClass(const std::vector<Attribute*>& attrs)
{
    
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
    
}
/*
  GRAMMAR OF AN INITIALIZER DECLARATION
 
 ‌ initializer-declaration → initializer-head generic-parameter-clause opt parameter-clause initializer-body
 ‌ initializer-head → attributes opt convenience opt init
 ‌ initializer-body → code-block
 
*/
Declaration* Parser::parseInit(const std::vector<Attribute*>& attrs)
{
    
}
/*
  GRAMMAR OF A DEINITIALIZER DECLARATION
 
 ‌ deinitializer-declaration → attributes opt deinit code-block
*/
Declaration* Parser::parseDeinit(const std::vector<Attribute*>& attrs)
{
    expect(Keyword::Deinit);
    DeinitializerDef* ret = nodeFactory->createDeinitializer(attrs);
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
    expect(Keyword::Operator);
    next(token);
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
    next(token);
    tassert(token, token.type == TokenType::Operator);
    OperatorDef* op = nodeFactory->createOperator(attrs);
    op->setName(token.token);
    expect(L"{");
    while(!match(L"}"))
    {
        next(token);
        switch(token.getKeyword())
        {
            case Keyword::Precedence:
                next(token);
                tassert(token, token.type == TokenType::Integer);
                tassert(token, token.number.value >= 0 && token.number.value <= 255);
                op->setPrecedence(token.number.value);
                break;
            case Keyword::Associativity:
                next(token);
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
