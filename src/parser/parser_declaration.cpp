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
            return parseInit(attrs, specifiers);
        case Keyword::Deinit://declaration → deinitializer-declaration
            return parseDeinit(attrs, specifiers);
        case Keyword::Extension://declaration → extension-declaration
            return parseExtension(attrs);
        case Keyword::Subscript://declaration → subscript-declaration
            return parseSubscript(attrs, specifiers);
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
Declaration* Parser::parseVar(const std::vector<Attribute*>& attrs, int specifiers)
{
    
}
Declaration* Parser::parseTypealias(const std::vector<Attribute*>& attrs)
{
    
}
Declaration* Parser::parseFunc(const std::vector<Attribute*>& attrs, int specifiers)
{
    
}
Declaration* Parser::parseEnum(const std::vector<Attribute*>& attrs)
{
    
}
Declaration* Parser::parseStruct(const std::vector<Attribute*>& attrs)
{
    
}
Declaration* Parser::parseClass(const std::vector<Attribute*>& attrs)
{
    
}
Declaration* Parser::parseProtocol(const std::vector<Attribute*>& attrs)
{
    
}
Declaration* Parser::parseInit(const std::vector<Attribute*>& attrs, int specifiers)
{
    
}
Declaration* Parser::parseDeinit(const std::vector<Attribute*>& attrs, int specifiers)
{
    
}
Declaration* Parser::parseExtension(const std::vector<Attribute*>& attrs)
{
    
}
Declaration* Parser::parseSubscript(const std::vector<Attribute*>& attrs, int specifiers)
{
    
}
Declaration* Parser::parseOperator(const std::vector<Attribute*>& attrs)
{
    
}



/*
 “ import-declaration → attributesoptimportimport-kindoptimport-path
 ‌ import-kind → typealias  struct  class  enum protocol  var  func
 ‌ import-path → import-path-identifier  import-path-identifier.import-path
 ‌ import-path-identifier → identifier  operator”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l

 “GRAMMAR OF A CONSTANT DECLARATION
 
 ‌ constant-declaration → attributesoptdeclaration-specifiersoptletpattern-initializer-list
 ‌ pattern-initializer-list → pattern-initializer  pattern-initializer,pattern-initializer-list
 ‌ pattern-initializer → patterninitializeropt
 ‌ initializer → =expression
 ”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A VARIABLE DECLARATION
 
 ‌ variable-declaration → variable-declaration-headpattern-initializer-list
 ‌ variable-declaration → variable-declaration-headvariable-nametype-annotationcode-block
 ‌ variable-declaration → variable-declaration-headvariable-nametype-annotationgetter-setter-block
  variable-declaration → variable-declaration-headvariable-nametype-annotationgetter-setter-keyword-block
 ‌ variable-declaration → variable-declaration-headvariable-nametype-annotationinitializeroptwillSet-didSet-block
 ‌ variable-declaration-head → attributesoptdeclaration-specifiersoptvar
 ‌ variable-name → identifier
 ‌ getter-setter-block → {getter-clausesetter-clauseopt}
 ‌ getter-setter-block → {setter-clausegetter-clause}
 ‌ getter-clause → attributesoptgetcode-block
 ‌ setter-clause → attributesoptsetsetter-nameoptcode-block
 ‌ setter-name → (identifier)
 ‌ getter-setter-keyword-block → {getter-keyword-clausesetter-keyword-clauseopt}
 ‌ getter-setter-keyword-block → {setter-keyword-clausegetter-keyword-clause}
 ‌ getter-keyword-clause → attributesoptget
 ‌ setter-keyword-clause → attributesoptset
 ‌ willSet-didSet-block → {willSet-clausedidSet-clauseopt}
 ‌ willSet-didSet-block → {didSet-clausewillSet-clause}
 ‌ willSet-clause → attributesoptwillSetsetter-nameoptcode-block
 ‌ didSet-clause → attributesoptdidSetsetter-nameoptcode-block
 ”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A TYPE ALIAS DECLARATION
 
 ‌ typealias-declaration → typealias-headtypealias-assignment
 ‌ typealias-head → typealiastypealias-name
 ‌ typealias-name → identifier
 ‌ typealias-assignment → =type”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A FUNCTION DECLARATION
 
 ‌ function-declaration → function-headfunction-namegeneric-parameter-clauseoptfunction-signaturefunction-body
 ‌ function-head → attributesoptdeclaration-specifiersoptfunc
 ‌ function-name → identifier  operator
 ‌ function-signature → parameter-clausesfunction-resultopt
 ‌ function-result → ->attributesopttype
 ‌ function-body → code-block
 ‌ parameter-clauses → parameter-clauseparameter-clausesopt
 ‌ parameter-clause → ()  (parameter-list...opt)
 ‌ parameter-list → parameter  parameter,parameter-list
 ‌ parameter → inoutoptletopt#optparameter-namelocal-parameter-nameopttype-annotationdefault-argument-clauseopt
 ‌ parameter → inoutoptvar#optparameter-namelocal-parameter-nameopttype-annotationdefault-argument-clauseopt
 ‌ parameter → attributesopttype
 ‌ parameter-name → identifier  _
 ‌ local-parameter-name → identifier  _
 ‌ default-argument-clause → =expression”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF AN ENUMERATION DECLARATION
 
 ‌ enum-declaration → attributesoptunion-style-enum attributesoptraw-value-style-enum
 ‌ union-style-enum → enum-namegeneric-parameter-clauseopt{union-style-enum-membersopt}
 ‌ union-style-enum-members → union-style-enum-memberunion-style-enum-membersopt
 ‌ union-style-enum-member → declaration  union-style-enum-case-clause
 ‌ union-style-enum-case-clause → attributesoptcaseunion-style-enum-case-list
 ‌ union-style-enum-case-list → union-style-enum-case  union-style-enum-case,union-style-enum-case-list
 ‌ union-style-enum-case → enum-case-nametuple-typeopt
 ‌ enum-name → identifier
 ‌ enum-case-name → identifier
 ‌ raw-value-style-enum → enum-namegeneric-parameter-clauseopt:type-identifier{raw-value-style-enum-membersopt}
 ‌ raw-value-style-enum-members → raw-value-style-enum-memberraw-value-style-enum-membersopt
 ‌ raw-value-style-enum-member → declaration  raw-value-style-enum-case-clause
 ‌ raw-value-style-enum-case-clause → attributesoptcaseraw-value-style-enum-case-list
 ‌ raw-value-style-enum-case-list → raw-value-style-enum-case raw-value-style-enum-case,raw-value-style-enum-case-list
 ‌ raw-value-style-enum-case → enum-case-nameraw-value-assignmentopt
 ‌ raw-value-assignment → =literal”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A STRUCTURE DECLARATION
 
 ‌ struct-declaration → attributesoptstructstruct-namegeneric-parameter-clauseopttype-inheritance-clauseoptstruct-body
 ‌ struct-name → identifier
 ‌ struct-body → {declarationsopt}”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “‌ class-declaration → attributesoptclassclass-namegeneric-parameter-clauseopttype-inheritance-clauseoptclass-body
 ‌ class-name → identifier
 ‌ class-body → {declarationsopt}”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A PROTOCOL DECLARATION
 
 ‌ protocol-declaration → attributesoptprotocolprotocol-nametype-inheritance-clauseoptprotocol-body
 ‌ protocol-name → identifier
 ‌ protocol-body → {protocol-member-declarationsopt}
 ‌ protocol-member-declaration → protocol-property-declaration
 ‌ protocol-member-declaration → protocol-method-declaration
 ‌ protocol-member-declaration → protocol-initializer-declaration
 ‌ protocol-member-declaration → protocol-subscript-declaration
 ‌ protocol-member-declaration → protocol-associated-type-declaration
 ‌ protocol-member-declarations → protocol-member-declarationprotocol-member-declarationsopt”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A PROTOCOL PROPERTY DECLARATION
 
 ‌ protocol-property-declaration → variable-declaration-headvariable-nametype-annotationgetter-setter-keyword-block
 ”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A PROTOCOL METHOD DECLARATION
 
 ‌ protocol-method-declaration → function-headfunction-namegeneric-parameter-clauseoptfunction-signature
 ”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A PROTOCOL INITIALIZER DECLARATION
 
 ‌ protocol-initializer-declaration → initializer-headgeneric-parameter-clauseoptparameter-clause”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A PROTOCOL SUBSCRIPT DECLARATION
 
 ‌ protocol-subscript-declaration → subscript-headsubscript-resultgetter-setter-keyword-block”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A PROTOCOL ASSOCIATED TYPE DECLARATION
 
 ‌ protocol-associated-type-declaration → typealias-headtype-inheritance-clauseopttypealias-assignmentopt
 ‌”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF AN INITIALIZER DECLARATION
 
 ‌ initializer-declaration → initializer-headgeneric-parameter-clauseoptparameter-clauseinitializer-body
 ‌ initializer-head → attributesoptconvenienceoptinit
 ‌ initializer-body → code-block”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A DEINITIALIZER DECLARATION
 
 ‌ deinitializer-declaration → attributesoptdeinitcode-block
 ‌”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF AN EXTENSION DECLARATION
 
 ‌ extension-declaration → extensiontype-identifiertype-inheritance-clauseoptextension-body
 ‌ extension-body → {declarationsopt}”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF A SUBSCRIPT DECLARATION
 
 ‌ subscript-declaration → subscript-headsubscript-resultcode-block
 ‌ subscript-declaration → subscript-headsubscript-resultgetter-setter-block
 ‌ subscript-declaration → subscript-headsubscript-resultgetter-setter-keyword-block
 ‌ subscript-head → attributesoptsubscriptparameter-clause
 ‌ subscript-result → ->attributesopttype”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 “GRAMMAR OF AN OPERATOR DECLARATION
 
 ‌ operator-declaration → prefix-operator-declaration  postfix-operator-declaration  infix-operator-declaration
 ‌ prefix-operator-declaration → operatorprefixoperator{}
 ‌ postfix-operator-declaration → operatorpostfixoperator{}
 ‌ infix-operator-declaration → operatorinfixoperator{infix-operator-attributesopt}
 ‌ infix-operator-attributes → precedence-clauseoptassociativity-clauseopt
 ‌ precedence-clause → precedenceprecedence-level
 ‌ precedence-level → Digit 0 through 255
 ‌ associativity-clause → associativityassociativity
 ‌ associativity → left  right  none”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 
 “GRAMMAR OF AN ATTRIBUTE
 
 ‌ attribute → @attribute-nameattribute-argument-clauseopt
 ‌ attribute-name → identifier
 ‌ attribute-argument-clause → (balanced-tokensopt)
 ‌ attributes → attributeattributesopt
 ‌ balanced-tokens → balanced-tokenbalanced-tokensopt
 ‌ balanced-token → (balanced-tokensopt)
 ‌ balanced-token → [balanced-tokensopt]
 ‌ balanced-token → {balanced-tokensopt}
 ‌ balanced-token → Any identifier, keyword, literal, or operator
 ‌ balanced-token → Any punctuation except (, ), [, ], {, or }”
 
 摘录来自: Apple Inc. “The Swift Programming Language”。 iBooks. https://itun.es/cn/jEUH0.l
 
 
*/