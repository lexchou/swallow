#ifndef PARSER_H
#define PARSER_H
#include "swift_conf.h"
#include "tokenizer/tokens.h"
#include <string>

SWIFT_NS_BEGIN

class Tokenizer;
class Node;
class Expression;
class FunctionCall;
class SymbolRegistry;
class Identifier;
class NodeFactory;
class Closure;
class CodeBlock;
class Statement;
class ParenthesizedExpression;
class TypeNode;
class CaseStatement;
class Tuple;
class TupleType;
class TypeIdentifier;
class ProtocolComposition;
class Attribute;
class Declaration;
class Pattern;
class Variable;
class Parameters;
class GenericParameters;
class Parser
{
    friend struct Flags;
public:
    Parser(NodeFactory* nodeFactory, SymbolRegistry* symbolRegistry);
    ~Parser();
public:
    Node* parse(const wchar_t* code);
    void setFileName(const wchar_t* fileName);
    void setFunctionName(const wchar_t* function);
public:
    Node* parseType(const wchar_t* code);
private:
    TypeNode* parseType();
    TypeNode* parseTypeAnnotation();
    
    TupleType* parseTupleType();
    TypeIdentifier* parseTypeIdentifier();
    ProtocolComposition* parseProtocolComposition();
    GenericParameters* parseGenericParameters();
private://statement
    Statement* parseStatement();
    Statement* parseLoopStatement();
    Statement* parseForLoop();
    Statement* parseWhileLoop();
    Statement* parseDoLoop();
    Statement* parseIf();
    Statement* parseSwitch();
    void parseSwitchStatements(CaseStatement* case_);
    Statement* parseBreak();
    Statement* parseContinue();
    Statement* parseFallthrough();
    Statement* parseReturn();
    Statement* parseLabeledStatement();
    Expression* parseConditionExpression();
    
    CodeBlock* parseCodeBlock();
private://pattern
    Pattern* parsePattern();
    Pattern* parseTuple();
    Pattern* parseEnumPattern();
    Pattern* parseTypeCastingPattern();
private://Attribute
    void parseAttributes(std::vector<Attribute*>& attributes);
    Attribute* parseAttribute();
    void parseBalancedToken(Attribute* attr);
    void parseBalancedTokens(Attribute* attr, const wchar_t* end);
    
private://declaration
    Declaration* parseDeclaration();
    int parseDeclarationSpecifiers();
    void verifyDeclarationSpecifiers(const Token& token, int actualSpecifiers, int expectedSpecifiers);
    Declaration* parseImport(const std::vector<Attribute*>& attrs);
    Declaration* parseLet(const std::vector<Attribute*>& attrs, int specifiers);
    Declaration* parseVar(const std::vector<Attribute*>& attrs, int specifiers);
    Variable* parseVariableDeclaration();
    void parseWillSetDidSetBlock(Variable* variable);
    void parseWillSetClause(Variable* variable, bool opt);
    void parseDidSetClause(Variable* variable, bool opt);
    
    std::pair<CodeBlock*, std::pair<std::wstring, CodeBlock*> > parseGetterSetterBlock();
    std::pair<std::wstring, CodeBlock*> parseSetterClause();
    std::pair<CodeBlock*, CodeBlock*> parseGetterSetterKeywordBlock();
    
    Declaration* parseTypealias(const std::vector<Attribute*>& attrs);
    Declaration* parseFunc(const std::vector<Attribute*>& attrs, int specifiers);
    Parameters* parseParameterClause();

    Declaration* parseEnum(const std::vector<Attribute*>& attrs);
    Declaration* parseRawValueEnum(const std::vector<Attribute*>& attrs, const std::wstring& name, TypeIdentifier* baseType);
    Declaration* parseUnionEnum(const std::vector<Attribute*>& attrs, const std::wstring& name);

    
    Declaration* parseStruct(const std::vector<Attribute*>& attrs);
    Declaration* parseClass(const std::vector<Attribute*>& attrs);
    Declaration* parseProtocol(const std::vector<Attribute*>& attrs);
    Declaration* parseInit(const std::vector<Attribute*>& attrs);
    Declaration* parseDeinit(const std::vector<Attribute*>& attrs);
    Declaration* parseExtension(const std::vector<Attribute*>& attrs);
    Declaration* parseSubscript(const std::vector<Attribute*>& attrs);
    Declaration* parseOperator(const std::vector<Attribute*>& attrs);
private://expression
    Expression* parseFloat();
    Expression* parseInteger();
    Expression* parseString();
    Expression* parseExpression();
    Expression* parseBinaryExpression(Expression* lhs);
    Expression* parsePrefixExpression();
    Expression* parsePostfixExpression();
    FunctionCall* parseFunctionCallExpression();
    Expression* parsePrimaryExpression();
    Expression* parseLiteralExpression();
    Expression* parseParenthesizedExpression();
    void parseExpressionItem(ParenthesizedExpression* parent);
    Expression* parseLiteral();
    
    Expression* parseSelfExpression();
    Expression* parseSuperExpression();
    Identifier* parseIdentifier();
    Closure* parseClosureExpression();
    
    std::pair<Expression*, Expression*> parseDictionaryLiteralItem();
private:
    /**
     * Read next token from tokenizer, throw exception if EOF reached.
     */
    void expect_next(Token& token);
    /**
     * Restore the position of tokenizer to specified token
     */
    void restore(Token& token);
    /**
     * Read next token from tokenizer, return false if EOF reached.
     */
    bool next(Token& token);
    /**
     * Peek next token from tokenizer, return false if EOF reached.
     */
    bool peek(Token& token);
    /**
     * Check if the following token is an identifier, throw exception if not matched
     */
    void expect_identifier(Token& token);
    /**
     * Check if the following token is the specified one, throw exception if not matched
     */
    void expect(const wchar_t* token);
    void expect(const wchar_t* token, Token&);
    /**
     * Check if the following token is the specified keyword, throw exception if not matched
     */
    void expect(Keyword::T keyword);
    void expect(Keyword::T keyword, Token&);

    /**
     * Throw an exception with the unexpected token
     */
    void unexpected(const Token& token);

    /**
     * Check if the following token is the specified one, consume the token and return true if matched or return false if not.
     */
    bool match(const wchar_t* token);
    bool match(Keyword::T keyword);
    bool match(const wchar_t* token, Token&);
    bool match(Keyword::T keyword, Token&);
    /**
     * Check if the following token is an identifier(keywords included), consume the token and return true if matched or rollback and return false
     */
    bool match_identifier(Token& token);
    
    /**
     * Return true if the next token is the specified one, no token will be consumed
     */
    bool predicate(const wchar_t* token);

    void tassert(Token& token, bool cond);
private:
    Tokenizer* tokenizer;
    NodeFactory* nodeFactory;
    SymbolRegistry* symbolRegistry;
    std::wstring fileName;
    int fileHash;
    std::wstring functionName;
    int flags;
};



SWIFT_NS_END

#endif//PARSER_H
